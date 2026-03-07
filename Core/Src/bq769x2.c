/* --COPYRIGHT--,BSD_EX
 * Copyright (c) 2021, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * --/COPYRIGHT--*/

#include "bq769x2.h"
#include "bq769x2_st.h"
#include <stddef.h>
#include <stdint.h>
#include <string.h>

#define SUBCOMMAND_BUF_SIZE 32

#define RXBUF_TO_32(off) ((BQ769X2_RX_32Byte[off + 3] << 24) | (BQ769X2_RX_32Byte[off + 2] << 16) | (BQ769X2_RX_32Byte[off + 1] << 8) | (BQ769X2_RX_32Byte[off + 0] << 0))
#define RXBUF_TO_16(off) ((BQ769X2_RX_32Byte[off + 1] << 8) | (BQ769X2_RX_32Byte[off + 0] << 0))

static uint8_t BQ769X2_RX_32Byte[32];

// uint8_t RX_data[2] = {0x00, 0x00}; // used in several functions to store data read from BQ769x2

void bq769x2_memcpy(void *dest, void *source, size_t size)
{
    size_t i;
    for (i = 0; i < size; i++)
        ((uint8_t *)dest)[i] = ((uint8_t *)source)[i];
}

uint8_t bq769x2_checksum(uint8_t *ptr, size_t len)
// Calculates the checksum when writing to a RAM register. The checksum is the inverse of the sum of the bytes.
{
    size_t i;
    uint8_t checksum = 0;

    for (i = 0; i < len; i++)
        checksum += ptr[i];

    checksum = 0xFF & ~checksum;

    return checksum;
}

uint8_t bq769x2_crc8(uint8_t *ptr, size_t len)
// Calculates CRC8 for passed bytes. Used in i2c read and write functions
{
    static const uint16_t CRC_POLYNOMIAL = (1 << 8) | (1 << 2) | (1 << 1) | 1;
    uint8_t bitmask;
    uint8_t crc = 0;
    while (len-- > 0)
    {
        for (bitmask = 0b10000000; bitmask > 0; bitmask >>= 1)
        {
            if (crc & 0x80)
                crc = (crc << 1) ^ CRC_POLYNOMIAL;
            else
                crc = (crc << 1);

            if (*ptr & bitmask)
                crc ^= CRC_POLYNOMIAL;
        }
        ptr++;
    }
    return crc;
}

uint8_t bq769x2_iic_write_reg(uint8_t reg_addr, uint8_t *reg_data, uint8_t count)
{
    static uint8_t TX_Buffer[SUBCOMMAND_BUF_SIZE * 2];
#if CRC_Mode
    {
        uint8_t crc_count = count * 2;
        uint8_t crc1stByteBuffer[3] = {0x10, reg_addr, reg_data[0]};
        size_t i, j = 0;
        // uint8_t temp_crc_buffer[3];

        TX_Buffer[j++] = reg_data[0];
        TX_Buffer[j++] = bq769x2_crc8(crc1stByteBuffer, 3);

        for (i = 1; i < count; i++)
        {
            TX_Buffer[j++] = reg_data[i];
            // j++;
            // temp_crc_buffer[0] = reg_data[i];
            // TX_Buffer[j] = bq769x2_crc8(temp_crc_buffer, 1);
            TX_Buffer[j++] = bq769x2_crc8(&reg_data[i], 1);
            // j++;
        }
        if (bq769x2_iic_write(DEV_ADDR, reg_addr, 1, TX_Buffer, crc_count, 1000))
            bq769x2_error_handler();
    }
#else
    if (bq769x2_iic_write(DEV_ADDR, reg_addr, 1, reg_data, count, 1000))
        bq769x2_error_handler();
#endif
}

size_t bq769x2_iic_read_reg(uint8_t reg_addr, uint8_t *reg_data, size_t count)
{
    static uint8_t data_rx_buf[SUBCOMMAND_BUF_SIZE];
    static uint8_t data_crc_rx_buf[SUBCOMMAND_BUF_SIZE * 2];

    size_t RX_CRC_Fail = 0; // reset to 0. If in CRC Mode and CRC fails, this will be incremented.
#if CRC_Mode
    {
        uint8_t crc_count = count * 2;
        size_t i, j;
        uint8_t CRCc = 0;
        uint8_t temp_crc_buffer[3];

        if (bq769x2_iic_read(DEV_ADDR, reg_addr, 1, data_crc_rx_buf, crc_count, 1000))
            bq769x2_error_handler();
        uint8_t crc1stByteBuffer[4] = {0x10, reg_addr, 0x11, data_crc_rx_buf[0]};
        CRCc = bq769x2_crc8(crc1stByteBuffer, 4);
        RX_CRC_Fail += (CRCc != data_crc_rx_buf[1]);
        data_rx_buf[0] = data_crc_rx_buf[0];

        j = 2;
        for (i = 1; i < count; i++)
        {
            data_rx_buf[i] = data_crc_rx_buf[j];
            temp_crc_buffer[0] = data_crc_rx_buf[j];
            j++;
            CRCc = bq769x2_crc8(temp_crc_buffer, 1);
            // CRCc = bq769x2_crc8(&RX_Buffer[i], 1);
            RX_CRC_Fail += (CRCc != data_crc_rx_buf[j]);
            j++;
        }
        bq769x2_memcpy(reg_data, data_rx_buf, count);
    }
#else
    if (bq769x2_iic_read(DEV_ADDR, reg_addr, 1, reg_data, count, 1000))
        bq769x2_error_handler();
#endif
    return RX_CRC_Fail;
}

void bq769x2_set_register(uint16_t reg_addr, uint32_t reg_data, uint8_t datalen)
{
    uint8_t TX_Buffer[2] = {0x00, 0x00};
    uint8_t TX_RegData[6] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

    // TX_RegData in little endian format
    TX_RegData[0] = reg_addr & 0xff;
    TX_RegData[1] = (reg_addr >> 8) & 0xff;
    TX_RegData[2] = reg_data & 0xff; // 1st byte of data

    switch (datalen)
    {
    case 1: // 1 byte datalength
        bq769x2_iic_write_reg(0x3E, TX_RegData, 3);
        bq769x2_delay_us(2000);
        TX_Buffer[0] = bq769x2_checksum(TX_RegData, 3);
        TX_Buffer[1] = 0x05;                       // combined length of register address and data
        bq769x2_iic_write_reg(0x60, TX_Buffer, 2); // Write the checksum and length
        bq769x2_delay_us(2000);
        break;
    case 2: // 2 byte datalength
        TX_RegData[3] = (reg_data >> 8) & 0xff;
        bq769x2_iic_write_reg(0x3E, TX_RegData, 4);
        bq769x2_delay_us(2000);
        TX_Buffer[0] = bq769x2_checksum(TX_RegData, 4);
        TX_Buffer[1] = 0x06;                       // combined length of register address and data
        bq769x2_iic_write_reg(0x60, TX_Buffer, 2); // Write the checksum and length
        bq769x2_delay_us(2000);
        break;
    case 4: // 4 byte datalength, Only used for CCGain and Capacity Gain
        TX_RegData[3] = (reg_data >> 8) & 0xff;
        TX_RegData[4] = (reg_data >> 16) & 0xff;
        TX_RegData[5] = (reg_data >> 24) & 0xff;
        bq769x2_iic_write_reg(0x3E, TX_RegData, 6);
        bq769x2_delay_us(2000);
        TX_Buffer[0] = bq769x2_checksum(TX_RegData, 6);
        TX_Buffer[1] = 0x08;                       // combined length of register address and data
        bq769x2_iic_write_reg(0x60, TX_Buffer, 2); // Write the checksum and length
        bq769x2_delay_us(2000);
        break;
    }
}

void bq769x2_command_subcommands(uint16_t command, uint16_t command_delay) // For Command only Subcommands
// See the TRM or the BQ76952 header file for a full list of Command-only subcommands
{
    // Read 0x3E and 0x3F. If this returns 0xFF, this indicates the subcommand has not completed operation
    // yet. When the subcommand has completed, the readback will return what was originally written. Continue
    // reading 0x3E and 0x3F until it returns what was written originally. Note: this response only applies to
    // subcommands that return data to be read back.
    // => NO NEED TO CONFIRM

    // FIXED: For DEEPSLEEP/SHUTDOWN subcommand you will need to call this function twice consecutively
    uint8_t repeat = command == 0x000E ||
                     command == 0x000F;

    uint8_t TX_Reg[2] = {0x00, 0x00};
    do
    {
        // TX_Reg in little endian format
        TX_Reg[0] = command & 0xff;
        TX_Reg[1] = (command >> 8) & 0xff;
        bq769x2_iic_write_reg(0x3E, TX_Reg, 2);
        bq769x2_delay_us(command_delay);
    } while (repeat--);
}

void _bq769x2_subcommands(uint16_t command, uint16_t delay, uint16_t data, uint8_t type)
// See the TRM or the BQ76952 header file for a full list of Subcommands
{
    // security keys and Manu_data writes dont work with this function (reading these commands works)
    // max readback size is 32 bytes i.e. DASTATUS, CUV/COV snapshot
    uint8_t TX_Reg[4] = {0x00, 0x00, 0x00, 0x00};
    uint8_t TX_Buffer[2] = {0x00, 0x00};
    uint8_t retries = 16;

    // TX_Reg in little endian format
    TX_Reg[0] = command & 0xff;
    TX_Reg[1] = (command >> 8) & 0xff;

    switch (type)
    {
    case R:
        bq769x2_iic_write_reg(0x3E, TX_Reg, 2);
        // Read 0x3E and 0x3F. If this returns 0xFF, this indicates the subcommand has not completed operation
        // yet. When the subcommand has completed, the readback will return what was originally written. Continue
        // reading 0x3E and 0x3F until it returns what was written originally. Note: this response only applies to
        // subcommands that return data to be read back.
        do
        {
            bq769x2_delay_us(delay);
            memset(TX_Reg, 0x00, 2);
            bq769x2_iic_read_reg(0x3E, TX_Reg, 2);
        } while (!(TX_Reg[0] == (command & 0xff) && TX_Reg[1] == ((command >> 8) & 0xff)) && retries--);
        bq769x2_iic_read_reg(0x40, BQ769X2_RX_32Byte, 32); // RX_32Byte is a global variable
        break;
    case W:
        // FET_Control, REG12_Control
        TX_Reg[2] = data & 0xff;
        bq769x2_iic_write_reg(0x3E, TX_Reg, 3);
        bq769x2_delay_us(delay);
        TX_Buffer[0] = bq769x2_checksum(TX_Reg, 3);
        TX_Buffer[1] = 0x05; // combined length of registers address and data
        bq769x2_iic_write_reg(0x60, TX_Buffer, 2);
        bq769x2_delay_us(delay);
        break;
    case W2:
        // CB_Active_Cells, CB_SET_LVL
        TX_Reg[2] = data & 0xff;
        TX_Reg[3] = (data >> 8) & 0xff;
        bq769x2_iic_write_reg(0x3E, TX_Reg, 4);
        bq769x2_delay_us(delay);
        TX_Buffer[0] = bq769x2_checksum(TX_Reg, 4);
        TX_Buffer[1] = 0x06; // combined length of registers address and data
        bq769x2_iic_write_reg(0x60, TX_Buffer, 2);
        bq769x2_delay_us(delay);
        break;
    }
#undef REG_EQ_COMMAND
}

// See the TRM or the BQ76952 header file for a full list of Direct Commands
// void bq769x2_direct_commands(uint8_t command, uint16_t data, uint8_t type)
// { // type: R = read, W = write
//     uint8_t TX_data[2] = {0x00, 0x00};

//     // little endian format
//     TX_data[0] = data & 0xff;
//     TX_data[1] = (data >> 8) & 0xff;

//     switch (type)
//     {
//     case R:
//         bq769x2_iic_read_reg(command, RX_data, 2); // RX_data is a global variable
//         bq769x2_delay_us(2000);
//         break;
//     case W:
//         // Control_status, alarm_status, alarm_enable all 2 bytes long
//         bq769x2_iic_write_reg(command, TX_data, 2);
//         bq769x2_delay_us(2000);
//         break;
//     }
// }

// See the TRM or the BQ76952 header file for a full list of Direct Commands
void bq769x2_direct_cmd_write(uint8_t command, uint16_t data)
{
    uint8_t TX_data[2] = {0x00, 0x00};

    // little endian format
    TX_data[0] = data & 0xff;
    TX_data[1] = (data >> 8) & 0xff;

    // Control_status, alarm_status, alarm_enable all 2 bytes long
    bq769x2_iic_write_reg(command, TX_data, 2);
    bq769x2_delay_us(50); // Table 9-2. Command/Subcommand Operation Time
    // TODO: clarify if delay required.
}

uint16_t bq769x2_direct_cmd_read(uint8_t command)
{
    uint8_t rx[2] = {0x00, 0x00};
    bq769x2_iic_read_reg(command, rx, 2);
    // Table 9-2. Command/Subcommand Operation Time
    // This will clock-stretch if needs delay
    bq769x2_delay_us(50); // TODO: clock stretching should mean this is not required, clarify?
    return (rx[1] << 8) | (rx[0] << 0);
}

// MACRO INVOCATION
uint32_t bq769x2_float_to_uint32_t(float f)
{
    uint32_t buf;
    bq769x2_memcpy(&buf, &f, sizeof(f));
    return buf;
}

uint16_t bq769x2_h_cb_get_active_cells()
{
    bq769x2_subcommands(CB_ACTIVE_CELLS, 0x0000, R);
    return RXBUF_TO_16(0);
}

uint16_t bq769x2_h_cb_get_pack_balancing_time()
{
    bq769x2_subcommands(CBSTATUS1, 0x0000, R);
    return RXBUF_TO_16(0);
}

uint8_t bq769x2_h_cb_get_cell_balancing_time(uint32_t balancing_time[], size_t size)
{
    if (size < 64)
    {
        bq769x2_error_handler();
        return 1;
    }

    bq769x2_subcommands(CBSTATUS2, 0x0000, R);
    bq769x2_memcpy(&balancing_time[0], BQ769X2_RX_32Byte, 32);
    bq769x2_subcommands(CBSTATUS3, 0x0000, R);
    bq769x2_memcpy(&balancing_time[8], BQ769X2_RX_32Byte, 32);
    return 0;
}

uint8_t bq769x2_h_get_cov_snapshot(int16_t cov_snapshot[], size_t size)
{
    if (size < 32)
    {
        bq769x2_error_handler();
        return 1;
    }

    bq769x2_subcommands(COV_SNAPSHOT, 0x0000, R);
    bq769x2_memcpy(cov_snapshot, BQ769X2_RX_32Byte, size);
    return 0;
}

uint8_t bq769x2_h_get_cuv_snapshot(int16_t cuv_snapshot[], size_t size)
{
    if (size < 32)
    {
        bq769x2_error_handler();
        return 1;
    }

    bq769x2_subcommands(CUV_SNAPSHOT, 0x0000, R);
    bq769x2_memcpy(cuv_snapshot, BQ769X2_RX_32Byte, size);
    return 0;
}
// ------------

// ------------

// void BQ769x2_Init()
// {
//     // Configures all parameters in device RAM

//     // CONSTANTS

//     // INR21700-50XG
//     // Charge : 0 to 60℃
//     // (Recommended recharge release < 45℃)
//     // Discharge:-20 to 80℃
//     // (Recommended re-discharge release < 60℃)
//     const int32_t CELL_CHARGE_MAX_degC = 45;
//     const int32_t CELL_CHARGE_MIN_degC = 0;
//     const int32_t CELL_DISCHARGE_MAX_degC = 60;
//     const int32_t CELL_DISCHARGE_MIN_degC = -20;
//     const int32_t CELL_MIN_degC = CELL_CHARGE_MIN_degC > CELL_DISCHARGE_MIN_degC ? CELL_CHARGE_MIN_degC : CELL_DISCHARGE_MIN_degC;
//     const int32_t CELL_MAX_degC = CELL_CHARGE_MAX_degC < CELL_DISCHARGE_MAX_degC ? CELL_CHARGE_MAX_degC : CELL_DISCHARGE_MAX_degC;

//     // Enter CONFIGUPDATE mode (Subcommand 0x0090) - It is required to be in CONFIG_UPDATE mode to program the device RAM settings
//     // See TRM for full description of CONFIG_UPDATE mode
//     CMD_SUBCMD_SET_CFGUPDATE();

//     // After entering CONFIG_UPDATE mode, RAM registers can be programmed. When programming RAM, checksum and length must also be
//     // programmed for the change to take effect. All of the RAM registers are described in detail in the BQ769x2 TRM.
//     // An easier way to find the descriptions is in the BQStudio Data Memory screen. When you move the mouse over the register name,
//     // a full description of the register and the bits will pop up on the screen.

//     /*
//      *
//      * COMMUNICATIONS
//      *
//      */

//     SET_CommType_dimensionless(CommType_I2C_FAST); // TODO: CONSIDER CRC MODE? REQUIRES LOGIC TO CHANGE ON THE FLY.
//     SET_I2CAddress_DEFAULT();                      // 0x10
//     SET_CommIdleTime_DEFAULT();                    // NO IDLE TIME EXTENSION
//                                                    // SET_SPIConfiguration_DEFAULT();

//     /*
//      *
//      * POWER / REGULATOR OPTIONS
//      *
//      */

//     // 'Power Config' - 0x9234 = 0x2D80
//     // Setting the DSLP_LDO bit allows the LDOs to remain active when the device goes into Deep Sleep mode
//     // Set wake speed bits to 00 for best performance
//     SET_PowerConfig_Raw(PowerConfig_DPSLP_OT | PowerConfig_DPSLP_PD | PowerConfig_SLEEP | PowerConfig_OTSD | PowerConfig_DPSLP_LDO |
//                         PowerConfig_FASTADC_3ms0 | PowerConfig_CB_LOOP_HALF_SPD | PowerConfig_LOOP_FULL_SPD);
//     // NOTE: half speed while cell balancing to increase average cell balancing current

//     // 'REG12 Config' - Enable REG1 with 3.3V output (0x0D for 3.3V, 0x0F for 5V)
//     SET_REG12Config_Raw(REG12Config_REG1_EN | REG12Config_REG1V_3V3);

//     // 'REG0 Config' - set REG0_EN bit to enable pre-regulator
//     SET_REG0Config_Raw(REG0Config_REG0_EN);

//     SET_HWDRegulatorOptions_Raw(HWDRegulatorOptions_TOGGLE_TIME_15s | HWDRegulatorOptions_TOGGLE_OPT_REG12_OFFON);
//     SET_HWDDelay_s(180);

//     /*
//      *
//      * ENABLED PROTECTIONS
//      *
//      */

//     // PERMANENT FAILURE PROTECTIONS
//     // -----------------------------
//     // SCDL does not recover based on charge current.
//     // OCDL does not recover based on charge current.
//     // Permanent Failure does not cause the device to blow the fuse.
//     // Device does not automatically enter DEEPSLEEP mode when a Permanent Failure occurs. (TODO:)
//     // Permanent Failure does not cause the device to turn the regulators off. (TODO:)
//     // Permanent Failure causes the device to turn the FETs off.
//     SET_ProtectionConfiguration_Raw(ProtectionConfiguration_PF_FETS);

//     // PROTECTIONS A -- CURRENT
//     // ------------------------
//     // Short Circuit in Discharge Protection
//     // Overcurrent in Discharge 1st Tier Protection
//     // Overcurrent in Charge Protection
//     // Cell Overvoltage Protection
//     // Cell Undervoltage Protection
//     SET_EnabledProtectionsA_Raw(EnabledProtectionsA_SCD | EnabledProtectionsA_OCD1 | EnabledProtectionsA_OCC | EnabledProtectionsA_COV | EnabledProtectionsA_CUV);

//     // PROTECTIONS B -- OVERTEMPERATURE
//     // --------------------------------
//     // FET Overtemperature
//     // Internal Overtemperature
//     // Overtemperature in Discharge
//     // Overtemperature in Charge
//     // IGNORE UNDERTEMPERATURE PROTECTIONS (TODO:)
//     SET_EnabledProtectionsB_Raw(EnabledProtectionsB_OTF | EnabledProtectionsB_OTINT | EnabledProtectionsB_OTD | EnabledProtectionsB_OTC);

//     // PROTECTIONS C -- OVERCURRENT LATCH & TIMEOUTS
//     // ---------------------------------------------
//     // Host Watchdog Fault
//     // TODO: PTO needed to automatically stop predischarge????
//     SET_EnabledProtectionsC_Raw(EnabledProtectionsC_HWDF);
//     //   SET_EnabledProtectionsC_Raw(EnabledProtectionsC_SCDL | EnabledProtectionsC_OCDL | EnabledProtectionsC_PTO | EnabledProtectionsC_HWDF);

//     // DISABLE PERMANENT FAIL
//     SET_PFAlertMaskA_Raw(0x00);
//     SET_PFAlertMaskB_Raw(0x00);
//     SET_PFAlertMaskC_Raw(0x00);
//     SET_PFAlertMaskD_Raw(0x00);
//     SET_EnabledPFA_Raw(0x00);
//     SET_EnabledPFB_Raw(0x00);
//     SET_EnabledPFC_Raw(0x00);
//     SET_EnabledPFD_Raw(0x00);

//     /*
//      *
//      * FET CONTROL / CHARGE PUMP
//      *
//      */

//     // MOSFETS
//     // TODO: NOTE: HOST CANNOT DISABLE FETS (FETOptions_HOST_FET_EN)
//     // TODO: CHECK IF PDSG IS REQUIRED.
//     SET_FETOptions_Raw(FETOptions_PDSG_EN | FETOptions_FET_CTRL_EN | FETOptions_HOST_FET_EN | FETOptions_SFET);
//     SET_ChgPumpControl_Raw(ChgPumpControl_CPEN); // 11V overdrive, enable charge pump

//     // > When the cell voltage is below 2.0V, cell should be pre-charged lower than 0.1C current.
//     // > Only standard charging method can be switched after voltage exceed 2.0V. If the cell
//     // > voltage unable to reach in specified period/time, stop charging immediately.
//     SET_PrechargeStartVoltage_mV(2100);
//     SET_PrechargeStopVoltage_mV(2300);

//     //   MAXIMUM PREDISCHARGE DUE TO HIGH CAPACITANCE LOAD.
//     SET_PredischargeStopDelta_mV(250);
//     SET_PredischargeTimeout_ms(2550);

//     /*
//      *
//      * CURRENT SENSE & SENSE-RESISTOR CAL
//      * (also see PowerConfig for ADC speed settings)
//      *
//      */
// #define RSENSE_mOhm 1
//     SET_CCGain_float(CCGain_DEFAULTRAW / RSENSE_mOhm);
// #if RSENSE_mOhm > 1
//     SET_CapacityGain_float(CapacityGain_MIN * CCGain_DEFAULTRAW / RSENSE_mOhm);
// #else
//     SET_CapacityGain_DEFAULT();
// #endif

//     SET_CoulombCounterOffsetSamples_DEFAULT(); // 64 slots DEFAULT
//     SET_BoardOffset_DEFAULT();                 //
//     SET_CoulombCounterDeadband_DEFAULT();      // > used to filter out signals below the expected noise floor. THIS NORMALLY SHOULD NOT BE CHANGED.
//     SET_CC3Samples_DEFAULT();                  // AVERAGE OVER CC2 SAMPLES. (DEFAULT: 80 SLOTS)
//     // TODO: is 1mA worth sacrificing for 32.767 A maximum current before saturating?
//     SET_DAConfiguration_Raw(DAConfiguration_USER_AMPS_cA_10mA | DAConfiguration_USER_VOLTS_cV_10mV); // 327.67 A maximum, 327.67 V maximum (*1cV)

//     /*
//      *
//      * CELL BALANCING
//      *
//      */

//     // DISABLE BALANCING ON 16,15,14
//     SET_VCellMode_Raw(VCellMode_ALL ^ VCellMode_16_EN ^ VCellMode_15_EN ^ VCellMode_14_EN);

//     // TODO: CELL INTERCONNECT RESISTANCES (MEASURE NICKEL STRIP)
//     SET_Cell1Interconnect_mOhm(0);
//     SET_Cell2Interconnect_mOhm(0);
//     SET_Cell3Interconnect_mOhm(0);
//     SET_Cell4Interconnect_mOhm(0);
//     SET_Cell5Interconnect_mOhm(0);
//     SET_Cell6Interconnect_mOhm(0);
//     SET_Cell7Interconnect_mOhm(0);
//     SET_Cell8Interconnect_mOhm(0);
//     SET_Cell9Interconnect_mOhm(0);
//     SET_Cell10Interconnect_mOhm(0);
//     SET_Cell11Interconnect_mOhm(0);
//     SET_Cell12Interconnect_mOhm(0);
//     SET_Cell13Interconnect_mOhm(0); // TODO:
//     SET_Cell14Interconnect_mOhm(0); // TODO:
//     SET_Cell15Interconnect_mOhm(0); // TODO:
//     SET_Cell16Interconnect_mOhm(0); // TODO:

//     // Set up Cell Balancing Configuration - 0x9335 = 0x03   -  Automated balancing while in Relax or Charge modes
//     // Also see "Cell Balancing with BQ769x2 Battery Monitors" document on ti.com
//     SET_BalancingConfiguration_Raw(BalancingConfiguration_CB_CHG | BalancingConfiguration_CB_RLX);
//     SET_MinCellTemp_degC(CELL_MIN_degC);
//     SET_MaxCellTemp_degC(CELL_MAX_degC);
//     SET_MaxInternalTemp_DEFAULT(); // SELECT 70degC, MUST BE LESS THAN OTINTThreshold (85degC)
//     SET_CellBalanceInterval_DEFAULT();
//     SET_CellBalanceMinCellVCharge_mV(3900); // PASSIVE BALANCING IS LOSSY, ONLY START BALANCING ABOVE THIS THRESHOLD.
//     SET_CellBalanceMinCellVRelax_mV(4100);  // PASSIVE BALANCING IS LOSSY, ONLY START BALANCING ABOVE THIS THRESHOLD.
//     SET_CellBalanceMinDeltaCharge_DEFAULT();
//     SET_CellBalanceStopDeltaCharge_DEFAULT();
//     SET_CellBalanceStopDeltaRelax_DEFAULT();
//     SET_CellBalanceMinDeltaRelax_DEFAULT();
//     SET_CellBalanceMaxCells_Num(4);
//     // R_DS(ON) ~= 25R, 2 R_SER = 2 20R
//     // PASSIVE BALANCING AT 4.2V -> BALANCE CURRENT 65mA
//     // -> 0.1W DISSIPATION/PARALLEL FET
//     // R_JA = 66 °C/W, FOR AMBIENT TEMPERATURE OF 35 AND LIMIT OF 70: P_D(MAX) = 0.53W
//     // -> 5 PARALLEL MAXIMUM
//     // TODO: REPORT: ACTUAL TEMPERATURES MAINTAINED ON THE MOON.

//     /*
//      *
//      * PIN CONFIG
//      *
//      */

//     SET_CFETOFFPinConfig_Raw(CFETOFFPinConfig_FXN_SPI_CS_UNUSED);
//     SET_DFETOFFPinConfig_Raw(DFETOFFPinConfig_FXN_UNUSED);
//     SET_ALERTPinConfig_Raw(ALERTPinConfig_FXN_ALERT | PIN_DIGITAL_1_PUSH_PULL | PIN_DIGITAL_5_ACTIVE_HIGH | PIN_DIGITAL_3_DRIVE_HIGH_REG1);
//     SET_TS1Config_Raw(TS1Config_FXN_ADC_THERMISTOR | PIN_ANALOG_PU_18K_NTC10K | PIN_ANALOG_POLY_PU18K_NTC10K | PIN_ANALOG_TYPE_CELL_PROT);
//     SET_TS3Config_Raw(TS3Config_FXN_ADC_THERMISTOR | PIN_ANALOG_PU_18K_NTC10K | PIN_ANALOG_POLY_PU18K_NTC10K | PIN_ANALOG_TYPE_FET_PROT);
//     //   SET_TS2Config_Raw(TS2Config_FXN_ADC_THERMISTOR | PIN_ANALOG_PU_18K_NTC10K | PIN_ANALOG_POLY_PU18K_NTC10K | PIN_ANALOG_TYPE_CELL_PROT); // TODO: REPORTING?
//     SET_TS2Config_Raw(TS2Config_FXN_UNUSED); // TODO: REPORTING?
//     SET_HDQPinConfig_Raw(HDQPinConfig_FXN_UNUSED1);
//     SET_DCHGPinConfig_Raw(DCHGPinConfig_FXN_UNUSED);
//     SET_DDSGPinConfig_Raw(DDSGPinConfig_FXN_UNUSED);

//     /*
//      *
//      * ENABLED PROTECTIONS
//      *
//      */
//     // Set up SCD Threshold - 0x9286 = 0x05 (100 mV = 100A across 1mOhm sense resistor)  0x05=100mV
//     // Set up SCD Delay - 0x9287 = 0x03 (30 us) Enabled with a delay of (value - 1) * 15 µs; min value of 1
//     // Set up SCDL Latch Limit to 1 to set SCD recovery only with load removal 0x9295 = 0x01
//     // If this is not set, then SCD will recover based on time (SCD Recovery Time parameter).
//     SET_SCDThreshold_dimensionless(SCDThreshold_100mV);
//     SET_SCDDelay_us(30);      // TODO: Coordinate with motor breakers
//     SET_SCDRecoveryTime_s(5); // TODO: DEFAULT.
//     //   TODO: USE AUTORECOVERY OR SEND SIGNAL AND HAVE MCU RECOVER

//     // OCD1 TODO:
//     // OCC TODO:

//     // COV
//     // Charging voltage must be set 4.2V/cell. Concerning charge voltage tolerance of charger,
//     // charging voltage must be set lower than 4.25V/cell. Even if the charge could be out of order,
//     // charge voltage of charger should not be more than 4.25V/cell to avoid over-charging.
//     SET_COVThreshold_mV(4240);
//     SET_COVDelay_ms(250);              // TODO: DEFAULT
//     SET_CUVRecoveryHysteresis_mV(100); // TODO: DEFAULT

//     // CUV
//     // The cell should not be over-discharged below 2.0V.
//     // Within a voltage range of 2.5V/cell, over-discharge protection should work. Then discharge
//     // current shall be shut down and consumption current is below 1μA.
//     SET_CUVThreshold_mV(2500);
//     SET_CUVDelay_ms(250);              // TODO: DEFAULT
//     SET_CUVRecoveryHysteresis_mV(100); // TODO: DEFAULT

// #define AMBIENT_TEMPERATURE 35

//     // IPT007N06N
//     // T_J(MAX): 175degC
//     // in operation, at 90A: 55.08degC/FET
//     // Select 100degC
//     SET_OTFThreshold_degC(100);
//     SET_OTFDelay_s(2);                               // TODO: DEFAULT
//     SET_OTFRecovery_degC(100 - AMBIENT_TEMPERATURE); // Allow for 35degC internal temperature (Extreme)

//     SET_OTINTThreshold_degC(85); // TODO: DEFAULT
//     SET_OTINTDelay_s(2);         // TODO: DEFAULT
//     SET_OTINTRecovery_degC(85 - AMBIENT_TEMPERATURE);

//     // DISABLE CHG FET ON:
//     // Discharge:-20 to 80℃
//     // (Recommended re-discharge release < 60℃)
//     SET_OTDThreshold_degC(60); // TODO: DEFAULT - MORE AGGRESSIVE PROFILE FOR COMP?
//     SET_OTDRecovery_degC(60 - AMBIENT_TEMPERATURE);

//     // Charge : 0 to 60℃
//     // (Recommended recharge release < 45℃)
//     SET_OTCThreshold_degC(45);
//     SET_OTCThreshold_degC(45 - 20); // COOLER IN CHARGING

//     //   SET_SCDLLatchLimit_dimensionless(1);
//     //   SET_SCDLCounterDecDelay_s(10);
//     //   SET_SCDLRecoveryTime_s(15);
//     //
//     //
//     //
//     //
//     //
//     //
//     //
//     //
//     // 'Default Alarm Mask' - 0x..82 Enables the FullScan and ADScan bits, default value = 0xF800
//     bq769x2_set_register(DefaultAlarmMask, 0xF882, 2);

//     // Set up OCC (over-current in charge) Threshold - 0x9280 = 0x05 (10 mV = 10A across 1mOhm sense resistor) Units in 2mV
//     bq769x2_set_register(OCCThreshold, 0x05, 1);

//     // Set up OCD1 Threshold - 0x9282 = 0x0A (20 mV = 20A across 1mOhm sense resistor) units of 2mV
//     bq769x2_set_register(OCD1Threshold, 0x0A, 1);

//     // Exit CONFIGUPDATE mode  - Subcommand 0x0092
//     CMD_SUBCMD_EXIT_CFGUPDATE();
// }
