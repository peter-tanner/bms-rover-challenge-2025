#include "bq769x2_config.h"
#include "bq769x2.h"
#include "main.h"
#include "stm32g0xx_hal.h"
#include "stm32g0xx_hal_gpio.h"
#include <stdint.h>

// FIXME: MODIFY BEFORE PRODUCTION
// #define DEBUG_MODE

// #define DISABLE_PF
// #define DISABLE_SF
// #define DISABLE_CELL_BALANCING
#define DISABLE_HWDF
#define AMBIENT_TEMPERATURE 35

void bq769x2_init(void)
{
    // Configures all parameters in device RAM

    // Enter CONFIGUPDATE mode (Subcommand 0x0090) - It is required to be in CONFIG_UPDATE mode to program the device RAM settings
    // See TRM for full description of CONFIG_UPDATE mode
    // CMD_SUBCMD_RESET(); // THIS CAUSES POWER OUTAGE

    HAL_Delay(3000);
    HAL_GPIO_WritePin(RST_SHUT_GPIO_Port, RST_SHUT_Pin, GPIO_PIN_SET);
    HAL_Delay(100);
    HAL_GPIO_WritePin(RST_SHUT_GPIO_Port, RST_SHUT_Pin, GPIO_PIN_RESET);
    bq769x2_delay_us(60000);
    CMD_SUBCMD_RESET_PASSQ();
    CMD_SUBCMD_SET_CFGUPDATE();

    // After entering CONFIG_UPDATE mode, RAM registers can be programmed. When programming RAM, checksum and length must also be
    // programmed for the change to take effect. All of the RAM registers are described in detail in the BQ769x2 TRM.
    // An easier way to find the descriptions is in the BQStudio Data Memory screen. When you move the mouse over the register name,
    // a full description of the register and the bits will pop up on the screen.

    /*
     *
     * COMMUNICATIONS
     *
     */
    SET_CommType_dimensionless(CommType_I2C_FAST_CRC);
    HAL_I2C_DeInit(&hi2c1);
    hi2c1.Init.Timing = 0x00300617; // FAST MODE 400kHz
    HAL_I2C_Init(&hi2c1);
    SET_I2CAddress_DEFAULT();   // 0x10
    SET_CommIdleTime_DEFAULT(); // NO IDLE TIME EXTENSION

    /*
     *
     * POWER / REGULATOR OPTIONS
     *
     */
    // 'Power Config' - 0x9234 = 0x2D80
    // Setting the DSLP_LDO bit allows the LDOs to remain active when the device goes into Deep Sleep mode
    // Set wake speed bits to 00 for best performance
    SET_PowerConfig_Raw(PowerConfig_DPSLP_OT | PowerConfig_DPSLP_PD | PowerConfig_OTSD | // PowerConfig_DPSLP_LFO | PowerConfig_DPSLP_LDO |
                        PowerConfig_SLEEP | PowerConfig_WK_SPD_24ms |
                        PowerConfig_FASTADC_3ms0 | PowerConfig_LOOP_SLOW_504ms |
                        PowerConfig_CB_LOOP_SLOW_HALF_SPD);
    // NOTE: half speed while cell balancing to increase average cell balancing current

    // SLEEP mode options
    // PowerConfig_WK_SPD_24ms -> minimum wake comparator current: i = 10 * 0.010mV / 1mR = 0.100A
    SET_WakeComparatorCurrent_mA(500); // DEFAULT
    SET_SleepCurrent_mA(20);           // DEFAULT
    SET_VoltageTime_s(4 * (5) + 1);
    SET_SleepHysteresisTime_DEFAULT();
    SET_SleepChargerVoltageThreshold_mV(52000); // 13cell * 4V/cell = 52V. (5% of capacity unused at 4V co).
    SET_SleepChargerPACKTOSDelta_DEFAULT();
    // TODO: maybe only works with disabled SLEEPCHG -> actual voltage buildup over fets.
    // (0.75mR/4 * 2) * 0.25A =

    // 'REG12 Config' - Enable REG1 with 3.3V output
    SET_REG12Config_Raw(REG12Config_REG1_EN | REG12Config_REG1V_3V3);

    // 'REG0 Config' - set REG0_EN bit to enable pre-regulator
    SET_REG0Config_Raw(REG0Config_REG0_EN);

    /*
     *
     * ENABLED PROTECTIONS
     *
     */

    SET_DefaultAlarmMask_Raw(DefaultAlarmMask_SSBC |
                             DefaultAlarmMask_SSA |
                             DefaultAlarmMask_PF |
                             DefaultAlarmMask_MSK_SFALERT |
                             DefaultAlarmMask_MSK_PFALERT |
                             DefaultAlarmMask_INITSTART |
                             DefaultAlarmMask_INITCOMP |
                             DefaultAlarmMask_FULLSCAN |
                             DefaultAlarmMask_XCHG |
                             DefaultAlarmMask_XDSG |
                             DefaultAlarmMask_SHUTV |
                             DefaultAlarmMask_FUSE |
                             //  DefaultAlarmMask_CB |
                             DefaultAlarmMask_WAKE);

#ifdef DISABLE_SF
    SET_EnabledProtectionsA_Raw(0x00);
    SET_EnabledProtectionsB_Raw(0x00);
    SET_EnabledProtectionsC_Raw(0x00);
#endif

    /*
     * PROTECTIONS A -- CURRENT & VOLTAGE : TESTED [❌]
     * ------------------------
     * Short Circuit in Discharge Protection [❌]
     * Overcurrent in Discharge 1st Tier Protection [❌]
     * Overcurrent in Charge Protection [❌]
     * Cell Overvoltage Protection [❌]
     * Cell Undervoltage Protection [✅]
     */
#ifndef DISABLE_SF
    SET_EnabledProtectionsA_Raw(EnabledProtectionsA_SCD |
                                EnabledProtectionsA_OCD1 |
                                EnabledProtectionsA_OCC |
                                EnabledProtectionsA_COV |
                                EnabledProtectionsA_CUV);
#endif

    // TODO: COORDINATE WITH MOTOR FUSES
    // SCD SHORT-CIRCUIT IN DISCHARGE
    // CALCS:
    // - Theoretical SC current with no load: Vbat / (Rbat*cells + Rsense + Rfet)
    //                                      = 54.6 / (4e-3*13 + 1e-3 + 0.75e-3*2/4)
    //                                  Isc = 1.022 kA.
    // - Batteries: Assume pulse is short enough? A commercial BMS online uses 250us for SCD.
    //   No data for extremely short and high current pulses in battery datasheet.
    //   https://www.nkon.nl/en/amfile/file/download/file/801/product/5883/
    // - PCB track: at min width (550 mils), using saturn PCB calculator for fusing current (Onderdonk's equation)
    //   at 500us, fusing current of 12.5kA > 1.015kA.
    // - RESISTOR : https://www.eaton.com.cn/content/dam/eaton/products/electronic-components/resources/data-sheet/eaton-cssa-smd-automotive-current-sense-resistor-shunt-data-sheet-elx1372-en.pdf
    //   From Maximum pulse power curve @ 500us, P=~2000W -> Imax=1400A > Isc.
    // - MOSFETS:https://www.infineon.com/assets/row/public/documents/24/49/infineon-ipt007n06n-datasheet-en.pdf
    //   In short-circuit, max Vds=60V. Tj=175*C
    //   From diagram 4 consider a ZthJC of 1e-1 K/W.
    //   0.75e-3*2/4 combined resistance of b2b mosfets.
    //   Temperature after short is : 1015**2 * 0.75e-3*2/4 * (1e-1) + 25 = 63.6*C.
    //   From diagram 4 a ZthJC of 1e-1 occurs forsingle pulse at 1ms.
    //   CONCLUSION: Max. SCD delay of 250us works.
    //
    // THRESHOLD: 500A (1mV/A) : Board designed to 90A maximum
    // DELAY: 30us : TODO: MOSFET calc
    // RECOVERY TIME: 30s (Use recovery time instead of latch since in competition
    // there is no opportunity to disconnect and reconnect the load).
    SET_SCDThreshold_dimensionless(SCDThreshold_500mV);
    SET_SCDDelay_us(250);
    SET_SCDRecoveryTime_s(30);

    // Above 140A and below SCC (500A), restrict to protect battery.
    // Board, MOSFETs and resistor safe.
    // - PCB track: using fusing current calculator for 500ms maximum,
    //   fusing current of 400A
    // - Resistor: at 2.5kW, safe for ~1s
    // - MOSFETS: at 500A and ZthJC=4e-1, temp rise of 37.5*C
    // Another commercial BMS uses 9ms above 150A.
    // CONCLUSION: 140ms.
    SET_OCD1Threshold_mV(140);
    SET_OCD1Delay_ms(9);

    // TODO: COORDINATE WITH GENERAL SPIKES FROM PC ETC.
    // OCD1 OVERCURRENT IN DISCHARGE 1ST TIER
    // CALCS:
    // - Battery: Pulsed current of up to 140A for 3s. 90A w/ 75*C cut-off
    // - PCB track: designed for 90A continuous.
    // - RESISTOR: At 140A, P=20W. Refer to datasheet MPP figure -> pulse width inf.
    // - MOSFETS: At 140A, can operate semi- continuously at this current
    //   with ~1s 140A single pulse, temp rise is negligble -> 16.2*C.
    // CONCLUSION: SELECT 90A with 3s cutoff.
    // NOTE: DAConfiguration_USER_AMPS_cA_10mA
    SET_OCD3Threshold_userA(-9000);
    SET_OCD3Delay_s(3);

    // OCC OVERCURRENT IN CHARGE
    // - Battery: from [datasheet](https://www.nkon.nl/en/amfile/file/download/file/801/product/5883/)
    //   Max charging current = 15A.
    SET_OCCThreshold_mV(15);
    SET_OCCDelay_DEFAULT();        // 20ms
    SET_OCCPACKTOSDelta_DEFAULT(); // auto-recovery on disconnect
    SET_OCCRecoveryThreshold_DEFAULT();

    // COV CELL OVERVOLTAGE
    // https://www.nkon.nl/en/amfile/file/download/file/801/product/5883/
    // > Charging voltage must be set 4.2V/cell. Concerning charge voltage tolerance of charger,
    // > charging voltage must be set lower than 4.25V/cell. Even if the charge could be out of order,
    // > charge voltage of charger should not be more than 4.25V/cell to avoid over-charging.
    SET_COVThreshold_mV(4240);
    SET_COVDelay_ms(250);
    SET_CUVRecoveryHysteresis_mV(100);

    // CUV CELL UNDERVOLTAGE
    // https://www.nkon.nl/en/amfile/file/download/file/801/product/5883/
    // > The cell should not be over-discharged below 2.0V.
    // > Within a voltage range of 2.5V/cell, over-discharge protection should work. Then discharge
    // > current shall be shut down and consumption current is below 1μA.
    SET_CUVThreshold_mV(2500);
    SET_CUVDelay_DEFAULT();
    SET_CUVRecoveryHysteresis_DEFAULT();

    /*
     * PROTECTIONS B -- OVERTEMPERATURE
     * --------------------------------
     * FET Overtemperature [❌]
     * Overtemperature Internal [❌]
     * Overtemperature in Discharge [❌]
     * Overtemperature in Charge [❌]
     * Undertemperature Internal [TBD]
     * Undertemperature in Charge [TBD]
     * Undertemperature in Discharge [TBD]
     * IGNORE UNDERTEMPERATURE PROTECTIONS (TODO:)
     */
#ifndef DISABLE_SF
    SET_EnabledProtectionsB_Raw(EnabledProtectionsB_OTF |
                                EnabledProtectionsB_OTINT |
                                EnabledProtectionsB_OTD |
                                EnabledProtectionsB_OTC |
                                EnabledProtectionsB_UTINT |
                                EnabledProtectionsB_UTD |
                                EnabledProtectionsB_UTC);
#endif

    // OTF OVERTEMPERATURE FET
    // IPT007N06N
    // T_J(MAX): 175degC
    // in operation, at 90A: 55.08degC/FET
    // Select 100degC
    SET_OTFThreshold_degC(100);
    SET_OTFRecovery_degC(100 - AMBIENT_TEMPERATURE); // Allow for 35degC internal temperature (Extreme)
    SET_OTFDelay_DEFAULT();

    // OTINT OVERTEMPERATURE INTERNAL
    SET_OTINTThreshold_DEFAULT();
    SET_OTINTRecovery_degC(85 - AMBIENT_TEMPERATURE);
    SET_OTINTDelay_DEFAULT();
    // UTINT UNDERTEMPERATURE INTERNAL
    SET_UTINTThreshold_DEFAULT();
    SET_UTINTDelay_DEFAULT();
    SET_UTINTRecovery_DEFAULT();

    // OTD OVERTEMPERATURE IN DISCHARGE (BATTERY CELL TEMP)
    // https://www.nkon.nl/en/amfile/file/download/file/801/product/5883/
    // Discharge:-20 to 80℃
    // (Recommended re-discharge release < 60℃)
    SET_OTDThreshold_degC(60); // TODO: DEFAULT - MORE AGGRESSIVE PROFILE FOR COMP?
    SET_OTDRecovery_degC(60 - AMBIENT_TEMPERATURE);
    SET_OTDDelay_DEFAULT();
    // UTD UNDERTEMPERATURE IN DISCHARGE
    SET_UTDThreshold_degC(-20);
    SET_UTDDelay_DEFAULT();
    SET_UTDRecovery_degC(-15);

    // OTC OVERTEMPERATURE IN CHARGE (BATTERY CELL TEMP)
    // https://www.nkon.nl/en/amfile/file/download/file/801/product/5883/
    // Charge : 0 to 60℃
    // (Recommended recharge release < 45℃)
    SET_OTCThreshold_degC(45);
    SET_OTCRecovery_degC(45 - 25); // COOLER IN CHARGING
    SET_OTCDelay_DEFAULT();
    // UTC UNDERTEMPERATURE IN CHARGE
    SET_UTCThreshold_degC(0);
    SET_UTCRecovery_degC(5);
    SET_UTCDelay_DEFAULT();

    /*
     * PROTECTIONS C -- OVERCURRENT LATCH & TIMEOUTS
     * ---------------------------------------------
     * Host Watchdog Fault [✅]
     * Overcurrent in Discharge 3 [❌]
     * TODO: PTO needed to automatically stop predischarge????
     */
#ifndef DISABLE_SF
#ifdef DISABLE_HWDF
#define PC_HWDF_VALUE 0x00
#else
#define PC_HWDF_VALUE EnabledProtectionsC_HWDF
#endif

    SET_EnabledProtectionsC_Raw( //   EnabledProtectionsC_OCDL |
                                 //   EnabledProtectionsC_COVL |
                                 //   EnabledProtectionsC_SCDL |
                                 //   EnabledProtectionsC_PTO |
        EnabledProtectionsC_OCD3 | PC_HWDF_VALUE);
#endif
    // HWD HOST WATCHDOG DELAY
    SET_HWDRegulatorOptions_Raw(HWDRegulatorOptions_TOGGLE_TIME_5s | HWDRegulatorOptions_TOGGLE_OPT_REG12_OFFON);
    SET_HWDDelay_s(60);

    /*
     * PERMANENT FAILURE PROTECTIONS
     * -----------------------------
     * SCDL does not recover based on charge current.
     * OCDL does not recover based on charge current.
     * Permanent Failure does not cause the device to blow the fuse.
     * Device does not automatically enter DEEPSLEEP mode when a Permanent Failure occurs. (NEEDED FOR COMMS TO THE HOST COMPUTER BEFORE IT KILLS POWER TO ITSELF)
     * Permanent Failure does not cause the device to turn the regulators off. (NEEDED FOR COMMS TO THE HOST COMPUTER BEFORE IT KILLS POWER TO ITSELF)
     * Permanent Failure causes the device to turn the FETs off.
     */
#ifndef DISABLE_PF
    SET_ProtectionConfiguration_Raw(ProtectionConfiguration_PF_FETS);
    SET_PFAlertMaskA_Raw(PFAlertMaskA_DEFAULTRAW | PFAlertMaskA_SUV);
    SET_PFAlertMaskB_DEFAULT();
    SET_PFAlertMaskC_DEFAULT();
    SET_PFAlertMaskD_DEFAULT();
    SET_EnabledPFA_Raw(EnabledPFA_DEFAULTRAW | EnabledPFA_SUV);
    SET_EnabledPFB_DEFAULT();
    SET_EnabledPFC_DEFAULT();
    SET_EnabledPFD_DEFAULT();
    SET_ShutdownStackVoltage_DEFAULT(); // FIXME: SET TO REASONABLE VOLTAGE
#else
    SET_EnabledPFA_Raw(0x00);
    SET_EnabledPFB_Raw(0x00);
    SET_EnabledPFC_Raw(0x00);
    SET_EnabledPFD_Raw(0x00);
#endif

    // SUV SAFETY CELL UNDERVOLTAGE
    // https://www.nkon.nl/en/amfile/file/download/file/801/product/5883/
    // > The cell should not be over-discharged below 2.0V.
    // > Within a voltage range of 2.5V/cell, over-discharge protection should work. Then discharge
    // > current shall be shut down and consumption current is below 1μA.
    SET_SUVThreshold_mV(2200);
    SET_SUVDelay_DEFAULT();
    // TODO: DISABLE ITSELF AFTER BEEP AND 30 SECOND DELAY

    /*
     * LOAD DETECT PROTECTION
     * ----------------------
     * CURRENTLY DISABLED SINCE LATCHED PROTECTIONS NOT USED.
     */
    // SET_LoadDetectActiveTime_DEFAULT();
    // SET_LoadDetectTimeout_DEFAULT();
    // SET_LoadDetectRetryDelay_DEFAULT();

    /*
     *
     * FET CONTROL / CHARGE PUMP
     *
     */
    // MOSFETS
    // TODO: CHECK IF PDSG IS REQUIRED.
    // SFET: SERIES (BACK TO BACK) MOSFETS
    //
    SET_FETOptions_Raw(FETOptions_FET_CTRL_EN |
                       // FETOptions_PDSG_EN |
                       //    FETOptions_HOST_FET_EN |
                       FETOptions_SFET |
                       FETOptions_SLEEPCHG); // TODO: FOR NOW ENABLE CHARGE IN SLEEP TO PREVENT JUMPSCARES ABOUT BMS APPEARING SEEMINGLY BROKEN...

    SET_ChgPumpControl_Raw(ChgPumpControl_CPEN); // 11V overdrive, enable charge pump
    // https://www.infineon.com/assets/row/public/documents/24/49/infineon-ipt007n06n-datasheet-en.pdf
    // Vf=1V MAX. 1A->1W->40K RISE
    // DEFAULT IS 50mA, x4 SINCE FOUR MORE MOSFETS
    SET_BodyDiodeThreshold_mA(200);

    // > When the cell voltage is below 2.0V, cell should be pre-charged lower than 0.1C current.
    // > Only standard charging method can be switched after voltage exceed 2.0V. If the cell
    // > voltage unable to reach in specified period/time, stop charging immediately.
    SET_PrechargeStartVoltage_mV(2100);
    SET_PrechargeStopVoltage_mV(2300);

    //   MAXIMUM PREDISCHARGE DUE TO HIGH CAPACITANCE LOAD.
    SET_PredischargeStopDelta_mV(250);
    SET_PredischargeTimeout_ms(2550);

    /*
     *
     * CURRENT SENSE & SENSE-RESISTOR CAL
     * (also see PowerConfig for ADC speed settings)
     *
     */
#define RSENSE_mOhm 1
    SET_CCGain_float(CCGain_DEFAULTRAW / RSENSE_mOhm);
#if RSENSE_mOhm > 1
    SET_CapacityGain_float(CapacityGain_MIN * CCGain_DEFAULTRAW / RSENSE_mOhm);
#else
    SET_CapacityGain_DEFAULT();
#endif

    SET_CoulombCounterOffsetSamples_DEFAULT(); // 64 slots DEFAULT
    SET_BoardOffset_DEFAULT();                 //
    SET_CoulombCounterDeadband_DEFAULT();      // > used to filter out signals below the expected noise floor. THIS NORMALLY SHOULD NOT BE CHANGED.
    SET_CC3Samples_DEFAULT();                  // AVERAGE OVER CC2 SAMPLES. (DEFAULT: 80 SLOTS)
    // TODO: is 1mA worth sacrificing for 32.767 A maximum current before saturating?
    SET_DAConfiguration_Raw(DAConfiguration_USER_AMPS_cA_10mA | DAConfiguration_USER_VOLTS_cV_10mV); // 327.67 A maximum, 327.67 V maximum (*1cV)

    /*
     *
     * CELL BALANCING
     *
     */
#ifdef DISABLE_CELL_BALANCING
    SET_BalancingConfiguration_Raw(0x00);
    bq769x2_subcommands(CB_ACTIVE_CELLS, 0x0000, W2);
#else
    // DISABLE BALANCING ON 16,15,14
    SET_VCellMode_Raw(VCellMode_ALL ^ VCellMode_7_EN ^ VCellMode_8_EN ^ VCellMode_15_EN);

    // TODO: CELL INTERCONNECT RESISTANCES (MEASURE NICKEL STRIP)
    SET_Cell1Interconnect_mOhm(0);
    SET_Cell2Interconnect_mOhm(0);
    SET_Cell3Interconnect_mOhm(0);
    SET_Cell4Interconnect_mOhm(0);
    SET_Cell5Interconnect_mOhm(0);
    SET_Cell6Interconnect_mOhm(0);
    SET_Cell7Interconnect_mOhm(0);
    SET_Cell8Interconnect_mOhm(0);
    SET_Cell9Interconnect_mOhm(0);
    SET_Cell10Interconnect_mOhm(0);
    SET_Cell11Interconnect_mOhm(0);
    SET_Cell12Interconnect_mOhm(0);
    SET_Cell13Interconnect_mOhm(0); // TODO:
    SET_Cell14Interconnect_mOhm(0); // TODO:
    SET_Cell15Interconnect_mOhm(0); // TODO:
    SET_Cell16Interconnect_mOhm(0); // TODO:

    // Set up Cell Balancing Configuration - 0x9335 = 0x03 - Automated balancing while in Relax or Charge modes
    // Also see "Cell Balancing with BQ769x2 Battery Monitors" document on ti.com
    SET_BalancingConfiguration_Raw(BalancingConfiguration_CB_CHG | BalancingConfiguration_CB_RLX |
                                   BalancingConfiguration_CB_NOSLEEP | BalancingConfiguration_CB_SLEEP | // When [CB_NOSLEEP] bit is set, CB_SLEEP should also be set.
                                   BalancingConfiguration_CB_NO_CMD);                                    // NO HOST BALANCING
    // BalancingConfiguration_CB_SLEEP |  // BALANCE IN SLEEP WITHOUT WAKING UP.

    SET_MinCellTemp_degC(CELL_MIN_degC);
    SET_MaxCellTemp_degC(CELL_MAX_degC);
    SET_MaxInternalTemp_DEFAULT(); // SELECT 70degC, MUST BE LESS THAN OTINTThreshold (85degC)
    SET_CellBalanceInterval_DEFAULT();
    SET_CellBalanceMinCellVCharge_mV(3900); // PASSIVE BALANCING IS LOSSY, ONLY START BALANCING ABOVE THIS THRESHOLD.
    SET_CellBalanceMinCellVRelax_mV(4100);  // PASSIVE BALANCING IS LOSSY, ONLY START BALANCING ABOVE THIS THRESHOLD.
    SET_CellBalanceMinDeltaCharge_DEFAULT();
    SET_CellBalanceStopDeltaCharge_DEFAULT();
    SET_CellBalanceStopDeltaRelax_DEFAULT();
    SET_CellBalanceMinDeltaRelax_DEFAULT();
    SET_CellBalanceMaxCells_Num(4);
    // R_DS(ON) ~= 25R, 2 R_SER = 2 20R
    // PASSIVE BALANCING AT 4.2V -> BALANCE CURRENT 65mA
    // -> 0.1W DISSIPATION/PARALLEL FET
    // R_JA = 66 °C/W, FOR AMBIENT TEMPERATURE OF 35 AND LIMIT OF 70: P_D(MAX) = 0.53W
    // -> 5 PARALLEL MAXIMUM
#endif

    /*
     *
     * PIN CONFIG
     *
     */
    SET_CFETOFFPinConfig_Raw(CFETOFFPinConfig_FXN_SPI_CS_UNUSED);
    SET_DFETOFFPinConfig_Raw(DFETOFFPinConfig_FXN_UNUSED);

    SET_ALERTPinConfig_Raw(ALERTPinConfig_FXN_ALERT | PIN_DIGITAL_1_PUSH_PULL | PIN_DIGITAL_5_ACTIVE_HIGH | PIN_DIGITAL_3_DRIVE_HIGH_REG1);

    SET_TS3Config_Raw(TS3Config_FXN_ADC_THERMISTOR | PIN_ANALOG_PU_18K_NTC10K | PIN_ANALOG_POLY_PU18K_NTC10K | PIN_ANALOG_TYPE_FET_PROT); // MOSFET THERMISTOR
    // SET_TS1Config_Raw(TS1Config_FXN_ADC_THERMISTOR | PIN_ANALOG_PU_18K_NTC10K | PIN_ANALOG_POLY_PU18K_NTC10K | PIN_ANALOG_TYPE_CELL_PROT); // THERMISTOR IN PACK // TODO:
    SET_TS1Config_Raw(TS1Config_FXN_UNUSED);
    SET_TS2Config_Raw(TS2Config_FXN_UNUSED);

    SET_HDQPinConfig_Raw(HDQPinConfig_FXN_UNUSED1);
    SET_DCHGPinConfig_Raw(DCHGPinConfig_FXN_UNUSED);
    SET_DDSGPinConfig_Raw(DDSGPinConfig_FXN_UNUSED);

    /*
     *
     * OTHER DEVICE OPTS
     *
     */

    // ENABLE PF AND FULLY AUTONOMOUS FET CONTROL
    SET_MfgStatusInit_Raw(MfgStatusInit_PF_EN | MfgStatusInit_FET_EN);

    // Exit CONFIGUPDATE mode  - Subcommand 0x0092
    CMD_SUBCMD_EXIT_CFGUPDATE();
}