/* USER CODE BEGIN Header */
/**
******************************************************************************
* @file           : main.c
* @brief          : Main program body
******************************************************************************
* @attention
*
* Copyright (c) 2025 STMicroelectronics.
* All rights reserved.
*
* This software is licensed under terms that can be found in the LICENSE file
* in the root directory of this software component.
* If no LICENSE file comes with this software, it is provided AS-IS.
*
******************************************************************************
*/
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
// #define UART_DBG
lwrb_t usart_tx_rb;
char uart_txt_buf[512];
uint8_t usart_tx_rb_data[1024];
volatile size_t usart_tx_dma_current_len;

struct can_buffer_slot
{
    uint16_t message_priority;
    uint8_t size;
    uint8_t data[8];
};

lwrb_t can_tx_rb;
char can_tx_rb_data[sizeof(struct can_buffer_slot) * 32];
volatile size_t can_tx_rb_current_len;
uint8_t sleep_guard = 1;
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/*
bit 10 9 8 7 6 5 4 3 2 1 0
     m m r r D D D D D D D

m = message type -> 00: critical errors | 01: command | 10: fast telemetry | 11: slow telemetry
r = reserved(11) -> for finer prio control if we need it
D = node ID -> lower id = higher priority (0x7F - 0x00)
*/

#define CAN_RX

#define CAN_MSG_PRI_ERR (0b00 << 9)
#define CAN_MSG_PRI_CMD (0b01 << 9)
#define CAN_MSG_PRI_TLM_FAST (0b10 << 9)
#define CAN_MSG_PRI_TLM_SLOW (0b11 << 9)

#define CAN_RES1 (0b11 << 7)

#define CAN_BATTERY_SUBGROUP (0b1101 << 3)

// TODO: UNIQUE BOARD SUFFIX ID?
const uint16_t CAN_ID_BASE = CAN_RES1 | CAN_BATTERY_SUBGROUP | 0b000;
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
FDCAN_HandleTypeDef hfdcan2;

I2C_HandleTypeDef hi2c1;

UART_HandleTypeDef hlpuart1;
DMA_HandleTypeDef hdma_lpuart1_tx;

TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim6;
TIM_HandleTypeDef htim7;
TIM_HandleTypeDef htim15;
TIM_HandleTypeDef htim16;

/* USER CODE BEGIN PV */
// TIM_HandleTypeDef htim2; // PWM TIMER
TIM_HandleTypeDef *htim_interrupt_watchdog = &htim6;
// TIM_HandleTypeDef htim7; // TONE SWITCHING TIMER
// TIM_HandleTypeDef htim15; // BQ769X2 us TIMER
TIM_HandleTypeDef *htim_can_disconnect = &htim16;

uint8_t bq769x2_pf = 0,
        bq769x2_initialized = 0;
uint32_t cb_telemetry_timer_ms = 0;
const uint32_t RESEND_INTERVAL = 10000;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_FDCAN2_Init(void);
static void MX_I2C1_Init(void);
static void MX_LPUART1_UART_Init(void);
static void MX_TIM2_Init(void);
static void MX_TIM7_Init(void);
static void MX_TIM15_Init(void);
static void MX_TIM6_Init(void);
static void MX_TIM16_Init(void);
/* USER CODE BEGIN PFP */
static void FDCAN2_classic_config(void);
void usart_send_string(const char *str);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

uint8_t can2_move_to_fifo()
{
    uint32_t primask;
    uint8_t started = 0;
    struct can_buffer_slot slot;

    primask = __get_PRIMASK();
    __disable_irq();
    if (HAL_FDCAN_GetTxFifoFreeLevel(&hfdcan2) > 0 && lwrb_read(&can_tx_rb, &slot, sizeof(slot)) > 0)
    {
        HAL_TIM_Base_Start_IT(htim_can_disconnect);
        __HAL_TIM_SET_COUNTER(htim_can_disconnect, 0);

        HAL_GPIO_WritePin(FDCAN12_SHUTDOWN_GPIO_Port, FDCAN12_SHUTDOWN_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(FDCAN2_SILENCE_GPIO_Port, FDCAN2_SILENCE_Pin, GPIO_PIN_RESET);

        HAL_GPIO_WritePin(RJ8P8C_LED_Y_GPIO_Port, RJ8P8C_LED_Y_Pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(RJ8P8C_LED_G_GPIO_Port, RJ8P8C_LED_G_Pin, GPIO_PIN_SET);

        FDCAN_TxHeaderTypeDef can_classic_tx_header = {
            .IdType = FDCAN_STANDARD_ID,
            .TxFrameType = FDCAN_DATA_FRAME,
            .ErrorStateIndicator = FDCAN_ESI_ACTIVE,
            .BitRateSwitch = FDCAN_BRS_OFF,
            .FDFormat = FDCAN_CLASSIC_CAN,
            .TxEventFifoControl = FDCAN_STORE_TX_EVENTS,
            .MessageMarker = 0,
            .DataLength = slot.size,
            .Identifier = CAN_ID_BASE | slot.message_priority,
        };

        uint8_t delay = 0;
        while (HAL_FDCAN_AddMessageToTxFifoQ(&hfdcan2, &can_classic_tx_header, slot.data) != HAL_OK)
        {
            bq769x2_delay_us(250);
            delay++;
        }
        if (delay > 0)
        {
#ifdef UART_DBG
            char buf[32];
            snprintf(buf, sizeof(buf), "CAN_OVERFLOW %d\r\n", delay);
            usart_send_string(buf);
#endif
        }
        usart_send_string("CAN_OK\r\n");
        started = 1;
        __HAL_TIM_SET_COUNTER(htim_can_disconnect, 0);
    }
    __set_PRIMASK(primask);
    return started;
}

void can2_transmit(void *data, uint32_t length, uint16_t id_priority)
{
    static struct can_buffer_slot slot_null;
    struct can_buffer_slot slot = {
        .size = length,
        .message_priority = id_priority,
        .data = {}};
    uint8_t fail = 9;

    memcpy(slot.data, data, length);

    /* Write data to transmit buffer */
    while (fail-- && lwrb_write(&can_tx_rb, &slot, sizeof(struct can_buffer_slot)) != sizeof(struct can_buffer_slot))
    {
        char err_str[] = "can buffer overrun 0\r\n";
        err_str[sizeof(err_str) - 4] = '0' + fail;
        usart_send_string(err_str);
        lwrb_read(&can_tx_rb, &slot_null, sizeof(slot_null));
    }
    can2_move_to_fifo();
}

void tx_ssbc(uint16_t priority)
{
    struct safety_status_BC ssbc = CREATE(
        safety_status_BC,
        .safety_status_B = DIRCMD_READ_SafetyStatusB_Raw(),
        .safety_status_C = DIRCMD_READ_SafetyStatusC_Raw());
    can2_transmit(&ssbc, sizeof(ssbc), priority);

#ifdef UART_DBG
    snprintf(uart_txt_buf, sizeof(uart_txt_buf), SSBC_UART_FMT);
    usart_send_string(uart_txt_buf);
#endif
}

static void tx_snapshot(int16_t snapshot[16], enum event_type snapshot_type)
{
    struct cv_group cv_groups[6];
    FILL_CELL_VOLTAGES(cv_groups, snapshot_type,
                       snapshot[0],
                       snapshot[1],
                       snapshot[2],
                       snapshot[3],
                       snapshot[4],
                       snapshot[5],
                       snapshot[6],
                       snapshot[7],
                       snapshot[8],
                       snapshot[9],
                       snapshot[10],
                       snapshot[11],
                       snapshot[12],
                       snapshot[13],
                       snapshot[14],
                       snapshot[15], 0);
    for (uint8_t i = 0; i < 6; i++)
        can2_transmit(&cv_groups[i], sizeof(cv_groups[i]), CAN_MSG_PRI_ERR);
}

void tx_ssa(uint16_t priority)
{
    static int16_t snapshot[16];
    struct safety_status_A ssa = CREATE(
        safety_status_A,
        .safety_status_A = DIRCMD_READ_SafetyStatusA_Raw());
    can2_transmit(&ssa, sizeof(ssa), priority);

#ifdef UART_DBG
    snprintf(uart_txt_buf, sizeof(uart_txt_buf), SSA_UART_FMT);
    usart_send_string(uart_txt_buf);
#endif

    if (BIT_SafetyStatusA_COV(ssa.safety_status_A) && priority == CAN_MSG_PRI_ERR)
    {
        memset(snapshot, 0x00, sizeof(snapshot));
        bq769x2_h_get_cov_snapshot(snapshot, sizeof(snapshot));
        tx_snapshot(snapshot, COV_SNAPSHOT_E);
    }

    if (BIT_SafetyStatusA_CUV(ssa.safety_status_A) && priority == CAN_MSG_PRI_ERR)
    {
        memset(snapshot, 0x00, sizeof(snapshot));
        bq769x2_h_get_cuv_snapshot(snapshot, sizeof(snapshot));
        tx_snapshot(snapshot, CUV_SNAPSHOT_E);
    }
}

void tx_pf(uint16_t priority)
{
    struct pf_status pf_status = CREATE(pf_status,
                                        .pf_status_A = DIRCMD_READ_PFStatusA_Raw(),
                                        .pf_status_B = DIRCMD_READ_PFStatusB_Raw(),
                                        .pf_status_C = DIRCMD_READ_PFStatusC_Raw(),
                                        .pf_status_D = DIRCMD_READ_PFStatusD_Raw());
    can2_transmit(&pf_status, sizeof(pf_status), priority);
}

void tx_pfalert(uint16_t priority)
{
    struct pf_alert pf_alert = CREATE(pf_alert,
                                      .pf_alert_A = DIRCMD_READ_PFAlertA_Raw(),
                                      .pf_alert_B = DIRCMD_READ_PFAlertB_Raw(),
                                      .pf_alert_C = DIRCMD_READ_PFAlertC_Raw(),
                                      .pf_alert_D = DIRCMD_READ_PFAlertD_Raw());
    can2_transmit(&pf_alert, sizeof(pf_alert), priority);
}

void tx_safety_alert(uint16_t priority)
{
    struct safety_alert sf_alert = CREATE(safety_alert,
                                          .safety_alert_A = DIRCMD_READ_SafetyAlertA_Raw(),
                                          .safety_alert_B = DIRCMD_READ_SafetyAlertB_Raw(),
                                          .safety_alert_C = DIRCMD_READ_SafetyAlertC_Raw());
    can2_transmit(&sf_alert, sizeof(sf_alert), priority);
}

void tx_fullscan()
{
    // Check if FULLSCAN is complete. If set, new measurements are available
    // cell voltages
    // current
    // temperature sensors

    struct cv_group cv_groups[6];

    FILL_CELL_VOLTAGES(cv_groups, FULLSCAN_E,
                       DIRCMD_READ_Cell1Voltage_mV(),
                       DIRCMD_READ_Cell2Voltage_mV(),
                       DIRCMD_READ_Cell3Voltage_mV(),
                       DIRCMD_READ_Cell4Voltage_mV(),
                       DIRCMD_READ_Cell5Voltage_mV(),
                       DIRCMD_READ_Cell6Voltage_mV(),
                       DIRCMD_READ_Cell7Voltage_mV(),
                       DIRCMD_READ_Cell8Voltage_mV(),
                       DIRCMD_READ_Cell9Voltage_mV(),
                       DIRCMD_READ_Cell10Voltage_mV(),
                       DIRCMD_READ_Cell11Voltage_mV(),
                       DIRCMD_READ_Cell12Voltage_mV(),
                       DIRCMD_READ_Cell13Voltage_mV(),
                       DIRCMD_READ_Cell14Voltage_mV(),
                       DIRCMD_READ_Cell15Voltage_mV(),
                       DIRCMD_READ_Cell16Voltage_mV(),
                       DIRCMD_READ_CC2Current_userA());

    struct temperature temperature =
        CREATE(temperature,
               .ts1_0K1 = DIRCMD_READ_TS1Temperature_0K1(),
               .ts3_0K1 = DIRCMD_READ_TS3Temperature_0K1(),
               .tint_0K1 = DIRCMD_READ_IntTemperature_0K1());

    struct pack_voltage pack_voltage =
        CREATE(pack_voltage,
               .stack_voltage_userV = DIRCMD_READ_StackVoltage_userV(),
               .ld_pin_voltage_userV = DIRCMD_READ_LDPinVoltage_userV(),
               .pack_pin_voltage_userV = DIRCMD_READ_PACKPinVoltage_userV());

#ifdef UART_DBG
    usart_send_string("--- FULLSCAN EVENT ---\r\n");
    for (uint8_t i = 0; i < 5; i++)
    {
        for (uint8_t j = 0; j < 3; j++)
        {
            snprintf(uart_txt_buf, sizeof(uart_txt_buf), ">c_%d_mV:%dmV\r\n", j, cv_groups[i].cv[j]);
            usart_send_string(uart_txt_buf);
        }
    }
    snprintf(uart_txt_buf, sizeof(uart_txt_buf), FULLSCAN_FMT);
    usart_send_string(uart_txt_buf);
#endif

    for (uint8_t i = 0; i < 6; i++)
        can2_transmit(&cv_groups[i], sizeof(cv_groups[i]), CAN_MSG_PRI_TLM_SLOW);

    can2_transmit(&temperature, sizeof(temperature), CAN_MSG_PRI_TLM_SLOW);
    can2_transmit(&pack_voltage, sizeof(pack_voltage), CAN_MSG_PRI_TLM_SLOW);
}

void tx_pack_status(uint16_t priority)
{
    can2_transmit(CREATE_PTR_L(pack_status,
                               .battery_status = DIRCMD_READ_BatteryStatus_Raw(),
                               .control_status = DIRCMD_READ_ControlStatus_Raw(),
                               .fet_status = DIRCMD_READ_FETStatus_Raw()),
                  priority);
}

void tx_cb_status(uint16_t priority)
{
    uint32_t cell_balancing_time[16];
    memset(cell_balancing_time, 0x00, sizeof(cell_balancing_time));
    bq769x2_h_cb_get_cell_balancing_time(cell_balancing_time, sizeof(cell_balancing_time));
    for (uint8_t i = 0; i < 16; i++)
    {
        struct cb_time _cb_time_s = CREATE(
            cb_time,
            .offset = i,
            .cb_time_s = cell_balancing_time[i]);

        can2_transmit(&_cb_time_s, sizeof(_cb_time_s), priority);
    }

    struct cb_status _cb_status = CREATE(
        cb_status,
        .cb_active_cells = bq769x2_h_cb_get_active_cells(),
        .cb_present_time_s = bq769x2_h_cb_get_pack_balancing_time());

    can2_transmit(&_cb_status, sizeof(_cb_status), priority);
}

void tx_event(uint16_t events)
{
    events &= ~(AlarmStatus_FULLSCAN_CLEAR | AlarmStatus_ADSCAN_CLEAR);

    if (events)
        can2_transmit(CREATE_PTR_L(alarm_event,
                                   .alarm_excl_ADSCAN_FULLSCAN = events),
                      CAN_MSG_PRI_ERR);
}

void reset_fault_alarm(const uint16_t alarm_mask)
{
    // ALARM AND FAULT LAMP
    if (BIT_AlarmStatus_PF(alarm_mask))
    {
        sfx_twotone(65535, 100, 250);
        mask_fault_lamp(FL_REPEAT);
    }
    else if (BIT_AlarmStatus_SSBC(alarm_mask) || BIT_AlarmStatus_SSA(alarm_mask) /* || BIT_AlarmStatus_XCHG(alarm_mask) || BIT_AlarmStatus_XDSG(alarm_mask)*/)
    {
        sfx_twotone(20, 250, 100);
        mask_fault_lamp(FL_REPEAT);
    }
    else if (BIT_AlarmStatus_MSK_PFALERT(alarm_mask) || BIT_AlarmStatus_MSK_SFALERT(alarm_mask))
    {
        sfx_fourtone(50);
        mask_fault_lamp(FL_ONESHOT);
    }
    else
    {
        mask_fault_lamp(FL_DISABLED);
    }
}

void refresh_faults(uint16_t mask);

uint16_t alarm_latched = 0,
         alarm_raw_now = 0,
         alarm_raw_last = 0;

void alarm_cb()
{
    if (!bq769x2_initialized)
        return;

    // FORCE SEND FULL STATUS EVERY X INTERRUPTS;
    const uint16_t repeat_msg_divisor = 16;

    // TIM6 -> ALERT INTERRUPT WATCHDOG
    __HAL_TIM_SET_COUNTER(htim_interrupt_watchdog, 0);
    static uint16_t alarm_trig = 0,
                    repeat_msg_cnt = 0;

    uint16_t can_priority = CAN_MSG_PRI_ERR;
    uint8_t retransmit = 0;

    if (repeat_msg_cnt++ == repeat_msg_divisor)
    {
        repeat_msg_cnt = 0;
        alarm_trig = 0xFFFF;
        retransmit = 1;
        can_priority = CAN_MSG_PRI_TLM_SLOW;

        // EXCEPTION - ONLY PERIODICALLY TRANSMIT CB IF CB IS ACTIVE.
        // alarm_trig = (alarm_trig ^ BIT_AlarmRawStatus_CB(alarm_trig)) | BIT_AlarmRawStatus_CB(alarm_raw_now);
    }
    else
    {
        alarm_latched = DIRCMD_READ_AlarmStatus_Raw();
        alarm_raw_now = DIRCMD_READ_AlarmRawStatus_Raw();

        // RISING (LATCHED) OR FALLING EDGE
        alarm_trig = (alarm_latched) | (alarm_raw_last & (alarm_raw_last ^ alarm_raw_now));
    }

    if (BIT_AlarmStatus_FULLSCAN(alarm_trig))
    {
        tx_pack_status(CAN_MSG_PRI_TLM_SLOW);
        tx_fullscan();
        DIRCMD_WRITE_AlarmStatus_Raw(AlarmStatus_FULLSCAN_CLEAR);
    }
    if (BIT_AlarmStatus_PF(alarm_trig))
    {
        tx_pf(can_priority);
        DIRCMD_WRITE_AlarmStatus_Raw(AlarmStatus_PF_CLEAR);
        HAL_PWR_DisableSleepOnExit(); // EXIT HANDLER IS NOT INTERRUPT BASED
        bq769x2_pf = !retransmit;
    }
    if (BIT_AlarmStatus_SSBC(alarm_trig))
    {
        tx_ssbc(can_priority);
        DIRCMD_WRITE_AlarmStatus_Raw(AlarmStatus_SSBC_CLEAR);
    }
    if (BIT_AlarmStatus_SSA(alarm_trig))
    {
        tx_ssa(can_priority);
        DIRCMD_WRITE_AlarmStatus_Raw(AlarmStatus_SSA_CLEAR);
    }
    if (BIT_AlarmStatus_MSK_PFALERT(alarm_trig))
    {
        tx_pfalert(can_priority);
        DIRCMD_WRITE_AlarmStatus_Raw(AlarmStatus_MSK_PFALERT_CLEAR);
    }
    if (BIT_AlarmStatus_MSK_SFALERT(alarm_trig))
    {
        tx_safety_alert(can_priority);
        DIRCMD_WRITE_AlarmStatus_Raw(AlarmStatus_MSK_SFALERT_CLEAR);
    }
    if (BIT_AlarmStatus_XCHG(alarm_trig) || BIT_AlarmStatus_XDSG(alarm_trig))
    {
        tx_pack_status(can_priority);
        DIRCMD_WRITE_AlarmStatus_Raw(AlarmStatus_XCHG_CLEAR | AlarmStatus_XDSG_CLEAR);
    }

    // ALARM UNLATCHED
    if (BIT_AlarmRawStatus_CB(alarm_trig))
    {
        tx_cb_status(can_priority);
    }

    // EVENT
    const uint16_t event_bits = alarm_latched & ~(AlarmStatus_FULLSCAN_CLEAR | AlarmStatus_ADSCAN_CLEAR);
    if (event_bits)
    {
        reset_fault_alarm(event_bits);
        tx_event(event_bits);
        DIRCMD_WRITE_AlarmStatus_Raw(event_bits);

#ifdef UART_DBG
        snprintf(uart_txt_buf, sizeof(uart_txt_buf), ALARM_UART_FMT);
        usart_send_string(uart_txt_buf);
#endif
    }
    else
    {
        // CMD_SUBCMD_ALL_FETS_ON();
    }
    alarm_raw_last = alarm_raw_now;
}

/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void)
{

    /* USER CODE BEGIN 1 */
    lwrb_init(&usart_tx_rb, usart_tx_rb_data, sizeof(usart_tx_rb_data));
    lwrb_init(&can_tx_rb, can_tx_rb_data, sizeof(can_tx_rb_data));
    /* USER CODE END 1 */

    /* MCU Configuration--------------------------------------------------------*/

    /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
    HAL_Init();

    /* USER CODE BEGIN Init */

    /* USER CODE END Init */

    /* Configure the system clock */
    SystemClock_Config();

    /* USER CODE BEGIN SysInit */

    /* USER CODE END SysInit */

    /* Initialize all configured peripherals */
    MX_GPIO_Init();
    MX_DMA_Init();
    MX_FDCAN2_Init();
    MX_I2C1_Init();
    MX_LPUART1_UART_Init();
    MX_TIM2_Init();
    MX_TIM7_Init();
    MX_TIM15_Init();
    MX_TIM6_Init();
    MX_TIM16_Init();
    /* USER CODE BEGIN 2 */
    FDCAN2_classic_config();
#define USERA_mA 10
#define USERV_mV 10
    HAL_GPIO_WritePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin, GPIO_PIN_SET);
    HAL_TIM_Base_Start(&htim15);
    // TIM6 -> ALERT INTERRUPT WATCHDOG
    HAL_TIM_Base_Start_IT(htim_interrupt_watchdog);
    HAL_GPIO_WritePin(RST_SHUT_GPIO_Port, RST_SHUT_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(DFETOFF_GPIO_Port, DFETOFF_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(CFETOFF_GPIO_Port, CFETOFF_Pin, GPIO_PIN_RESET);
    bq769x2_delay_us(10000);

    // NOTE: do not call CMD_SUBCMD_RESET(); - power on boot loop
    bq769x2_init(); // Configure all of the BQ769x2 register settings
    // NOTE: do not execute CMD_SUBCMD_FET_ENABLE(); at this point - toggles FET_EN register to off.
    usart_send_string("BQ769X2 INITIALIZATION COMPLETED\r\n");
    alarm_cb();
    bq769x2_delay_us(60000);
    bq769x2_delay_us(60000);
    bq769x2_delay_us(60000);
    bq769x2_delay_us(60000); // wait to start measurements after FETs close
#ifdef UART_DBG
    HAL_UART_Init(&hlpuart1);
#endif

    alarm_cb();
    can2_transmit(CREATE_PTR_L(bms_status, .bms_status = BMS_STATUS_INIT), CAN_MSG_PRI_TLM_SLOW);
    can2_transmit(CREATE_PTR_L(bms_status, .bms_status = BMS_STATUS_INIT), CAN_MSG_PRI_TLM_SLOW);
    can2_transmit(CREATE_PTR_L(bms_status, .bms_status = BMS_STATUS_INIT), CAN_MSG_PRI_TLM_SLOW);
    HAL_GPIO_WritePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin, GPIO_PIN_RESET);
    bq769x2_initialized = 1;
    alarm_cb();
    sfx_onetone(932, 180, 253);
    bq769x2_delay_us(60000);
    HAL_PWR_EnableSleepOnExit(); // EASY SLEEP MODE - TODO: REIMPLEMENT WITH STOP MODE FOR MORE SAVING.

    /* USER CODE END 2 */

    /* Infinite loop */
    /* USER CODE BEGIN WHILE */
    while (1)
    {
        /* USER CODE END WHILE */

        /* USER CODE BEGIN 3 */
        if (bq769x2_pf)
        {
            // SHUTDOWN HOST MCU AND BQ.
            for (uint32_t i = 0; i < 20; i++)
            {
                HAL_Delay(500);
                can2_transmit(CREATE_PTR_L(bms_status, .bms_status = BMS_STATUS_SHUTDOWN), CAN_MSG_PRI_ERR);
            }
            CMD_SUBCMD_SHUTDOWN();
            CMD_SUBCMD_SHUTDOWN();
            __HAL_PWR_CLEAR_FLAG(PWR_FLAG_WUF);
            HAL_PWR_EnterSTANDBYMode();
            while (1)
                ;
        }
    }
    /* USER CODE END 3 */
}

/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void)
{
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

    /** Configure the main internal regulator output voltage
     */
    HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1);

    /** Initializes the RCC Oscillators according to the specified parameters
     * in the RCC_OscInitTypeDef structure.
     */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
    RCC_OscInitStruct.HSIState = RCC_HSI_ON;
    RCC_OscInitStruct.HSIDiv = RCC_HSI_DIV1;
    RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
    RCC_OscInitStruct.PLL.PLLM = RCC_PLLM_DIV1;
    RCC_OscInitStruct.PLL.PLLN = 8;
    RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
    RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV4;
    RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
        Error_Handler();
    }

    /** Initializes the CPU, AHB and APB buses clocks
     */
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
    {
        Error_Handler();
    }
}

/**
 * @brief FDCAN2 Initialization Function
 * @param None
 * @retval None
 */
static void MX_FDCAN2_Init(void)
{

    /* USER CODE BEGIN FDCAN2_Init 0 */

    /* USER CODE END FDCAN2_Init 0 */

    /* USER CODE BEGIN FDCAN2_Init 1 */

    /* USER CODE END FDCAN2_Init 1 */
    hfdcan2.Instance = FDCAN2;
    hfdcan2.Init.ClockDivider = FDCAN_CLOCK_DIV1;
    hfdcan2.Init.FrameFormat = FDCAN_FRAME_CLASSIC;
    hfdcan2.Init.Mode = FDCAN_MODE_NORMAL;
    hfdcan2.Init.AutoRetransmission = ENABLE;
    hfdcan2.Init.TransmitPause = ENABLE;
    hfdcan2.Init.ProtocolException = DISABLE;
    hfdcan2.Init.NominalPrescaler = 32;
    hfdcan2.Init.NominalSyncJumpWidth = 16;
    hfdcan2.Init.NominalTimeSeg1 = 2;
    hfdcan2.Init.NominalTimeSeg2 = 1;
    hfdcan2.Init.DataPrescaler = 1;
    hfdcan2.Init.DataSyncJumpWidth = 4;
    hfdcan2.Init.DataTimeSeg1 = 5;
    hfdcan2.Init.DataTimeSeg2 = 4;
    hfdcan2.Init.StdFiltersNbr = 1;
    hfdcan2.Init.ExtFiltersNbr = 0;
    hfdcan2.Init.TxFifoQueueMode = FDCAN_TX_FIFO_OPERATION;
    if (HAL_FDCAN_Init(&hfdcan2) != HAL_OK)
    {
        Error_Handler();
    }
    /* USER CODE BEGIN FDCAN2_Init 2 */
    /* USER CODE END FDCAN2_Init 2 */
}

/**
 * @brief I2C1 Initialization Function
 * @param None
 * @retval None
 */
static void MX_I2C1_Init(void)
{

    /* USER CODE BEGIN I2C1_Init 0 */

    /* USER CODE END I2C1_Init 0 */

    /* USER CODE BEGIN I2C1_Init 1 */

    /* USER CODE END I2C1_Init 1 */
    hi2c1.Instance = I2C1;
    hi2c1.Init.Timing = 0x00300617;
    hi2c1.Init.OwnAddress1 = 0;
    hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
    hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
    hi2c1.Init.OwnAddress2 = 0;
    hi2c1.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
    hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
    hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
    if (HAL_I2C_Init(&hi2c1) != HAL_OK)
    {
        Error_Handler();
    }

    /** Configure Analogue filter
     */
    if (HAL_I2CEx_ConfigAnalogFilter(&hi2c1, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
    {
        Error_Handler();
    }

    /** Configure Digital filter
     */
    if (HAL_I2CEx_ConfigDigitalFilter(&hi2c1, 0) != HAL_OK)
    {
        Error_Handler();
    }
    /* USER CODE BEGIN I2C1_Init 2 */

    /* USER CODE END I2C1_Init 2 */
}

/**
 * @brief LPUART1 Initialization Function
 * @param None
 * @retval None
 */
static void MX_LPUART1_UART_Init(void)
{

    /* USER CODE BEGIN LPUART1_Init 0 */
#ifdef UART_DBG
    /* USER CODE END LPUART1_Init 0 */

    /* USER CODE BEGIN LPUART1_Init 1 */

    /* USER CODE END LPUART1_Init 1 */
    hlpuart1.Instance = LPUART1;
    hlpuart1.Init.BaudRate = 57600;
    hlpuart1.Init.WordLength = UART_WORDLENGTH_8B;
    hlpuart1.Init.StopBits = UART_STOPBITS_1;
    hlpuart1.Init.Parity = UART_PARITY_NONE;
    hlpuart1.Init.Mode = UART_MODE_TX_RX;
    hlpuart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    hlpuart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
    hlpuart1.Init.ClockPrescaler = UART_PRESCALER_DIV1;
    hlpuart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
    hlpuart1.FifoMode = UART_FIFOMODE_DISABLE;
    if (HAL_UART_Init(&hlpuart1) != HAL_OK)
    {
        Error_Handler();
    }
    if (HAL_UARTEx_SetTxFifoThreshold(&hlpuart1, UART_TXFIFO_THRESHOLD_1_8) != HAL_OK)
    {
        Error_Handler();
    }
    if (HAL_UARTEx_SetRxFifoThreshold(&hlpuart1, UART_RXFIFO_THRESHOLD_1_8) != HAL_OK)
    {
        Error_Handler();
    }
    if (HAL_UARTEx_DisableFifoMode(&hlpuart1) != HAL_OK)
    {
        Error_Handler();
    }
    /* USER CODE BEGIN LPUART1_Init 2 */
#endif
    /* USER CODE END LPUART1_Init 2 */
}

/**
 * @brief TIM2 Initialization Function
 * @param None
 * @retval None
 */
static void MX_TIM2_Init(void)
{

    /* USER CODE BEGIN TIM2_Init 0 */

    /* USER CODE END TIM2_Init 0 */

    TIM_ClockConfigTypeDef sClockSourceConfig = {0};
    TIM_MasterConfigTypeDef sMasterConfig = {0};
    TIM_OC_InitTypeDef sConfigOC = {0};

    /* USER CODE BEGIN TIM2_Init 1 */

    /* USER CODE END TIM2_Init 1 */
    htim2.Instance = TIM2;
    htim2.Init.Prescaler = HCLK_MHz - 1;
    htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
    htim2.Init.Period = 1000000 / (HCLK_kHz / 10) - 1;
    htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
    if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
    {
        Error_Handler();
    }
    sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
    if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
    {
        Error_Handler();
    }
    if (HAL_TIM_PWM_Init(&htim2) != HAL_OK)
    {
        Error_Handler();
    }
    sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
    sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
    if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
    {
        Error_Handler();
    }
    sConfigOC.OCMode = TIM_OCMODE_PWM1;
    sConfigOC.Pulse = 1000000 / 800 / 2;
    sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
    sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
    if (HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
    {
        Error_Handler();
    }
    /* USER CODE BEGIN TIM2_Init 2 */

    /* USER CODE END TIM2_Init 2 */
    HAL_TIM_MspPostInit(&htim2);
}

/**
 * @brief TIM6 Initialization Function
 * @param None
 * @retval None
 */
static void MX_TIM6_Init(void)
{

    /* USER CODE BEGIN TIM6_Init 0 */

    /* USER CODE END TIM6_Init 0 */

    TIM_MasterConfigTypeDef sMasterConfig = {0};

    /* USER CODE BEGIN TIM6_Init 1 */

    /* USER CODE END TIM6_Init 1 */
    htim6.Instance = TIM6;
    htim6.Init.Prescaler = HCLK_kHz - 1;
    htim6.Init.CounterMode = TIM_COUNTERMODE_UP;
    htim6.Init.Period = ALARM_CALLBACK_WATCHDOG_ms;
    htim6.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
    if (HAL_TIM_Base_Init(&htim6) != HAL_OK)
    {
        Error_Handler();
    }
    sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
    sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
    if (HAL_TIMEx_MasterConfigSynchronization(&htim6, &sMasterConfig) != HAL_OK)
    {
        Error_Handler();
    }
    /* USER CODE BEGIN TIM6_Init 2 */

    /* USER CODE END TIM6_Init 2 */
}

/**
 * @brief TIM7 Initialization Function
 * @param None
 * @retval None
 */
static void MX_TIM7_Init(void)
{

    /* USER CODE BEGIN TIM7_Init 0 */

    /* USER CODE END TIM7_Init 0 */

    TIM_MasterConfigTypeDef sMasterConfig = {0};

    /* USER CODE BEGIN TIM7_Init 1 */

    /* USER CODE END TIM7_Init 1 */
    htim7.Instance = TIM7;
    htim7.Init.Prescaler = HCLK_kHz - 1;
    htim7.Init.CounterMode = TIM_COUNTERMODE_UP;
    htim7.Init.Period = 65535;
    htim7.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
    if (HAL_TIM_Base_Init(&htim7) != HAL_OK)
    {
        Error_Handler();
    }
    sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
    sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
    if (HAL_TIMEx_MasterConfigSynchronization(&htim7, &sMasterConfig) != HAL_OK)
    {
        Error_Handler();
    }
    /* USER CODE BEGIN TIM7_Init 2 */

    /* USER CODE END TIM7_Init 2 */
}

/**
 * @brief TIM15 Initialization Function
 * @param None
 * @retval None
 */
static void MX_TIM15_Init(void)
{

    /* USER CODE BEGIN TIM15_Init 0 */

    /* USER CODE END TIM15_Init 0 */

    TIM_ClockConfigTypeDef sClockSourceConfig = {0};
    TIM_MasterConfigTypeDef sMasterConfig = {0};

    /* USER CODE BEGIN TIM15_Init 1 */

    /* USER CODE END TIM15_Init 1 */
    htim15.Instance = TIM15;
    htim15.Init.Prescaler = HCLK_MHz - 1;
    htim15.Init.CounterMode = TIM_COUNTERMODE_UP;
    htim15.Init.Period = 65535;
    htim15.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    htim15.Init.RepetitionCounter = 0;
    htim15.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
    if (HAL_TIM_Base_Init(&htim15) != HAL_OK)
    {
        Error_Handler();
    }
    sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
    if (HAL_TIM_ConfigClockSource(&htim15, &sClockSourceConfig) != HAL_OK)
    {
        Error_Handler();
    }
    sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
    sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
    if (HAL_TIMEx_MasterConfigSynchronization(&htim15, &sMasterConfig) != HAL_OK)
    {
        Error_Handler();
    }
    /* USER CODE BEGIN TIM15_Init 2 */

    /* USER CODE END TIM15_Init 2 */
}

/**
 * @brief TIM16 Initialization Function
 * @param None
 * @retval None
 */
static void MX_TIM16_Init(void)
{

    /* USER CODE BEGIN TIM16_Init 0 */

    /* USER CODE END TIM16_Init 0 */

    /* USER CODE BEGIN TIM16_Init 1 */

    /* USER CODE END TIM16_Init 1 */
    htim16.Instance = TIM16;
    htim16.Init.Prescaler = HCLK_kHz - 1;
    htim16.Init.CounterMode = TIM_COUNTERMODE_UP;
    htim16.Init.Period = 50;
    htim16.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    htim16.Init.RepetitionCounter = 0;
    htim16.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
    if (HAL_TIM_Base_Init(&htim16) != HAL_OK)
    {
        Error_Handler();
    }
    /* USER CODE BEGIN TIM16_Init 2 */

    /* USER CODE END TIM16_Init 2 */
}

/**
 * Enable DMA controller clock
 */
static void MX_DMA_Init(void)
{

    /* DMA controller clock enable */
    __HAL_RCC_DMA1_CLK_ENABLE();

    /* DMA interrupt init */
    /* DMA1_Channel1_IRQn interrupt configuration */
    HAL_NVIC_SetPriority(DMA1_Channel1_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(DMA1_Channel1_IRQn);
}

/**
 * @brief GPIO Initialization Function
 * @param None
 * @retval None
 */
static void MX_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    /* USER CODE BEGIN MX_GPIO_Init_1 */

    /* USER CODE END MX_GPIO_Init_1 */

    /* GPIO Ports Clock Enable */
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOF_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(GPIOA, LED_GREEN_Pin | BUCK_EN_Pin | FDCAN12_SHUTDOWN_Pin | FDCAN1_SILENCE_Pin, GPIO_PIN_RESET);

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(GPIOB, FAULT_LAMP_Pin | RJ8P8C_LED_G_Pin | RJ8P8C_LED_Y_Pin | FDCAN2_SILENCE_Pin, GPIO_PIN_RESET);

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(RST_SHUT_GPIO_Port, RST_SHUT_Pin, GPIO_PIN_RESET);

    /*Configure GPIO pin : ALERT_Pin */
    GPIO_InitStruct.Pin = ALERT_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(ALERT_GPIO_Port, &GPIO_InitStruct);

    /*Configure GPIO pins : PC14 PC15 TO_PD1__HIZ_Pin TO_PD0__HIZ_Pin */
    GPIO_InitStruct.Pin = GPIO_PIN_14 | GPIO_PIN_15 | TO_PD1__HIZ_Pin | TO_PD0__HIZ_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    /*Configure GPIO pins : PF0 PF1 PF2 */
    GPIO_InitStruct.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

    /*Configure GPIO pin : CLEAR_FAULT_Pin */
    GPIO_InitStruct.Pin = CLEAR_FAULT_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(CLEAR_FAULT_GPIO_Port, &GPIO_InitStruct);

    /*Configure GPIO pins : LED_GREEN_Pin BUCK_EN_Pin FDCAN12_SHUTDOWN_Pin FDCAN1_SILENCE_Pin */
    GPIO_InitStruct.Pin = LED_GREEN_Pin | BUCK_EN_Pin | FDCAN12_SHUTDOWN_Pin | FDCAN1_SILENCE_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /*Configure GPIO pins : TO_PA7__HIZ_Pin PA6 PA7 PA11
                             PA12 TO_PA14_BOOT0__HIZA15_Pin */
    GPIO_InitStruct.Pin = TO_PA7__HIZ_Pin | GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_11 | GPIO_PIN_12 | TO_PA14_BOOT0__HIZA15_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /*Configure GPIO pins : FAULT_LAMP_Pin RJ8P8C_LED_G_Pin RJ8P8C_LED_Y_Pin FDCAN2_SILENCE_Pin */
    GPIO_InitStruct.Pin = FAULT_LAMP_Pin | RJ8P8C_LED_G_Pin | RJ8P8C_LED_Y_Pin | FDCAN2_SILENCE_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /*Configure GPIO pins : PB1 TO_PA14_BOOT0__HIZ_Pin PB14 GND__HIZ_Pin
                             GND__HIZB6_Pin GND__HIZB7_Pin */
    GPIO_InitStruct.Pin = GPIO_PIN_1 | TO_PA14_BOOT0__HIZ_Pin | GPIO_PIN_14 | GND__HIZ_Pin | GND__HIZB6_Pin | GND__HIZB7_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /*Configure GPIO pins : PD0 PD1 TO_PA14_BOOT0__HIZD2_Pin */
    GPIO_InitStruct.Pin = GPIO_PIN_0 | GPIO_PIN_1 | TO_PA14_BOOT0__HIZD2_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

    /*Configure GPIO pin : RST_SHUT_Pin */
    GPIO_InitStruct.Pin = RST_SHUT_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(RST_SHUT_GPIO_Port, &GPIO_InitStruct);

    /*Configure GPIO pins : CFETOFF_Pin DFETOFF_Pin */
    GPIO_InitStruct.Pin = CFETOFF_Pin | DFETOFF_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /* EXTI interrupt init*/
    HAL_NVIC_SetPriority(EXTI4_15_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(EXTI4_15_IRQn);

    /* USER CODE BEGIN MX_GPIO_Init_2 */

    /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
void HAL_GPIO_EXTI_Rising_Callback(uint16_t GPIO_Pin)
{
    switch (GPIO_Pin)
    {
    case ALERT_Pin:
        alarm_cb();
        break;
    }
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim == &htim7)
        sfx_tone_cb();
    else if (htim == htim_interrupt_watchdog && bq769x2_initialized)
    {
        sfx_onetone(932, 50, 10);
        reset_fault_alarm(alarm_raw_now);
        alarm_cb();
    }
    else if (htim == htim_can_disconnect && bq769x2_initialized)
    {
        // sfx_twotone(5, 50, 10);
        reset_fault_alarm(alarm_raw_now);
        HAL_TIM_Base_Stop_IT(htim_can_disconnect);

        // empty stale
        HAL_FDCAN_AbortTxRequest(&hfdcan2, FDCAN_TX_BUFFER0 | FDCAN_TX_BUFFER1 | FDCAN_TX_BUFFER2);
        lwrb_reset(&can_tx_rb);

        // enter shutdown and standby
        HAL_GPIO_WritePin(RJ8P8C_LED_Y_GPIO_Port, RJ8P8C_LED_Y_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(RJ8P8C_LED_G_GPIO_Port, RJ8P8C_LED_G_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(FDCAN12_SHUTDOWN_GPIO_Port, FDCAN12_SHUTDOWN_Pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(FDCAN1_SILENCE_GPIO_Port, FDCAN1_SILENCE_Pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(FDCAN2_SILENCE_GPIO_Port, FDCAN2_SILENCE_Pin, GPIO_PIN_SET);
    }
}

FDCAN_RxHeaderTypeDef RxHeader;
uint8_t RxData[12];

void HAL_FDCAN_RxFifo0Callback(FDCAN_HandleTypeDef *hfdcan, uint32_t RxFifo0ITs)
{
    if ((RxFifo0ITs & FDCAN_IT_RX_FIFO0_NEW_MESSAGE) != RESET)
    {
        /* Retreive Rx messages from RX FIFO0 */
        if (HAL_FDCAN_GetRxMessage(hfdcan, FDCAN_RX_FIFO0, &RxHeader, RxData) != HAL_OK)
        {
            /* Reception Error */
            Error_Handler();
        }
        if (HAL_FDCAN_ActivateNotification(hfdcan, FDCAN_IT_RX_FIFO0_NEW_MESSAGE, 0) != HAL_OK)
        {
            /* Notification Error */
            Error_Handler();
        }
    }
}

void HAL_FDCAN_TxFifoEmptyCallback(FDCAN_HandleTypeDef *hfdcan)
{
    if (hfdcan == &hfdcan2)
    {
        if (can2_move_to_fifo() == 0 && lwrb_get_full(&can_tx_rb) == 0)
        {
            // EMPTY CAN TX BUF
            __HAL_TIM_SET_COUNTER(htim_can_disconnect, 0);
            HAL_TIM_Base_Stop_IT(htim_can_disconnect);

            HAL_GPIO_WritePin(RJ8P8C_LED_Y_GPIO_Port, RJ8P8C_LED_Y_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(RJ8P8C_LED_G_GPIO_Port, RJ8P8C_LED_G_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(FDCAN12_SHUTDOWN_GPIO_Port, FDCAN12_SHUTDOWN_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(FDCAN2_SILENCE_GPIO_Port, FDCAN2_SILENCE_Pin, GPIO_PIN_SET);
        }
    }
}

/**
 * @brief  Configures the FDCAN.
 * @param  None
 * @retval None
 */
static void FDCAN2_classic_config(void)
{
#ifdef CAN_RX
    FDCAN_FilterTypeDef sFilterConfig;

    /* Configure Rx filter */
    sFilterConfig.IdType = FDCAN_STANDARD_ID;
    sFilterConfig.FilterIndex = 0;
    sFilterConfig.FilterType = FDCAN_FILTER_RANGE;
    sFilterConfig.FilterConfig = FDCAN_FILTER_TO_RXFIFO0;
    sFilterConfig.FilterID1 = 0;     // TODO: REMOVE IN PROD TO PREVENT LOOPBACK
    sFilterConfig.FilterID2 = 0x7FF; // TODO:
    if (HAL_FDCAN_ConfigFilter(&hfdcan2, &sFilterConfig) != HAL_OK)
        Error_Handler();
    if (HAL_FDCAN_ActivateNotification(&hfdcan2, FDCAN_IT_RX_FIFO0_NEW_MESSAGE, 0) != HAL_OK)
        Error_Handler();
    HAL_FDCAN_ConfigGlobalFilter(&hfdcan2,
                                 FDCAN_ACCEPT_IN_RX_FIFO0,
                                 FDCAN_ACCEPT_IN_RX_FIFO0,
                                 FDCAN_FILTER_REMOTE,
                                 FDCAN_FILTER_REMOTE);
    if (HAL_FDCAN_ActivateNotification(&hfdcan2, FDCAN_IT_TX_COMPLETE | FDCAN_IT_TX_FIFO_EMPTY, 0) != HAL_OK)
        Error_Handler();

    /* Configure global filter:
       Filter all remote frames with STD and EXT ID
       Reject non matching frames with STD ID and EXT ID */
    // if (HAL_FDCAN_ConfigGlobalFilter(&hfdcan2, FDCAN_REJECT, FDCAN_REJECT, FDCAN_FILTER_REMOTE, FDCAN_FILTER_REMOTE) != HAL_OK)
    //     Error_Handler();
    // if (HAL_FDCAN_ActivateNotification(&hfdcan1, FDCAN_IT_RX_FIFO0_NEW_MESSAGE, 0) != HAL_OK)
    // Error_Handler();
#endif
    if (HAL_FDCAN_Start(&hfdcan2) != HAL_OK)
        Error_Handler();

    HAL_GPIO_WritePin(FDCAN1_SILENCE_GPIO_Port, FDCAN1_SILENCE_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(FDCAN2_SILENCE_GPIO_Port, FDCAN2_SILENCE_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(FDCAN12_SHUTDOWN_GPIO_Port, FDCAN12_SHUTDOWN_Pin, GPIO_PIN_SET);

    usart_send_string("CAN INITIALIZED\r\n");
}

/**
 * \brief           Check if DMA is active and if not try to send data
 * \return          `1` if transfer just started, `0` if on-going or no data to transmit
 */
uint8_t usart_start_tx_dma_transfer(void)
{
#ifdef UART_DBG
    uint32_t primask;
    uint8_t started = 0;

    /*
     * First check if transfer is currently in-active,
     * by examining the value of usart_tx_dma_current_len variable.
     *
     * This variable is set before DMA transfer is started and cleared in DMA TX complete interrupt.
     *
     * It is not necessary to disable the interrupts before checking the variable:
     *
     * When usart_tx_dma_current_len == 0
     *    - This function is called by either application or TX DMA interrupt
     *    - When called from interrupt, it was just reset before the call,
     *         indicating transfer just completed and ready for more
     *    - When called from an application, transfer was previously already in-active
     *         and immediate call from interrupt cannot happen at this moment
     *
     * When usart_tx_dma_current_len != 0
     *    - This function is called only by an application.
     *    - It will never be called from interrupt with usart_tx_dma_current_len != 0 condition
     *
     * Disabling interrupts before checking for next transfer is advised
     * only if multiple operating system threads can access to this function w/o
     * exclusive access protection (mutex) configured,
     * or if application calls this function from multiple interrupts.
     *
     * This example assumes worst use case scenario,
     * hence interrupts are disabled prior every check
     */
    primask = __get_PRIMASK();
    __disable_irq();
    if (usart_tx_dma_current_len == 0 && (usart_tx_dma_current_len = lwrb_get_linear_block_read_length(&usart_tx_rb)) > 0)
    {
        HAL_UART_Transmit_DMA(&hlpuart1, lwrb_get_linear_block_read_address(&usart_tx_rb), usart_tx_dma_current_len);
        started = 1;
    }
    __set_PRIMASK(primask);
    return started;
#else
    return 0;
#endif
}

/**
 * \brief           Send string over USART
 * \param[in]       str: String to send
 */
void usart_send_string(const char *str)
{
#ifdef UART_DBG
    lwrb_write(&usart_tx_rb, str, strlen(str)); /* Write data to transmit buffer */
    usart_start_tx_dma_transfer();
#endif
}

/**
 * @brief Tx Transfer completed callback.
 * @param huart UART handle.
 * @retval None
 */
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{

    if (huart == &hlpuart1)
    {
#ifdef UART_DBG
        lwrb_skip(&usart_tx_rb, usart_tx_dma_current_len);
        usart_tx_dma_current_len = 0;
        usart_start_tx_dma_transfer();
#endif
    }
}

/* USER CODE END 4 */

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void)
{
    /* USER CODE BEGIN Error_Handler_Debug */
    __BKPT();
    /* User can add his own implementation to report the HAL error return state */
    __disable_irq();
    while (1)
    {
    }
    /* USER CODE END Error_Handler_Debug */
}
#ifdef USE_FULL_ASSERT
/**
 * @brief  Reports the name of the source file and the source line number
 *         where the assert_param error has occurred.
 * @param  file: pointer to the source file name
 * @param  line: assert_param error line source number
 * @retval None
 */
void assert_failed(uint8_t *file, uint32_t line)
{
    /* USER CODE BEGIN 6 */
    /* User can add his own implementation to report the file name and line number,
    ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
    /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
