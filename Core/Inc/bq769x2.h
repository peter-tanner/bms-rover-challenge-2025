// BQ769x2 General Program Header File
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

#ifndef __BQ769X2_H
#define __BQ769X2_H

#include <assert.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

// MODIFY AS REQUIRED
#ifdef USE_HAL_DRIVER // stm32 hal
extern void Error_Handler(void);
#define bq769x2_error_handler() Error_Handler();
#else
#define bq769x2_error_handler() ;
#endif

struct balancing_time_T
{
    uint32_t cb_s[16];
};

struct cov_snapshot_T
{
    int16_t cov_snapshot_mV[16];
};

struct cuv_snapshot_T
{
    int16_t cuv_snapshot_mV[16];
};

extern void bq769x2_delay_us(uint16_t us);
extern uint8_t bq769x2_iic_write(uint16_t DevAddress, uint16_t MemAddress,
                                 uint16_t MemAddSize, uint8_t *pData, uint16_t Size, uint32_t Timeout);
extern uint8_t bq769x2_iic_read(uint16_t DevAddress, uint16_t MemAddress,
                                uint16_t MemAddSize, uint8_t *pData, uint16_t Size, uint32_t Timeout);
extern void bq769x2_mutex_get();
extern void bq769x2_mutex_release();

void bq769x2_set_register(uint16_t reg_addr, uint32_t reg_data, uint8_t datalen);
void bq769x2_command_subcommands(uint16_t command, uint16_t command_delay); // For Command only Subcommands
#define bq769x2_subcommands(command, data, type) _bq769x2_subcommands(command, command##_TIME, data, type);
void _bq769x2_subcommands(uint16_t command, uint16_t delay, uint16_t data, uint8_t type);
void bq769x2_direct_commands(uint8_t command, uint16_t data, uint8_t type);
void bq769x2_direct_cmd_write(uint8_t command, uint16_t data);
uint16_t bq769x2_direct_cmd_read(uint8_t command);
uint32_t bq769x2_float_to_uint32_t(float f);
uint16_t bq769x2_h_cb_get_active_cells();
uint16_t bq769x2_h_cb_get_pack_balancing_time();
uint8_t bq769x2_h_cb_get_cell_balancing_time(uint32_t balancing_time[], size_t len_balancing_time);
uint8_t bq769x2_h_get_cov_snapshot(int16_t cov_snapshot[], size_t size);
uint8_t bq769x2_h_get_cuv_snapshot(int16_t cuv_snapshot[], size_t size);

void BQ769x2_ReadFETStatus();
uint16_t BQ769x2_ReadAlarmStatus();
void BQ769x2_ReadSafetyStatus(); // good example functions
void BQ769x2_ReadPFStatus();
uint16_t BQ769x2_ReadVoltage(uint8_t command);
void BQ769x2_ReadAllVoltages();
uint16_t BQ769x2_ReadCurrent();
float BQ769x2_ReadTemperature(uint8_t command);
void BQ769x2_ReadPassQ(); // Read Accumulated Charge and Time from DASTATUS6
void BQ769x2_BOTHOFF();
void BQ769x2_RESET_BOTHOFF();
void BQ769x2_ShutdownPin();
void BQ769x2_ReleaseShutdownPin();

uint16_t bq769x2_direct_cmd_read(uint8_t command);

#define DEV_ADDR 0x10 // BQ769x2 address is 0x10 including R/W bit or 0x8 as 7-bit address
#define CRC_Mode 1    // 0 for disabled, 1 for enabled
#define R 0           // Read; Used in DirectCommands and Subcommands functions
#define W 1           // Write; Used in DirectCommands and Subcommands functions
#define W2 2          // Write data with two bytes; Used in Subcommands function

// TYPES
#define I2 2
#define U2 2
#define I1 1
#define U1 1
#define F4 4
#define H1 1
#define H2 2
#define H3 3
#define H4 4

// Data	Memory	registers	Name in TRM
#define Cell1Gain 0x9180                    // Calibration:Voltage:Cell 1 Gain
#define Cell2Gain 0x9182                    // Calibration:Voltage:Cell 2 Gain
#define Cell3Gain 0x9184                    // Calibration:Voltage:Cell 3 Gain
#define Cell4Gain 0x9186                    // Calibration:Voltage:Cell 4 Gain
#define Cell5Gain 0x9188                    // Calibration:Voltage:Cell 5 Gain
#define Cell6Gain 0x918A                    // Calibration:Voltage:Cell 6 Gain
#define Cell7Gain 0x918C                    // Calibration:Voltage:Cell 7 Gain
#define Cell8Gain 0x918E                    // Calibration:Voltage:Cell 8 Gain
#define Cell9Gain 0x9190                    // Calibration:Voltage:Cell 9 Gain
#define Cell10Gain 0x9192                   // Calibration:Voltage:Cell 10 Gain
#define Cell11Gain 0x9194                   // Calibration:Voltage:Cell 11 Gain
#define Cell12Gain 0x9196                   // Calibration:Voltage:Cell 12 Gain
#define Cell13Gain 0x9198                   // Calibration:Voltage:Cell 13 Gain
#define Cell14Gain 0x919A                   // Calibration:Voltage:Cell 14 Gain
#define Cell15Gain 0x919C                   // Calibration:Voltage:Cell 15 Gain
#define Cell16Gain 0x919E                   // Calibration:Voltage:Cell 16 Gain
#define PackGain 0x91A0                     // Calibration:Voltage:Pack Gain
#define TOSGain 0x91A2                      // Calibration:Voltage:TOS Gain
#define LDGain 0x91A4                       // Calibration:Voltage:LD Gain
#define ADCGain 0x91A6                      // Calibration:Voltage:ADC Gain
#define CCGain 0x91A8                       // Calibration:Current:CC Gain
#define CapacityGain 0x91AC                 // Calibration:Current:Capacity Gain
#define VcellOffset 0x91B0                  // Calibration:Vcell Offset:Vcell Offset
#define VdivOffset 0x91B2                   // Calibration:V Divider Offset:Vdiv Offset
#define CoulombCounterOffsetSamples 0x91C6  // Calibration:Current Offset:Coulomb Counter Offset Samples
#define BoardOffset 0x91C8                  // Calibration:Current Offset:Board Offset
#define InternalTempOffset 0x91CA           // Calibration:Temperature:Internal Temp Offset
#define CFETOFFTempOffset 0x91CB            // Calibration:Temperature:CFETOFF Temp Offset
#define DFETOFFTempOffset 0x91CC            // Calibration:Temperature:DFETOFF Temp Offset
#define ALERTTempOffset 0x91CD              // Calibration:Temperature:ALERT Temp Offset
#define TS1TempOffset 0x91CE                // Calibration:Temperature:TS1 Temp Offset
#define TS2TempOffset 0x91CF                // Calibration:Temperature:TS2 Temp Offset
#define TS3TempOffset 0x91D0                // Calibration:Temperature:TS3 Temp Offset
#define HDQTempOffset 0x91D1                // Calibration:Temperature:HDQ Temp Offset
#define DCHGTempOffset 0x91D2               // Calibration:Temperature:DCHG Temp Offset
#define DDSGTempOffset 0x91D3               // Calibration:Temperature:DDSG Temp Offset
#define IntGain 0x91E2                      // Calibration:Internal Temp Model:Int Gain
#define Intbaseoffset 0x91E4                // Calibration:Internal Temp Model:Int base offset
#define IntMaximumAD 0x91E6                 // Calibration:Internal Temp Model:Int Maximum AD
#define IntMaximumTemp 0x91E8               // Calibration:Internal Temp Model:Int Maximum Temp
#define T18kCoeffa1 0x91EA                  // Calibration:18K Temperature Model:Coeff a1
#define T18kCoeffa2 0x91EC                  // Calibration:18K Temperature Model:Coeff a2
#define T18kCoeffa3 0x91EE                  // Calibration:18K Temperature Model:Coeff a3
#define T18kCoeffa4 0x91F0                  // Calibration:18K Temperature Model:Coeff a4
#define T18kCoeffa5 0x91F2                  // Calibration:18K Temperature Model:Coeff a5
#define T18kCoeffb1 0x91F4                  // Calibration:18K Temperature Model:Coeff b1
#define T18kCoeffb2 0x91F6                  // Calibration:18K Temperature Model:Coeff b2
#define T18kCoeffb3 0x91F8                  // Calibration:18K Temperature Model:Coeff b3
#define T18kCoeffb4 0x91FA                  // Calibration:18K Temperature Model:Coeff b4
#define T18kAdc0 0x91FE                     // Calibration:18K Temperature Model:Adc0
#define T180kCoeffa1 0x9200                 // Calibration:180K Temperature Model:Coeff a1
#define T180kCoeffa2 0x9202                 // Calibration:180K Temperature Model:Coeff a2
#define T180kCoeffa3 0x9204                 // Calibration:180K Temperature Model:Coeff a3
#define T180kCoeffa4 0x9206                 // Calibration:180K Temperature Model:Coeff a4
#define T180kCoeffa5 0x9208                 // Calibration:180K Temperature Model:Coeff a5
#define T180kCoeffb1 0x920A                 // Calibration:180K Temperature Model:Coeff b1
#define T180kCoeffb2 0x920C                 // Calibration:180K Temperature Model:Coeff b2
#define T180kCoeffb3 0x920E                 // Calibration:180K Temperature Model:Coeff b3
#define T180kCoeffb4 0x9210                 // Calibration:180K Temperature Model:Coeff b4
#define T180kAdc0 0x9214                    // Calibration:180K Temperature Model:Adc0
#define CustomCoeffa1 0x9216                // Calibration:Custom Temperature Model:Coeff a1
#define CustomCoeffa2 0x9218                // Calibration:Custom Temperature Model:Coeff a2
#define CustomCoeffa3 0x921A                // Calibration:Custom Temperature Model:Coeff a3
#define CustomCoeffa4 0x921C                // Calibration:Custom Temperature Model:Coeff a4
#define CustomCoeffa5 0x921E                // Calibration:Custom Temperature Model:Coeff a5
#define CustomCoeffb1 0x9220                // Calibration:Custom Temperature Model:Coeff b1
#define CustomCoeffb2 0x9222                // Calibration:Custom Temperature Model:Coeff b2
#define CustomCoeffb3 0x9224                // Calibration:Custom Temperature Model:Coeff b3
#define CustomCoeffb4 0x9226                // Calibration:Custom Temperature Model:Coeff b4
#define CustomRc0 0x9228                    // Calibration:Custom Temperature Model:Rc0
#define CustomAdc0 0x922A                   // Calibration:Custom Temperature Model:Adc0
#define CoulombCounterDeadband 0x922D       // Calibration:Current Deadband:Coulomb Counter Deadband
#define CUVThresholdOverride 0x91D4         // Calibration:CUV:CUV Threshold Override
#define COVThresholdOverride 0x91D6         // Calibration:COV:COV Threshold Override
#define MinBlowFuseVoltage 0x9231           // Settings:Fuse:Min Blow Fuse Voltage
#define FuseBlowTimeout 0x9233              // Settings:Fuse:Fuse Blow Timeout
#define PowerConfig 0x9234                  // Settings:Configuration:Power Config
#define REG12Config 0x9236                  // Settings:Configuration:REG12 Config
#define REG0Config 0x9237                   // Settings:Configuration:REG0 Config
#define HWDRegulatorOptions 0x9238          // Settings:Configuration:HWD Regulator Options
#define CommType 0x9239                     // Settings:Configuration:Comm Type
#define I2CAddress 0x923A                   // Settings:Configuration:I2C Address
#define SPIConfiguration 0x923C             // Settings:Configuration:SPI Configuration
#define CommIdleTime 0x923D                 // Settings:Configuration:Comm Idle Time
#define CFETOFFPinConfig 0x92FA             // Settings:Configuration:CFETOFF Pin Config
#define DFETOFFPinConfig 0x92FB             // Settings:Configuration:DFETOFF Pin Config
#define ALERTPinConfig 0x92FC               // Settings:Configuration:ALERT Pin Config
#define TS1Config 0x92FD                    // Settings:Configuration:TS1 Config
#define TS2Config 0x92FE                    // Settings:Configuration:TS2 Config
#define TS3Config 0x92FF                    // Settings:Configuration:TS3 Config
#define HDQPinConfig 0x9300                 // Settings:Configuration:HDQ Pin Config
#define DCHGPinConfig 0x9301                // Settings:Configuration:DCHG Pin Config
#define DDSGPinConfig 0x9302                // Settings:Configuration:DDSG Pin Config
#define DAConfiguration 0x9303              // Settings:Configuration:DA Configuration
#define VCellMode 0x9304                    // Settings:Configuration:Vcell Mode
#define CC3Samples 0x9307                   // Settings:Configuration:CC3 Samples
#define ProtectionConfiguration 0x925F      // Settings:Protection:Protection Configuration
#define EnabledProtectionsA 0x9261          // Settings:Protection:Enabled Protections A
#define EnabledProtectionsB 0x9262          // Settings:Protection:Enabled Protections B
#define EnabledProtectionsC 0x9263          // Settings:Protection:Enabled Protections C
#define CHGFETProtectionsA 0x9265           // Settings:Protection:CHG FET Protections A
#define CHGFETProtectionsB 0x9266           // Settings:Protection:CHG FET Protections B
#define CHGFETProtectionsC 0x9267           // Settings:Protection:CHG FET Protections C
#define DSGFETProtectionsA 0x9269           // Settings:Protection:DSG FET Protections A
#define DSGFETProtectionsB 0x926A           // Settings:Protection:DSG FET Protections B
#define DSGFETProtectionsC 0x926B           // Settings:Protection:DSG FET Protections C
#define BodyDiodeThreshold 0x9273           // Settings:Protection:Body Diode Threshold
#define DefaultAlarmMask 0x926D             // Settings:Alarm:Default Alarm Mask
#define SFAlertMaskA 0x926F                 // Settings:Alarm:SF Alert Mask A
#define SFAlertMaskB 0x9270                 // Settings:Alarm:SF Alert Mask B
#define SFAlertMaskC 0x9271                 // Settings:Alarm:SF Alert Mask C
#define PFAlertMaskA 0x92C4                 // Settings:Alarm:PF Alert Mask A
#define PFAlertMaskB 0x92C5                 // Settings:Alarm:PF Alert Mask B
#define PFAlertMaskC 0x92C6                 // Settings:Alarm:PF Alert Mask C
#define PFAlertMaskD 0x92C7                 // Settings:Alarm:PF Alert Mask D
#define EnabledPFA 0x92C0                   // Settings:Permanent Failure:Enabled PF A
#define EnabledPFB 0x92C1                   // Settings:Permanent Failure:Enabled PF B
#define EnabledPFC 0x92C2                   // Settings:Permanent Failure:Enabled PF C
#define EnabledPFD 0x92C3                   // Settings:Permanent Failure:Enabled PF D
#define FETOptions 0x9308                   // Settings:FET:FET Options
#define ChgPumpControl 0x9309               // Settings:FET:Chg Pump Control
#define PrechargeStartVoltage 0x930A        // Settings:FET:Precharge Start Voltage
#define PrechargeStopVoltage 0x930C         // Settings:FET:Precharge Stop Voltage
#define PredischargeTimeout 0x930E          // Settings:FET:Predischarge Timeout
#define PredischargeStopDelta 0x930F        // Settings:FET:Predischarge Stop Delta
#define DsgCurrentThreshold 0x9310          // Settings:Current Thresholds:Dsg Current Threshold
#define ChgCurrentThreshold 0x9312          // Settings:Current Thresholds:Chg Current Threshold
#define CheckTime 0x9314                    // Settings:Cell Open-Wire:Check Time
#define Cell1Interconnect 0x9315            // Settings:Interconnect Resistances:Cell 1 Interconnect
#define Cell2Interconnect 0x9317            // Settings:Interconnect Resistances:Cell 2 Interconnect
#define Cell3Interconnect 0x9319            // Settings:Interconnect Resistances:Cell 3 Interconnect
#define Cell4Interconnect 0x931B            // Settings:Interconnect Resistances:Cell 4 Interconnect
#define Cell5Interconnect 0x931D            // Settings:Interconnect Resistances:Cell 5 Interconnect
#define Cell6Interconnect 0x931F            // Settings:Interconnect Resistances:Cell 6 Interconnect
#define Cell7Interconnect 0x9321            // Settings:Interconnect Resistances:Cell 7 Interconnect
#define Cell8Interconnect 0x9323            // Settings:Interconnect Resistances:Cell 8 Interconnect
#define Cell9Interconnect 0x9325            // Settings:Interconnect Resistances:Cell 9 Interconnect
#define Cell10Interconnect 0x9327           // Settings:Interconnect Resistances:Cell 10 Interconnect
#define Cell11Interconnect 0x9329           // Settings:Interconnect Resistances:Cell 11 Interconnect
#define Cell12Interconnect 0x932B           // Settings:Interconnect Resistances:Cell 12 Interconnect
#define Cell13Interconnect 0x932D           // Settings:Interconnect Resistances:Cell 13 Interconnect
#define Cell14Interconnect 0x932F           // Settings:Interconnect Resistances:Cell 14 Interconnect
#define Cell15Interconnect 0x9331           // Settings:Interconnect Resistances:Cell 15 Interconnect
#define Cell16Interconnect 0x9333           // Settings:Interconnect Resistances:Cell 16 Interconnect
#define MfgStatusInit 0x9343                // Settings:Manufacturing:Mfg Status Init
#define BalancingConfiguration 0x9335       // Settings:Cell Balancing Config:Balancing Configuration
#define MinCellTemp 0x9336                  // Settings:Cell Balancing Config:Min Cell Temp
#define MaxCellTemp 0x9337                  // Settings:Cell Balancing Config:Max Cell Temp
#define MaxInternalTemp 0x9338              // Settings:Cell Balancing Config:Max Internal Temp
#define CellBalanceInterval 0x9339          // Settings:Cell Balancing Config:Cell Balance Interval
#define CellBalanceMaxCells 0x933A          // Settings:Cell Balancing Config:Cell Balance Max Cells
#define CellBalanceMinCellVCharge 0x933B    // Settings:Cell Balancing Config:Cell Balance Min Cell V (Charge)
#define CellBalanceMinDeltaCharge 0x933D    // Settings:Cell Balancing Config:Cell Balance Min Delta (Charge)
#define CellBalanceStopDeltaCharge 0x933E   // Settings:Cell Balancing Config:Cell Balance Stop Delta (Charge)
#define CellBalanceMinCellVRelax 0x933F     // Settings:Cell Balancing Config:Cell Balance Min Cell V (Relax)
#define CellBalanceMinDeltaRelax 0x9341     // Settings:Cell Balancing Config:Cell Balance Min Delta (Relax)
#define CellBalanceStopDeltaRelax 0x9342    // Settings:Cell Balancing Config:Cell Balance Stop Delta (Relax)
#define ShutdownCellVoltage 0x923F          // Power:Shutdown:Shutdown Cell Voltage
#define ShutdownStackVoltage 0x9241         // Power:Shutdown:Shutdown Stack Voltage
#define LowVShutdownDelay 0x9243            // Power:Shutdown:Low V Shutdown Delay
#define ShutdownTemperature 0x9244          // Power:Shutdown:Shutdown Temperature
#define ShutdownTemperatureDelay 0x9245     // Power:Shutdown:Shutdown Temperature Delay
#define FETOffDelay 0x9252                  // Power:Shutdown:FET Off Delay
#define ShutdownCommandDelay 0x9253         // Power:Shutdown:Shutdown Command Delay
#define AutoShutdownTime 0x9254             // Power:Shutdown:Auto Shutdown Time
#define RAMFailShutdownTime 0x9255          // Power:Shutdown:RAM Fail Shutdown Time
#define SleepCurrent 0x9248                 // Power:Sleep:Sleep Current
#define VoltageTime 0x924A                  // Power:Sleep:Voltage Time
#define WakeComparatorCurrent 0x924B        // Power:Sleep:Wake Comparator Current
#define SleepHysteresisTime 0x924D          // Power:Sleep:Sleep Hysteresis Time
#define SleepChargerVoltageThreshold 0x924E // Power:Sleep:Sleep Charger Voltage Threshold
#define SleepChargerPACKTOSDelta 0x9250     // Power:Sleep:Sleep Charger PACK-TOS Delta
#define ConfigRAMSignature 0x91E0           // System Data:Integrity:Config RAM Signature
#define CUVThreshold 0x9275                 // Protections:CUV:Threshold
#define CUVDelay 0x9276                     // Protections:CUV:Delay
#define CUVRecoveryHysteresis 0x927B        // Protections:CUV:Recovery Hysteresis
#define COVThreshold 0x9278                 // Protections:COV:Threshold
#define COVDelay 0x9279                     // Protections:COV:Delay
#define COVRecoveryHysteresis 0x927C        // Protections:COV:Recovery Hysteresis
#define COVLLatchLimit 0x927D               // Protections:COVL:Latch Limit
#define COVLCounterDecDelay 0x927E          // Protections:COVL:Counter Dec Delay
#define COVLRecoveryTime 0x927F             // Protections:COVL:Recovery Time
#define OCCThreshold 0x9280                 // Protections:OCC:Threshold
#define OCCDelay 0x9281                     // Protections:OCC:Delay
#define OCCRecoveryThreshold 0x9288         // Protections:OCC:Recovery Threshold
#define OCCPACKTOSDelta 0x92B0              // Protections:OCC:PAAK-AOS mAa
#define OCD1Threshold 0x9282                // Protections:OCD1:Thre-20shold
#define OCD1Delay 0x9283                    // Protections:OCD1:Delay
#define OCD2Threshold 0x9284                // Protections:OCD2:Threshold
#define OCD2Delay 0x9285                    // Protections:OCD2:Delay
#define SCDThreshold 0x9286                 // Protections:SCD:Threshold
#define SCDDelay 0x9287                     // Protections:SCD:Delay
#define SCDRecoveryTime 0x9294              // Protections:SCD:Recovery Time
#define OCD3Threshold 0x928A                // Protections:OCD3:Threshold
#define OCD3Delay 0x928C                    // Protections:OCD3:Delay
#define OCDRecoveryThreshold 0x928D         // Protections:OCD:Recovery Threshold
#define OCDLLatchLimit 0x928F               // Protections:OCDLALaAch mAt
#define OCDLCounterDecDelay 0x9290          // Protections:OCDL:Co-20unter Dec Delay
#define OCDLRecoveryTime 0x9291             // Protections:OCDL:Recovery Time
#define OCDLRecoveryThreshold 0x9292        // Protections:OCDL:Recovery Threshold
#define SCDLLatchLimit 0x9295               // Protections:SCDLALaAch mAt
#define SCDLCounterDecDelay 0x9296          // Protections:SCDL:Co-20unter Dec Delay
#define SCDLRecoveryTime 0x9297             // Protections:SCDL:Recovery Time
#define SCDLRecoveryThreshold 0x9298        // Protections:SCDL:Recovery Threshold
#define OTCThreshold 0x929A                 // ProtectioAs:ATC:mAd
#define OTCDelay 0x920B                     // Protections:-20OTC:Delay
#define OTCRecovery 0x929C                  // Protections:OTC:Recovery
#define OTDThreshold 0x929D                 // Protections:OTD:Threshold
#define OTDDelay 0x929E                     // Protections:OTD:Delay
#define OTDRecovery 0x929F                  // Protections:OTD:Recovery
#define OTFThreshold 0x92A0                 // Protections:OTF:Threshold
#define OTFDelay 0x92A1                     // Protections:OTF:Delay
#define OTFRecovery 0x92A2                  // Protections:OTF:Recovery
#define OTINTThreshold 0x92A3               // Protections:OTINT:Threshold
#define OTINTDelay 0x92A4                   // Protections:OTINT:Delay
#define OTINTRecovery 0x92A5                // Protections:OTINT:Recovery
#define UTCThreshold 0x92A6                 // Protections:UTC:Threshold
#define UTCDelay 0x92A7                     // Protections:UTC:Delay
#define UTCRecovery 0x92A8                  // Protections:UTC:Recovery
#define UTDThreshold 0x92A9                 // Protections:UTD:Threshold
#define UTDDelay 0x92AA                     // Protections:UTD:Delay
#define UTDRecovery 0x92AB                  // Protections:UTD:Recovery
#define UTINTThreshold 0x92AC               // Protections:UTINT:Threshold
#define UTINTDelay 0x92AD                   // Protections:UTINT:Delay
#define UTINTRecovery 0x92AE                // Protections:UTINT:Recovery
#define ProtectionsRecoveryTime 0x92AF      // Protections:Recovery:Time
#define HWDDelay 0x92B2                     // Protections:HWD:Delay
#define LoadDetectActiveTime 0x92B4         // Protections:Load Detect:Active Time
#define LoadDetectRetryDelay 0x92B5         // Protections:Load Detect:Retry Delay
#define LoadDetectTimeout 0x92B6            // Protections:Load Detect:Timeout
#define PTOChargeThreshold 0x92BA           // Protections:PTO:Charge Threshold
#define PTODelay 0x92BC                     // Protections:PTO:Delay
#define PTOReset 0x92BE                     // Protections:PTO:Reset
#define CUDEPThreshold 0x92C8               // Permanent Fail:CUDEP:Threshold
#define CUDEPDelay 0x92CA                   // Permanent Fail:CUDEP:Delay
#define SUVThreshold 0x92CB                 // Permanent Fail:SUV:Threshold
#define SUVDelay 0x92CD                     // Permanent Fail:SUV:Delay
#define SOVThreshold 0x92CE                 // Permanent Fail:SOV:Threshold
#define SOVDelay 0x92D0                     // Permanent Fail:SOV:Delay
#define TOSSThreshold 0x92D1                // Permanent Fail:TOS:Threshold
#define TOSSDelay 0x92D3                    // Permanent Fail:TOS:Delay
#define SOCCThreshold 0x92D4                // Permanent Fail:SOCC:Threshold
#define SOCCDelay 0x92D6                    // Permanent Fail:SOCC:Delay
#define SOCDThreshold 0x92D7                // Permanent Fail:SOCD:Threshold
#define SOCDDelay 0x92D9                    // Permanent Fail:SOCD:Delay
#define SOTThreshold 0x92DA                 // Permanent Fail:SOT:Threshold
#define SOTDelay 0x92DB                     // Permanent Fail:SOT:Delay
#define SOTFThreshold 0x92DC                // Permanent Fail:SOTF:Threshold
#define SOTFDelay 0x92DD                    // Permanent Fail:SOTF:Delay
#define VIMRCheckVoltage 0x92DE             // Permanent Fail:VIMR:Check Voltage
#define VIMRMaxRelaxCurrent 0x92E0          // Permanent Fail:VIMR:Max Relax Current
#define VIMRThreshold 0x92E2                // Permanent Fail:VIMR:Threshold
#define VIMRDelay 0x92E4                    // Permanent Fail:VIMR:Delay
#define VIMRRelaxMinDuration 0x92E5         // Permanent Fail:VIMR:Relax Min Duration
#define VIMACheckVoltage 0x92E7             // Permanent Fail:VIMA:Check Voltage
#define VIMAMinActiveCurrent 0x92E9         // Permanent Fail:VIMA:Min Active Current
#define VIMAThreshold 0x92EB                // Permanent Fail:VIMA:Threshold
#define VIMADelay 0x92ED                    // Permanent Fail:VIMA:Delay
#define CFETFOFFThreshold 0x92EE            // Permanent Fail:CFETF:OFF Threshold
#define CFETFOFFDelay 0x92F0                // Permanent Fail:CFETF:OFF Delay
#define DFETFOFFThreshold 0x92F1            // Permanent Fail:DFETF:OFF Threshold
#define DFETFOFFDelay 0x92F3                // Permanent Fail:DFETF:OFF Delay
#define VSSFFailThreshold 0x92F4            // Permanent Fail:VSSF:Fail Threshold
#define VSSFDelay 0x92F6                    // Permanent Fail:VSSF:Delay
#define PF2LVLDelay 0x92F7                  // Permanent Fail:2LVL:Delay
#define LFOFDelay 0x92F8                    // Permanent Fail:LFOF:Delay
#define HWMXDelay 0x92F9                    // Permanent Fail:HWMX:Delay
#define SecuritySettings 0x9256             // Security:Settings:Security Settings
#define UnsealKeyStep1 0x9257               // Security:Keys:Unseal Key Step 1
#define UnsealKeyStep2 0x9259               // Security:Keys:Unseal Key Step 2
#define FullAccessKeyStep1 0x925B           // Security:Keys:Full Access Key Step 1
#define FullAccessKeyStep2 0x925D           // Security:Keys:Full Access Key Step 2

// Direct Commands
#define ControlStatus 0x00
#define SafetyAlertA 0x02
#define SafetyStatusA 0x03
#define SafetyAlertB 0x04
#define SafetyStatusB 0x05
#define SafetyAlertC 0x06
#define SafetyStatusC 0x07
#define PFAlertA 0x0A
#define PFStatusA 0x0B
#define PFAlertB 0x0C
#define PFStatusB 0x0D
#define PFAlertC 0x0E
#define PFStatusC 0x0F
#define PFAlertD 0x10
#define PFStatusD 0x11
#define BatteryStatus 0x12
#define Cell1Voltage 0x14
#define Cell2Voltage 0x16
#define Cell3Voltage 0x18
#define Cell4Voltage 0x1A
#define Cell5Voltage 0x1C
#define Cell6Voltage 0x1E
#define Cell7Voltage 0x20
#define Cell8Voltage 0x22
#define Cell9Voltage 0x24
#define Cell10Voltage 0x26
#define Cell11Voltage 0x28
#define Cell12Voltage 0x2A
#define Cell13Voltage 0x2C
#define Cell14Voltage 0x2E
#define Cell15Voltage 0x30
#define Cell16Voltage 0x32
#define StackVoltage 0x34
#define PACKPinVoltage 0x36
#define LDPinVoltage 0x38
#define CC2Current 0x3A
#define AlarmStatus 0x62
#define AlarmRawStatus 0x64
#define AlarmEnable 0x66
#define IntTemperature 0x68
#define CFETOFFTemperature 0x6A
#define DFETOFFTemperature 0x6C
#define ALERTTemperature 0x6E
#define TS1Temperature 0x70
#define TS2Temperature 0x72
#define TS3Temperature 0x74
#define HDQTemperature 0x76
#define DCHGTemperature 0x78
#define DDSGTemperature 0x7A
#define FETStatus 0x7F

#define BQ769X2_READ_REG(register, type) \
    bq769x2_direct_cmd_read(register)

#define BQ769X2_WRITE_REG(register, value) \
    bq769x2_direct_cmd_write(register, value)

#define DIRCMD_WRITE_ControlStatus_Raw(Raw) bq769x2_direct_cmd_write(ControlStatus, Raw)
#define DIRCMD_WRITE_AlarmStatus_Raw(Raw) bq769x2_direct_cmd_write(AlarmStatus, Raw)
#define DIRCMD_WRITE_AlarmEnable_Raw(Raw) bq769x2_direct_cmd_write(AlarmEnable, Raw)

#define DIRCMD_READ_ControlStatus_Raw() BQ769X2_READ_REG(ControlStatus, H2)
#define DIRCMD_READ_SafetyAlertA_Raw() BQ769X2_READ_REG(SafetyAlertA, H1)
#define DIRCMD_READ_SafetyStatusA_Raw() BQ769X2_READ_REG(SafetyStatusA, H1)
#define DIRCMD_READ_SafetyAlertB_Raw() BQ769X2_READ_REG(SafetyAlertB, H1)
#define DIRCMD_READ_SafetyStatusB_Raw() BQ769X2_READ_REG(SafetyStatusB, H1)
#define DIRCMD_READ_SafetyAlertC_Raw() BQ769X2_READ_REG(SafetyAlertC, H1)
#define DIRCMD_READ_SafetyStatusC_Raw() BQ769X2_READ_REG(SafetyStatusC, H1)
#define DIRCMD_READ_PFAlertA_Raw() BQ769X2_READ_REG(PFAlertA, H1)
#define DIRCMD_READ_PFStatusA_Raw() BQ769X2_READ_REG(PFStatusA, H1)
#define DIRCMD_READ_PFAlertB_Raw() BQ769X2_READ_REG(PFAlertB, H1)
#define DIRCMD_READ_PFStatusB_Raw() BQ769X2_READ_REG(PFStatusB, H1)
#define DIRCMD_READ_PFAlertC_Raw() BQ769X2_READ_REG(PFAlertC, H1)
#define DIRCMD_READ_PFStatusC_Raw() BQ769X2_READ_REG(PFStatusC, H1)
#define DIRCMD_READ_PFAlertD_Raw() BQ769X2_READ_REG(PFAlertD, H1)
#define DIRCMD_READ_PFStatusD_Raw() BQ769X2_READ_REG(PFStatusD, H1)
#define DIRCMD_READ_BatteryStatus_Raw() BQ769X2_READ_REG(BatteryStatus, H2)
#define DIRCMD_READ_Cell1Voltage_mV() BQ769X2_READ_REG(Cell1Voltage, I2)
#define DIRCMD_READ_Cell2Voltage_mV() BQ769X2_READ_REG(Cell2Voltage, I2)
#define DIRCMD_READ_Cell3Voltage_mV() BQ769X2_READ_REG(Cell3Voltage, I2)
#define DIRCMD_READ_Cell4Voltage_mV() BQ769X2_READ_REG(Cell4Voltage, I2)
#define DIRCMD_READ_Cell5Voltage_mV() BQ769X2_READ_REG(Cell5Voltage, I2)
#define DIRCMD_READ_Cell6Voltage_mV() BQ769X2_READ_REG(Cell6Voltage, I2)
#define DIRCMD_READ_Cell7Voltage_mV() BQ769X2_READ_REG(Cell7Voltage, I2)
#define DIRCMD_READ_Cell8Voltage_mV() BQ769X2_READ_REG(Cell8Voltage, I2)
#define DIRCMD_READ_Cell9Voltage_mV() BQ769X2_READ_REG(Cell9Voltage, I2)
#define DIRCMD_READ_Cell10Voltage_mV() BQ769X2_READ_REG(Cell10Voltage, I2)
#define DIRCMD_READ_Cell11Voltage_mV() BQ769X2_READ_REG(Cell11Voltage, I2)
#define DIRCMD_READ_Cell12Voltage_mV() BQ769X2_READ_REG(Cell12Voltage, I2)
#define DIRCMD_READ_Cell13Voltage_mV() BQ769X2_READ_REG(Cell13Voltage, I2)
#define DIRCMD_READ_Cell14Voltage_mV() BQ769X2_READ_REG(Cell14Voltage, I2)
#define DIRCMD_READ_Cell15Voltage_mV() BQ769X2_READ_REG(Cell15Voltage, I2)
#define DIRCMD_READ_Cell16Voltage_mV() BQ769X2_READ_REG(Cell16Voltage, I2)
#define DIRCMD_READ_StackVoltage_userV() BQ769X2_READ_REG(StackVoltage, I2)
#define DIRCMD_READ_PACKPinVoltage_userV() BQ769X2_READ_REG(PACKPinVoltage, I2)
#define DIRCMD_READ_LDPinVoltage_userV() BQ769X2_READ_REG(LDPinVoltage, I2)
#define DIRCMD_READ_CC2Current_userA() BQ769X2_READ_REG(CC2Current, I2)
#define DIRCMD_READ_AlarmStatus_Raw() BQ769X2_READ_REG(AlarmStatus, H2)
#define DIRCMD_READ_AlarmRawStatus_Raw() BQ769X2_READ_REG(AlarmRawStatus, H2)
#define DIRCMD_READ_AlarmEnable_Raw() BQ769X2_READ_REG(AlarmEnable, H2)
#define DIRCMD_READ_IntTemperature_0K1() BQ769X2_READ_REG(IntTemperature, I2)
#define DIRCMD_READ_CFETOFFTemperature_0K1() BQ769X2_READ_REG(CFETOFFTemperature, I2)
#define DIRCMD_READ_DFETOFFTemperature_0K1() BQ769X2_READ_REG(DFETOFFTemperature, I2)
#define DIRCMD_READ_ALERTTemperature_0K1() BQ769X2_READ_REG(ALERTTemperature, I2)
#define DIRCMD_READ_TS1Temperature_0K1() BQ769X2_READ_REG(TS1Temperature, I2)
#define DIRCMD_READ_TS2Temperature_0K1() BQ769X2_READ_REG(TS2Temperature, I2)
#define DIRCMD_READ_TS3Temperature_0K1() BQ769X2_READ_REG(TS3Temperature, I2)
#define DIRCMD_READ_HDQTemperature_0K1() BQ769X2_READ_REG(HDQTemperature, I2)
#define DIRCMD_READ_DCHGTemperature_0K1() BQ769X2_READ_REG(DCHGTemperature, I2)
#define DIRCMD_READ_DDSGTemperature_0K1() BQ769X2_READ_REG(DDSGTemperature, I2)
#define DIRCMD_READ_FETStatus_Raw() BQ769X2_READ_REG(FETStatus, H1)

#define BIT_READ_1(reg_raw, mask_bit1) (((reg_raw) & (1 << mask_bit1)) >> mask_bit1)

#define BIT_ControlStatus_DEEPSLEEP(reg_raw) BIT_READ_1(reg_raw, 2)
#define BIT_ControlStatus_LD_TIMEOUT(reg_raw) BIT_READ_1(reg_raw, 1)
#define BIT_ControlStatus_LD_ON(reg_raw) BIT_READ_1(reg_raw, 0)
// Writing this command is used for legacy auto-detection, and it is not recommended for customers to write to it.
#define ControlStatus_DEEPSLEEP(reg_raw) (1 << 2)
#define ControlStatus_LD_TIMEOUT(reg_raw) (1 << 1)
#define ControlStatus_LD_ON(reg_raw) (1 << 0)

#define BIT_SafetyAlertA_SCD(reg_raw) BIT_READ_1(reg_raw, 7)
#define BIT_SafetyAlertA_OCD2(reg_raw) BIT_READ_1(reg_raw, 6)
#define BIT_SafetyAlertA_OCD1(reg_raw) BIT_READ_1(reg_raw, 5)
#define BIT_SafetyAlertA_OCC(reg_raw) BIT_READ_1(reg_raw, 4)
#define BIT_SafetyAlertA_COV(reg_raw) BIT_READ_1(reg_raw, 3)
#define BIT_SafetyAlertA_CUV(reg_raw) BIT_READ_1(reg_raw, 2)

#define BIT_SafetyStatusA_SCD(reg_raw) BIT_READ_1(reg_raw, 7)
#define BIT_SafetyStatusA_OCD2(reg_raw) BIT_READ_1(reg_raw, 6)
#define BIT_SafetyStatusA_OCD1(reg_raw) BIT_READ_1(reg_raw, 5)
#define BIT_SafetyStatusA_OCC(reg_raw) BIT_READ_1(reg_raw, 4)
#define BIT_SafetyStatusA_COV(reg_raw) BIT_READ_1(reg_raw, 3)
#define BIT_SafetyStatusA_CUV(reg_raw) BIT_READ_1(reg_raw, 2)

#define BIT_SafetyAlertB_OTF(reg_raw) BIT_READ_1(reg_raw, 7)
#define BIT_SafetyAlertB_OTINT(reg_raw) BIT_READ_1(reg_raw, 6)
#define BIT_SafetyAlertB_OTD(reg_raw) BIT_READ_1(reg_raw, 5)
#define BIT_SafetyAlertB_OTC(reg_raw) BIT_READ_1(reg_raw, 4)
#define BIT_SafetyAlertB_UTINT(reg_raw) BIT_READ_1(reg_raw, 2)
#define BIT_SafetyAlertB_UTD(reg_raw) BIT_READ_1(reg_raw, 1)
#define BIT_SafetyAlertB_UTC(reg_raw) BIT_READ_1(reg_raw, 0)

#define BIT_SafetyStatusB_OTF(reg_raw) BIT_READ_1(reg_raw, 7)
#define BIT_SafetyStatusB_OTINT(reg_raw) BIT_READ_1(reg_raw, 6)
#define BIT_SafetyStatusB_OTD(reg_raw) BIT_READ_1(reg_raw, 5)
#define BIT_SafetyStatusB_OTC(reg_raw) BIT_READ_1(reg_raw, 4)
#define BIT_SafetyStatusB_UTINT(reg_raw) BIT_READ_1(reg_raw, 2)
#define BIT_SafetyStatusB_UTD(reg_raw) BIT_READ_1(reg_raw, 1)
#define BIT_SafetyStatusB_UTC(reg_raw) BIT_READ_1(reg_raw, 0)

#define BIT_SafetyAlertC_OCD3(reg_raw) BIT_READ_1(reg_raw, 7)
#define BIT_SafetyAlertC_SCDL(reg_raw) BIT_READ_1(reg_raw, 6)
#define BIT_SafetyAlertC_OCDL(reg_raw) BIT_READ_1(reg_raw, 5)
#define BIT_SafetyAlertC_COVL(reg_raw) BIT_READ_1(reg_raw, 4)
#define BIT_SafetyAlertC_PTOS(reg_raw) BIT_READ_1(reg_raw, 3)

#define BIT_SafetyStatusC_OCD3(reg_raw) BIT_READ_1(reg_raw, 7)
#define BIT_SafetyStatusC_SCDL(reg_raw) BIT_READ_1(reg_raw, 6)
#define BIT_SafetyStatusC_OCDL(reg_raw) BIT_READ_1(reg_raw, 5)
#define BIT_SafetyStatusC_COVL(reg_raw) BIT_READ_1(reg_raw, 4)
#define BIT_SafetyStatusC_PTO(reg_raw) BIT_READ_1(reg_raw, 2)
#define BIT_SafetyStatusC_HWDF(reg_raw) BIT_READ_1(reg_raw, 1)

#define BIT_PFAlertA_CUDEP(reg_raw) BIT_READ_1(reg_raw, 7)
#define BIT_PFAlertA_SOTF(reg_raw) BIT_READ_1(reg_raw, 6)
#define BIT_PFAlertA_SOT(reg_raw) BIT_READ_1(reg_raw, 4)
#define BIT_PFAlertA_SOCD(reg_raw) BIT_READ_1(reg_raw, 3)
#define BIT_PFAlertA_SOCC(reg_raw) BIT_READ_1(reg_raw, 2)
#define BIT_PFAlertA_SOV(reg_raw) BIT_READ_1(reg_raw, 1)
#define BIT_PFAlertA_SUV(reg_raw) BIT_READ_1(reg_raw, 0)

#define BIT_PFStatusA_CUDEP(reg_raw) BIT_READ_1(reg_raw, 7)
#define BIT_PFStatusA_SOTF(reg_raw) BIT_READ_1(reg_raw, 6)
#define BIT_PFStatusA_SOT(reg_raw) BIT_READ_1(reg_raw, 4)
#define BIT_PFStatusA_SOCD(reg_raw) BIT_READ_1(reg_raw, 3)
#define BIT_PFStatusA_SOCC(reg_raw) BIT_READ_1(reg_raw, 2)
#define BIT_PFStatusA_SOV(reg_raw) BIT_READ_1(reg_raw, 1)
#define BIT_PFStatusA_SUV(reg_raw) BIT_READ_1(reg_raw, 0)

#define BIT_PFAlertB_SCDL(reg_raw) BIT_READ_1(reg_raw, 7)
#define BIT_PFAlertB_VIMA(reg_raw) BIT_READ_1(reg_raw, 4)
#define BIT_PFAlertB_VIMR(reg_raw) BIT_READ_1(reg_raw, 3)
#define BIT_PFAlertB_2LVL(reg_raw) BIT_READ_1(reg_raw, 2)
#define BIT_PFAlertB_DFETF(reg_raw) BIT_READ_1(reg_raw, 1)
#define BIT_PFAlertB_CFETF(reg_raw) BIT_READ_1(reg_raw, 0)

#define BIT_PFStatusB_SCDL(reg_raw) BIT_READ_1(reg_raw, 7)
#define BIT_PFStatusB_VIMA(reg_raw) BIT_READ_1(reg_raw, 4)
#define BIT_PFStatusB_VIMR(reg_raw) BIT_READ_1(reg_raw, 3)
#define BIT_PFStatusB_2LVL(reg_raw) BIT_READ_1(reg_raw, 2)
#define BIT_PFStatusB_DFETF(reg_raw) BIT_READ_1(reg_raw, 1)
#define BIT_PFStatusB_CFETF(reg_raw) BIT_READ_1(reg_raw, 0)

#define BIT_PFAlertC_HWMX(reg_raw) BIT_READ_1(reg_raw, 6)
#define BIT_PFAlertC_VSSF(reg_raw) BIT_READ_1(reg_raw, 5)
#define BIT_PFAlertC_VREF(reg_raw) BIT_READ_1(reg_raw, 4)
#define BIT_PFAlertC_LFOF(reg_raw) BIT_READ_1(reg_raw, 3)

#define BIT_PFStatusC_CMDF(reg_raw) BIT_READ_1(reg_raw, 7)
#define BIT_PFStatusC_HWMX(reg_raw) BIT_READ_1(reg_raw, 6)
#define BIT_PFStatusC_VSSF(reg_raw) BIT_READ_1(reg_raw, 5)
#define BIT_PFStatusC_VREF(reg_raw) BIT_READ_1(reg_raw, 4)
#define BIT_PFStatusC_LFOF(reg_raw) BIT_READ_1(reg_raw, 3)
#define BIT_PFStatusC_IRMF(reg_raw) BIT_READ_1(reg_raw, 2)
#define BIT_PFStatusC_DRMF(reg_raw) BIT_READ_1(reg_raw, 1)
#define BIT_PFStatusC_OTPF(reg_raw) BIT_READ_1(reg_raw, 0)

#define BIT_PFAlertD_TOSF(reg_raw) BIT_READ_1(reg_raw, 0)

#define BIT_PFStatusD_TOSF(reg_raw) BIT_READ_1(reg_raw, 0)

#define BIT_BatteryStatus_SLEEP(reg_raw) BIT_READ_1(reg_raw, 15)
#define BIT_BatteryStatus_SD_CMD(reg_raw) BIT_READ_1(reg_raw, 13)
#define BIT_BatteryStatus_PF(reg_raw) BIT_READ_1(reg_raw, 12)
#define BIT_BatteryStatus_SS(reg_raw) BIT_READ_1(reg_raw, 11)
#define BIT_BatteryStatus_FUSE(reg_raw) BIT_READ_1(reg_raw, 10)
#define BIT_BatteryStatus_SEC1(reg_raw) BIT_READ_1(reg_raw, 9) // TODO:
#define BIT_BatteryStatus_SEC0(reg_raw) BIT_READ_1(reg_raw, 8) // TODO:
#define BIT_BatteryStatus_OTPB(reg_raw) BIT_READ_1(reg_raw, 7)
#define BIT_BatteryStatus_OTPW(reg_raw) BIT_READ_1(reg_raw, 6)
#define BIT_BatteryStatus_COW_CHK(reg_raw) BIT_READ_1(reg_raw, 5)
#define BIT_BatteryStatus_WD(reg_raw) BIT_READ_1(reg_raw, 4)
#define BIT_BatteryStatus_POR(reg_raw) BIT_READ_1(reg_raw, 3)
#define BIT_BatteryStatus_SLEEP_EN(reg_raw) BIT_READ_1(reg_raw, 2)
#define BIT_BatteryStatus_PCHG_MODE(reg_raw) BIT_READ_1(reg_raw, 1)
#define BIT_BatteryStatus_CFGUPDATE(reg_raw) BIT_READ_1(reg_raw, 0)

#define BIT_AlarmStatus_SSBC(reg_raw) BIT_READ_1(reg_raw, 15)
#define BIT_AlarmStatus_SSA(reg_raw) BIT_READ_1(reg_raw, 14)
#define BIT_AlarmStatus_PF(reg_raw) BIT_READ_1(reg_raw, 13)
#define BIT_AlarmStatus_MSK_SFALERT(reg_raw) BIT_READ_1(reg_raw, 12)
#define BIT_AlarmStatus_MSK_PFALERT(reg_raw) BIT_READ_1(reg_raw, 11)
#define BIT_AlarmStatus_INITSTART(reg_raw) BIT_READ_1(reg_raw, 10)
#define BIT_AlarmStatus_INITCOMP(reg_raw) BIT_READ_1(reg_raw, 9)
#define BIT_AlarmStatus_FULLSCAN(reg_raw) BIT_READ_1(reg_raw, 7)
#define BIT_AlarmStatus_XCHG(reg_raw) BIT_READ_1(reg_raw, 6)
#define BIT_AlarmStatus_XDSG(reg_raw) BIT_READ_1(reg_raw, 5)
#define BIT_AlarmStatus_SHUTV(reg_raw) BIT_READ_1(reg_raw, 4)
#define BIT_AlarmStatus_FUSE(reg_raw) BIT_READ_1(reg_raw, 3)
#define BIT_AlarmStatus_CB(reg_raw) BIT_READ_1(reg_raw, 2)
#define BIT_AlarmStatus_ADSCAN(reg_raw) BIT_READ_1(reg_raw, 1)
#define BIT_AlarmStatus_WAKE(reg_raw) BIT_READ_1(reg_raw, 0)
//
#define AlarmStatus_SSBC_CLEAR (1 << 15)
#define AlarmStatus_SSA_CLEAR (1 << 14)
#define AlarmStatus_PF_CLEAR (1 << 13)
#define AlarmStatus_MSK_SFALERT_CLEAR (1 << 12)
#define AlarmStatus_MSK_PFALERT_CLEAR (1 << 11)
#define AlarmStatus_INITSTART_CLEAR (1 << 10)
#define AlarmStatus_INITCOMP_CLEAR (1 << 9)
#define AlarmStatus_XCHG_CLEAR (1 << 6)
#define AlarmStatus_FULLSCAN_CLEAR (1 << 7)
#define AlarmStatus_XDSG_CLEAR (1 << 5)
#define AlarmStatus_SHUTV_CLEAR (1 << 4)
#define AlarmStatus_FUSE_CLEAR (1 << 3)
#define AlarmStatus_CB_CLEAR (1 << 2)
#define AlarmStatus_ADSCAN_CLEAR (1 << 1)
#define AlarmStatus_WAKE_CLEAR (1 << 0)

#define BIT_AlarmRawStatus_SSBC(reg_raw) BIT_READ_1(reg_raw, 15)
#define BIT_AlarmRawStatus_SSA(reg_raw) BIT_READ_1(reg_raw, 14)
#define BIT_AlarmRawStatus_PF(reg_raw) BIT_READ_1(reg_raw, 13)
#define BIT_AlarmRawStatus_MSK_SFALERT(reg_raw) BIT_READ_1(reg_raw, 12)
#define BIT_AlarmRawStatus_MSK_PFALERT(reg_raw) BIT_READ_1(reg_raw, 11)
#define BIT_AlarmRawStatus_INITSTART(reg_raw) BIT_READ_1(reg_raw, 10)
#define BIT_AlarmRawStatus_INITCOMP(reg_raw) BIT_READ_1(reg_raw, 9)
#define BIT_AlarmRawStatus_FULLSCAN(reg_raw) BIT_READ_1(reg_raw, 7)
#define BIT_AlarmRawStatus_XCHG(reg_raw) BIT_READ_1(reg_raw, 6)
#define BIT_AlarmRawStatus_XDSG(reg_raw) BIT_READ_1(reg_raw, 5)
#define BIT_AlarmRawStatus_SHUTV(reg_raw) BIT_READ_1(reg_raw, 4)
#define BIT_AlarmRawStatus_FUSE(reg_raw) BIT_READ_1(reg_raw, 3)
#define BIT_AlarmRawStatus_CB(reg_raw) BIT_READ_1(reg_raw, 2)
#define BIT_AlarmRawStatus_ADSCAN(reg_raw) BIT_READ_1(reg_raw, 1)
#define BIT_AlarmRawStatus_WAKE(reg_raw) BIT_READ_1(reg_raw, 0)

#define BIT_AlarmEnable_SSBC(reg_raw) BIT_READ_1(reg_raw, 15)
#define BIT_AlarmEnable_SSA(reg_raw) BIT_READ_1(reg_raw, 14)
#define BIT_AlarmEnable_PF(reg_raw) BIT_READ_1(reg_raw, 13)
#define BIT_AlarmEnable_MSK_SFALERT(reg_raw) BIT_READ_1(reg_raw, 12)
#define BIT_AlarmEnable_MSK_PFALERT(reg_raw) BIT_READ_1(reg_raw, 11)
#define BIT_AlarmEnable_INITSTART(reg_raw) BIT_READ_1(reg_raw, 10)
#define BIT_AlarmEnable_INITCOMP(reg_raw) BIT_READ_1(reg_raw, 9)
#define BIT_AlarmEnable_FULLSCAN(reg_raw) BIT_READ_1(reg_raw, 7)
#define BIT_AlarmEnable_XCHG(reg_raw) BIT_READ_1(reg_raw, 6)
#define BIT_AlarmEnable_XDSG(reg_raw) BIT_READ_1(reg_raw, 5)
#define BIT_AlarmEnable_SHUTV(reg_raw) BIT_READ_1(reg_raw, 4)
#define BIT_AlarmEnable_FUSE(reg_raw) BIT_READ_1(reg_raw, 3)
#define BIT_AlarmEnable_CB(reg_raw) BIT_READ_1(reg_raw, 2)
#define BIT_AlarmEnable_ADSCAN(reg_raw) BIT_READ_1(reg_raw, 1)
#define BIT_AlarmEnable_WAKE(reg_raw) BIT_READ_1(reg_raw, 0)
//
#define AlarmEnable_SSBC (1 << 15)
#define AlarmEnable_SSA (1 << 14)
#define AlarmEnable_PF (1 << 13)
#define AlarmEnable_MSK_SFALERT (1 << 12)
#define AlarmEnable_MSK_PFALERT (1 << 11)
#define AlarmEnable_INITSTART (1 << 10)
#define AlarmEnable_INITCOMP (1 << 9)
#define AlarmEnable_FULLSCAN (1 << 7)
#define AlarmEnable_XCHG (1 << 6)
#define AlarmEnable_XDSG (1 << 5)
#define AlarmEnable_SHUTV (1 << 4)
#define AlarmEnable_FUSE (1 << 3)
#define AlarmEnable_CB (1 << 2)
#define AlarmEnable_ADSCAN (1 << 1)
#define AlarmEnable_WAKE (1 << 0)

#define BIT_FETStatus_ALRT_PIN(reg_raw) BIT_READ_1(reg_raw, 6)
#define BIT_FETStatus_DDSG_PIN(reg_raw) BIT_READ_1(reg_raw, 5)
#define BIT_FETStatus_DCHG_PIN(reg_raw) BIT_READ_1(reg_raw, 4)
#define BIT_FETStatus_PDSG_FET(reg_raw) BIT_READ_1(reg_raw, 3)
#define BIT_FETStatus_DSG_FET(reg_raw) BIT_READ_1(reg_raw, 2)
#define BIT_FETStatus_PCHG_FET(reg_raw) BIT_READ_1(reg_raw, 1)
#define BIT_FETStatus_CHG_FET(reg_raw) BIT_READ_1(reg_raw, 0)

// Subcommands
#define DEVICE_NUMBER 0x0001
#define FW_VERSION 0x0002
#define HW_VERSION 0x0003
#define IROM_SIG 0x0004
#define STATIC_CFG_SIG 0x0005
#define PREV_MACWRITE 0x0007
#define DROM_SIG 0x0009
#define SECURITY_KEYS 0x0035
#define SAVED_PF_STATUS 0x0053
#define MANUFACTURINGSTATUS 0x0057
#define MANU_DATA 0x0070
#define DASTATUS1 0x0071
#define DASTATUS2 0x0072
#define DASTATUS3 0x0073
#define DASTATUS4 0x0074
#define DASTATUS5 0x0075
#define DASTATUS6 0x0076
#define DASTATUS7 0x0077
#define CUV_SNAPSHOT 0x0080
#define COV_SNAPSHOT 0X0081
#define CB_ACTIVE_CELLS 0x0083
#define CBSTATUS1 0x0085
#define CB_SET_LVL 0x0084
#define CBSTATUS2 0x0086
#define CBSTATUS3 0x0087
#define FET_CONTROL 0x0097
#define REG12_CONTROL 0x0098
#define OTP_WR_CHECK 0x00A0
#define OTP_WRITE 0x00A1
#define READ_CAL1 0xF081
#define CAL_CUV 0xF090
#define CAL_COV 0xF091

#define DEVICE_NUMBER_TIME 400
#define FW_VERSION_TIME 400
#define HW_VERSION_TIME 400
#define IROM_SIG_TIME 8500
#define STATIC_CFG_SIG_TIME 450
#define PREV_MACWRITE_TIME 8500 /*TI INTERNAL - UNKNOWN DELAY*/
#define DROM_SIG_TIME 650
#define SECURITY_KEYS_TIME 8500 /*UNKNOWN DELAY*/
#define SAVED_PF_STATUS_TIME 500
#define MANUFACTURINGSTATUS_TIME 605
#define MANU_DATA_TIME 660
#define DASTATUS1_TIME 660
#define DASTATUS2_TIME 660
#define DASTATUS3_TIME 660
#define DASTATUS4_TIME 660
#define DASTATUS5_TIME 660
#define DASTATUS6_TIME 660
#define DASTATUS7_TIME 660
#define CUV_SNAPSHOT_TIME 660
#define COV_SNAPSHOT_TIME 660
#define CB_ACTIVE_CELLS_TIME 560
#define CBSTATUS1_TIME 575
#define CB_SET_LVL_TIME 480
#define CBSTATUS2_TIME 575
#define CBSTATUS3_TIME 575
#define FET_CONTROL_TIME 495
#define REG12_CONTROL_TIME 450
#define OTP_WR_CHECK_TIME 580
#define OTP_WRITE_TIME 8500 /*UNKNOWN DELAY*/
#define READ_CAL1_TIME 630
#define CAL_CUV_TIME 8500 /*UNKNOWN DELAY*/
#define CAL_COV_TIME 8500 /*UNKNOWN DELAY*/

// Command Only Subcommands
#define CMD_DELAY_DEFAULT 1000
#define CMD_SUBCMD_EXIT_DEEPSLEEP() bq769x2_command_subcommands(0x000E, 500)
#define CMD_SUBCMD_DEEPSLEEP() bq769x2_command_subcommands(0x000F, 500)
#define CMD_SUBCMD_SHUTDOWN() bq769x2_command_subcommands(0x0010, 500)
#define CMD_SUBCMD_RESET() bq769x2_command_subcommands(0x0012, CMD_DELAY_DEFAULT)
#define CMD_SUBCMD_PDSGTEST() bq769x2_command_subcommands(0x001C, 550)
#define CMD_SUBCMD_FUSE_TOGGLE() bq769x2_command_subcommands(0x001D, 500)
#define CMD_SUBCMD_PCHGTEST() bq769x2_command_subcommands(0x001E, 550)
#define CMD_SUBCMD_CHGTEST() bq769x2_command_subcommands(0x001F, 550)
#define CMD_SUBCMD_DSGTEST() bq769x2_command_subcommands(0x0020, 550)
#define CMD_SUBCMD_FET_ENABLE() bq769x2_command_subcommands(0x0022, 550)
#define CMD_SUBCMD_PF_ENABLE() bq769x2_command_subcommands(0x0024, 550)
#define CMD_SUBCMD_PF_RESET() bq769x2_command_subcommands(0x0029, CMD_DELAY_DEFAULT)
#define CMD_SUBCMD_SEAL() bq769x2_command_subcommands(0x0030, 550)
#define CMD_SUBCMD_RESET_PASSQ() bq769x2_command_subcommands(0x0082, 600)
#define CMD_SUBCMD_PTO_RECOVER() bq769x2_command_subcommands(0x008A, 500)
#define CMD_SUBCMD_SET_CFGUPDATE() bq769x2_command_subcommands(0x0090, 2000)
#define CMD_SUBCMD_EXIT_CFGUPDATE() bq769x2_command_subcommands(0x0092, 1000)
#define CMD_SUBCMD_DSG_PDSG_OFF() bq769x2_command_subcommands(0x0093, 550)
#define CMD_SUBCMD_CHG_PCHG_OFF() bq769x2_command_subcommands(0x0094, 550)
#define CMD_SUBCMD_ALL_FETS_OFF() bq769x2_command_subcommands(0x0095, 550)
#define CMD_SUBCMD_ALL_FETS_ON() bq769x2_command_subcommands(0x0096, 500)
#define CMD_SUBCMD_SLEEP_ENABLE() bq769x2_command_subcommands(0x0099, 500)
#define CMD_SUBCMD_SLEEP_DISABLE() bq769x2_command_subcommands(0x009A, 500)
#define CMD_SUBCMD_OCDL_RECOVER() bq769x2_command_subcommands(0x009B, 500)
#define CMD_SUBCMD_SCDL_RECOVER() bq769x2_command_subcommands(0x009C, 500)
#define CMD_SUBCMD_LOAD_DETECT_RESTART() bq769x2_command_subcommands(0x009D, 500)
#define CMD_SUBCMD_LOAD_DETECT_ON() bq769x2_command_subcommands(0x009E, 500)
#define CMD_SUBCMD_LOAD_DETECT_OFF() bq769x2_command_subcommands(0x009F, 500)
#define CMD_SUBCMD_CFETOFF_LO() bq769x2_command_subcommands(0x2800, 500)
#define CMD_SUBCMD_DFETOFF_LO() bq769x2_command_subcommands(0x2801, 500)
#define CMD_SUBCMD_ALERT_LO() bq769x2_command_subcommands(0x2802, 500)
#define CMD_SUBCMD_HDQ_LO() bq769x2_command_subcommands(0x2806, 500)
#define CMD_SUBCMD_DCHG_LO() bq769x2_command_subcommands(0x2807, 500)
#define CMD_SUBCMD_DDSG_LO() bq769x2_command_subcommands(0x2808, 500)
#define CMD_SUBCMD_CFETOFF_HI() bq769x2_command_subcommands(0x2810, 500)
#define CMD_SUBCMD_DFETOFF_HI() bq769x2_command_subcommands(0x2811, 500)
#define CMD_SUBCMD_ALERT_HI() bq769x2_command_subcommands(0x2812, 500)
#define CMD_SUBCMD_HDQ_HI() bq769x2_command_subcommands(0x2816, 500)
#define CMD_SUBCMD_DCHG_HI() bq769x2_command_subcommands(0x2817, 500)
#define CMD_SUBCMD_DDSG_HI() bq769x2_command_subcommands(0x2818, 500)
#define CMD_SUBCMD_PF_FORCE_A() bq769x2_command_subcommands(0x2857, 500)
#define CMD_SUBCMD_PF_FORCE_B() bq769x2_command_subcommands(0x29A3, 800)
#define CMD_SUBCMD_SWAP_COMM_MODE() bq769x2_command_subcommands(0x29BC, 500)
#define CMD_SUBCMD_SWAP_TO_I2C() bq769x2_command_subcommands(0x29E7, 500)
#define CMD_SUBCMD_SWAP_TO_SPI() bq769x2_command_subcommands(0x7C35, 500)
#define CMD_SUBCMD_SWAP_TO_HDQ() bq769x2_command_subcommands(0x7C40, 500)

// 13.3 Settings
// 13.3.2 Settings:Configuration

#define I2CAddress_DEFAULT_0x10 (0x00)

// 13.3.2.1 Settings:Configuration:Power Config
#define PowerConfig_DPSLP_OT (1 << 13)
#define PowerConfig_SHUT_TS2 (1 << 12)
#define PowerConfig_DPSLP_PD (1 << 11)
#define PowerConfig_DPSLP_LDO (1 << 10)
#define PowerConfig_DPSLP_LFO (1 << 9)
#define PowerConfig_SLEEP (1 << 8)
#define PowerConfig_OTSD (1 << 7)

#define PowerConfig_FASTADC_1ms5 (1 << 6)
#define PowerConfig_FASTADC_3ms0 (0 << 6)

#define PowerConfig_CB_LOOP_SLOW_FULL_SPD (0 << 4)
#define PowerConfig_CB_LOOP_SLOW_HALF_SPD (1 << 4)
#define PowerConfig_CB_LOOP_SLOW_QUARTER_SPD (2 << 4)
#define PowerConfig_CB_LOOP_SLOW_EIGTH_SPD (3 << 4)

#define PowerConfig_LOOP_SLOW_63ms (0 << 2)
#define PowerConfig_LOOP_SLOW_126ms (1 << 2)
#define PowerConfig_LOOP_SLOW_252ms (2 << 2)
#define PowerConfig_LOOP_SLOW_504ms (3 << 2)

#define PowerConfig_WK_SPD_48ms (0 << 0)
#define PowerConfig_WK_SPD_24ms (1 << 0)
#define PowerConfig_WK_SPD_12ms (2 << 0)
#define PowerConfig_WK_SPD_6ms (3 << 0)

// 13.3.2.2 Settings:Configuration:REG12 Config
#define REG12Config_REG2V_1V8 (3 << 5)
#define REG12Config_REG2V_2V5 (4 << 5)
#define REG12Config_REG2V_3V0 (5 << 5)
#define REG12Config_REG2V_3V3 (6 << 5)
#define REG12Config_REG2V_5V0 (7 << 5)
#define REG12Config_REG2_EN (1 << 4)
#define REG12Config_REG1V_1V8 (3 << 1)
#define REG12Config_REG1V_2V5 (4 << 1)
#define REG12Config_REG1V_3V0 (5 << 1)
#define REG12Config_REG1V_3V3 (6 << 1)
#define REG12Config_REG1V_5V0 (7 << 1)
#define REG12Config_REG1_EN (1 << 0)

// 13.3.2.3 Settings:Configuration:REG0 Config
#define REG0Config_REG0_EN (1 << 0)

// 13.3.2.4 Settings:Configuration:HWD Regulator Options
#define HWDRegulatorOptions_TOGGLE_TIME_OFF (0 << 0)
#define HWDRegulatorOptions_TOGGLE_TIME_1s (1 << 0)
#define HWDRegulatorOptions_TOGGLE_TIME_2s (2 << 0)
#define HWDRegulatorOptions_TOGGLE_TIME_3s (3 << 0)
#define HWDRegulatorOptions_TOGGLE_TIME_4s (4 << 0)
#define HWDRegulatorOptions_TOGGLE_TIME_5s (5 << 0)
#define HWDRegulatorOptions_TOGGLE_TIME_6s (6 << 0)
#define HWDRegulatorOptions_TOGGLE_TIME_7s (7 << 0)
#define HWDRegulatorOptions_TOGGLE_TIME_8s (8 << 0)
#define HWDRegulatorOptions_TOGGLE_TIME_9s (9 << 0)
#define HWDRegulatorOptions_TOGGLE_TIME_10s (10 << 0)
#define HWDRegulatorOptions_TOGGLE_TIME_11s (11 << 0)
#define HWDRegulatorOptions_TOGGLE_TIME_12s (12 << 0)
#define HWDRegulatorOptions_TOGGLE_TIME_13s (13 << 0)
#define HWDRegulatorOptions_TOGGLE_TIME_14s (14 << 0)
#define HWDRegulatorOptions_TOGGLE_TIME_15s (15 << 0)

#define HWDRegulatorOptions_TOGGLE_OPT_NOP (0 << 4)
#define HWDRegulatorOptions_TOGGLE_OPT_REG12_OFF (1 << 4)
#define HWDRegulatorOptions_TOGGLE_OPT_REG12_OFFON (2 << 4)

// 13.3.2.5 Settings:Configuration:Comm Type
#define CommType_HDQ_ALERT_PIN (0x03)
#define CommType_HDQ_HDQ_PIN (0x04)
#define CommType_I2C (0x07)
#define CommType_I2C_FAST (0x08)
#define CommType_I2C_FAST_TIMEOUT (0x09)
#define CommType_SPI (0x0F)
#define CommType_SPI_CRC (0x10)
#define CommType_I2C_CRC (0x11)
#define CommType_I2C_FAST_CRC (0x12)
#define CommType_I2C_TIMEOUT (0x1E)

// 13.3.2.7 Settings:Configuration:SPI Configuration
#define SPIConfiguration_MISO_REG1_1V8 (0 << 6)
#define SPIConfiguration_MISO_REG1_REG1 (1 << 6)

#define SPIConfiguration_FILT_EN (1 << 5)

// IO
// 13.3.2.9 Settings:Configuration:CFETOFF Pin Config
// 13.3.2.10 Settings:Configuration:DFETOFF Pin Config
// 13.3.2.11 Settings:Configuration:ALERT Pin Config
// 13.3.2.12 Settings:Configuration:TS1 Config
// 13.3.2.13 Settings:Configuration:TS2 Config
// 13.3.2.14 Settings:Configuration:TS3 Config
// 13.3.2.15 Settings:Configuration:HDQ Pin Config
// 13.3.2.16 Settings:Configuration:DCHG Pin Config
// 13.3.2.17 Settings:Configuration:DDSG Pin Config

// PIN FXN ADCIN/THERMISTOR
#define PIN_ANALOG_TYPE_ADC (0 << 2)
#define PIN_ANALOG_TYPE_CELL_PROT (1 << 2)
#define PIN_ANALOG_TYPE_REPORT (2 << 2)
#define PIN_ANALOG_TYPE_FET_PROT (3 << 2)

#define PIN_ANALOG_POLY_PU18K_NTC10K (0 << 4)
#define PIN_ANALOG_POLY_PU180K_NTC200K (1 << 4)
#define PIN_ANALOG_POLY_CUSTOM_NTC (2 << 4)
#define PIN_ANALOG_POLY_RAW (3 << 4)

#define PIN_ANALOG_PU_18K_NTC10K (0 << 6)
#define PIN_ANALOG_PU_180K_NTC200K (1 << 6)
#define PIN_ANALOG_PU_NONE (2 << 6)

// PIN FXN ALT/GPO
#define PIN_DIGITAL_0_WPD_ON (1 << 2)
#define PIN_DIGITAL_0_WPD_OFF (0 << 2)

#define PIN_DIGITAL_1_PUSH_PULL (1 << 3)
#define PIN_DIGITAL_1_TRISTATE_HIGHZ (0 << 3)

#define PIN_DIGITAL_2_WPU_REG1_ON (1 << 4)
#define PIN_DIGITAL_2_WPU_REG1_OFF (0 << 4)

#define PIN_DIGITAL_3_DRIVE_HIGH_REG1 (1 << 5)
#define PIN_DIGITAL_3_DRIVE_HIGH_REG18 (0 << 5)

#define PIN_DIGITAL_5_ACTIVE_LOW (1 << 7)
#define PIN_DIGITAL_5_ACTIVE_HIGH (0 << 7)

// 13.3.2.9 Settings:Configuration:CFETOFF Pin Config
#define CFETOFFPinConfig_FXN_SPI_CS_UNUSED (0 << 0)
#define CFETOFFPinConfig_FXN_GPO (1 << 0)
#define CFETOFFPinConfig_FXN_CFETOFF (2 << 0)
#define CFETOFFPinConfig_FXN_ADC_THERMISTOR (3 << 0)

// 13.3.2.10 Settings:Configuration:DFETOFF Pin Config
#define DFETOFFPinConfig_FXN_UNUSED (0 << 0)
#define DFETOFFPinConfig_FXN_GPO (1 << 0)
#define DFETOFFPinConfig_FXN_DFETOFF_BOTHOFF (2 << 0)
#define DFETOFFPinConfig_FXN_ADC_THERMISTOR (3 << 0)
#define DFETOFFPinConfig_DIGITAL_4_BOTHOFF (1 << 6)
#define DFETOFFPinConfig_DIGITAL_4_DFETOFF (0 << 6)

// 13.3.2.11 Settings:Configuration:ALERT Pin Config
#define ALERTPinConfig_FXN_HDQ_UNUSED (0 << 0)
#define ALERTPinConfig_FXN_GPO (1 << 0)
#define ALERTPinConfig_FXN_ALERT (2 << 0)
#define ALERTPinConfig_FXN_ADC_THERMISTOR (3 << 0)

// 13.3.2.12 Settings:Configuration:TS1 Config
#define TS1Config_FXN_UNUSED (0 << 0)
#define TS1Config_FXN_UNUSED1 (1 << 0)
#define TS1Config_FXN_UNUSED2 (2 << 0)
#define TS1Config_FXN_ADC_THERMISTOR (3 << 0)

// 13.3.2.13 Settings:Configuration:TS2 Config
#define TS2Config_FXN_UNUSED (0 << 0)
#define TS2Config_FXN_UNUSED1 (1 << 0)
#define TS2Config_FXN_UNUSED2 (2 << 0)
#define TS2Config_FXN_ADC_THERMISTOR (3 << 0)

// 13.3.2.14 Settings:Configuration:TS3 Config
#define TS3Config_FXN_UNUSED (0 << 0)
#define TS3Config_FXN_UNUSED1 (1 << 0)
#define TS3Config_FXN_UNUSED2 (2 << 0)
#define TS3Config_FXN_ADC_THERMISTOR (3 << 0)

// 13.3.2.15 Settings:Configuration:HDQ Pin Config
#define HDQPinConfig_FXN_HDQ_SPIMOSI_UNUSED (0 << 0)
#define HDQPinConfig_FXN_GPO (1 << 0)
#define HDQPinConfig_FXN_UNUSED1 (2 << 0)
#define HDQPinConfig_FXN_ADC_THERMISTOR (3 << 0)

// 13.3.2.16 Settings:Configuration:DCHG Pin Config
#define DCHGPinConfig_FXN_UNUSED (0 << 0)
#define DCHGPinConfig_FXN_GPO (1 << 0)
#define DCHGPinConfig_FXN_DCHG (2 << 0)
#define DCHGPinConfig_FXN_ADC_THERMISTOR (3 << 0)

// 13.3.2.17 Settings:Configuration:DDSG Pin Config
#define DDSGPinConfig_FXN_UNUSED (0 << 0)
#define DDSGPinConfig_FXN_GPO (1 << 0)
#define DDSGPinConfig_FXN_DDSG (2 << 0)
#define DDSGPinConfig_FXN_ADC_THERMISTOR (3 << 0)

// 13.3.2.18 Settings:Configuration:DA Configuration
#define DAConfiguration_TINT_FETT (1 << 4)
#define DAConfiguration_TINT_EN (1 << 3)
#define DAConfiguration_USER_VOLTS_mV_1mV (0 << 2)
#define DAConfiguration_USER_VOLTS_cV_10mV (1 << 2)
#define DAConfiguration_USER_AMPS_dmA_0mA1 (0 << 0)
#define DAConfiguration_USER_AMPS_mA_1mA (1 << 0)
#define DAConfiguration_USER_AMPS_cA_10mA (2 << 0)
#define DAConfiguration_USER_AMPS_dA_100mA (3 << 0)

// 13.3.2.19 Settings:Configuration:Vcell Mode
#define VCellMode_1_EN (1 << 0)
#define VCellMode_2_EN (1 << 1)
#define VCellMode_3_EN (1 << 2)
#define VCellMode_4_EN (1 << 3)
#define VCellMode_5_EN (1 << 4)
#define VCellMode_6_EN (1 << 5)
#define VCellMode_7_EN (1 << 6)
#define VCellMode_8_EN (1 << 7)
#define VCellMode_9_EN (1 << 8)
#define VCellMode_10_EN (1 << 9)
#define VCellMode_11_EN (1 << 10)
#define VCellMode_12_EN (1 << 11)
#define VCellMode_13_EN (1 << 12)
#define VCellMode_14_EN (1 << 13)
#define VCellMode_15_EN (1 << 14)
#define VCellMode_16_EN (1 << 15)
#define VCellMode_ALL (VCellMode_1_EN | VCellMode_2_EN | VCellMode_3_EN | VCellMode_4_EN | VCellMode_5_EN | VCellMode_6_EN | VCellMode_7_EN | VCellMode_8_EN | VCellMode_9_EN | VCellMode_10_EN | VCellMode_11_EN | VCellMode_12_EN | VCellMode_13_EN | VCellMode_14_EN | VCellMode_15_EN | VCellMode_16_EN)

// 13.3.3 Settings:Protection
// 13.3.3.1 Settings:Protection:Protection Configuration
#define ProtectionConfiguration_SCDL_CURR_RECOV (1 << 10)
#define ProtectionConfiguration_OCDL_CURR_RECOV (1 << 9)
#define ProtectionConfiguration_FETF_FUSE (1 << 8)
#define ProtectionConfiguration_PACK_FUSE (1 << 7)
#define ProtectionConfiguration_PF_OTP (1 << 5)
#define ProtectionConfiguration_PF_FUSE (1 << 4)
#define ProtectionConfiguration_PF_DPSLP (1 << 3)
#define ProtectionConfiguration_PF_REGS (1 << 2)
#define ProtectionConfiguration_PF_FETS (1 << 1)

// 13.3.3.2 Settings:Protection:Enabled Protections A
#define EnabledProtectionsA_SCD (1 << 7)
#define EnabledProtectionsA_OCD2 (1 << 6)
#define EnabledProtectionsA_OCD1 (1 << 5)
#define EnabledProtectionsA_OCC (1 << 4)
#define EnabledProtectionsA_COV (1 << 3)
#define EnabledProtectionsA_CUV (1 << 2)

// 13.3.3.3 Settings:Protection:Enabled Protections B
#define EnabledProtectionsB_OTF (1 << 7)
#define EnabledProtectionsB_OTINT (1 << 6)
#define EnabledProtectionsB_OTD (1 << 5)
#define EnabledProtectionsB_OTC (1 << 4)
#define EnabledProtectionsB_UTINT (1 << 2)
#define EnabledProtectionsB_UTD (1 << 1)
#define EnabledProtectionsB_UTC (1 << 0)

// 13.3.3.4 Settings:Protection:Enabled Protections C
#define EnabledProtectionsC_OCD3 (1 << 7)
#define EnabledProtectionsC_SCDL (1 << 6)
#define EnabledProtectionsC_OCDL (1 << 5)
#define EnabledProtectionsC_COVL (1 << 4)
#define EnabledProtectionsC_PTO (1 << 2)
#define EnabledProtectionsC_HWDF (1 << 1)

// 13.3.3.5 Settings:Protection:CHG FET Protections A
#define CHGFETProtectionsA_SCD (1 << 7)
#define CHGFETProtectionsA_OCC (1 << 4)
#define CHGFETProtectionsA_COV (1 << 3)

// 13.3.3.6 Settings:Protection:CHG FET Protections B
#define CHGFETProtectionsB_OTF (1 << 7)
#define CHGFETProtectionsB_OTINT (1 << 6)
#define CHGFETProtectionsB_OTC (1 << 4)
#define CHGFETProtectionsB_UTINT (1 << 2)
#define CHGFETProtectionsB_UTC (1 << 0)

// 13.3.3.7 Settings:Protection:CHG FET Protections C
#define CHGFETProtectionsC_SCDL (1 << 6)
#define CHGFETProtectionsC_COVL (1 << 4)
#define CHGFETProtectionsC_PTO (1 << 2)
#define CHGFETProtectionsC_HWDF (1 << 1)

// 13.3.3.8 Settings:Protection:DSG FET Protections A
#define DSGFETProtectionsA_SCD (1 << 7)
#define DSGFETProtectionsA_OCD2 (1 << 6)
#define DSGFETProtectionsA_OCD1 (1 << 5)
#define DSGFETProtectionsA_CUV (1 << 2)

// 13.3.3.9 Settings:Protection:DSG FET Protections B
#define DSGFETProtectionsB_OTF (1 << 7)
#define DSGFETProtectionsB_OTINT (1 << 6)
#define DSGFETProtectionsB_OTD (1 << 5)
#define DSGFETProtectionsB_UTINT (1 << 2)
#define DSGFETProtectionsB_UTD (1 << 1)

// 13.3.3.10 Settings:Protection:DSG FET Protections C
#define DSGFETProtectionsC_OCD3 (1 << 7)
#define DSGFETProtectionsC_SCDL (1 << 6)
#define DSGFETProtectionsC_OCDL (1 << 5)
#define DSGFETProtectionsC_HWDF (1 << 1)

// 13.3.4 Settings:Alarm
// 13.3.4.1 Settings:Alarm:Default Alarm Mask
#define DefaultAlarmMask_SSBC (1 << 15)
#define DefaultAlarmMask_SSA (1 << 14)
#define DefaultAlarmMask_PF (1 << 13)
#define DefaultAlarmMask_MSK_SFALERT (1 << 12)
#define DefaultAlarmMask_MSK_PFALERT (1 << 11)
#define DefaultAlarmMask_INITSTART (1 << 10)
#define DefaultAlarmMask_INITCOMP (1 << 9)
#define DefaultAlarmMask_FULLSCAN (1 << 7)
#define DefaultAlarmMask_XCHG (1 << 6)
#define DefaultAlarmMask_XDSG (1 << 5)
#define DefaultAlarmMask_SHUTV (1 << 4)
#define DefaultAlarmMask_FUSE (1 << 3)
#define DefaultAlarmMask_CB (1 << 2)
#define DefaultAlarmMask_ADSCAN (1 << 1)
#define DefaultAlarmMask_WAKE (1 << 0)

// 13.3.4.2 Settings:Alarm:SF Alert Mask A
#define SFAlertMaskA_SCD (1 << 7)
#define SFAlertMaskA_OCD2 (1 << 6)
#define SFAlertMaskA_OCD1 (1 << 5)
#define SFAlertMaskA_OCC (1 << 4)
#define SFAlertMaskA_COV (1 << 3)
#define SFAlertMaskA_CUV (1 << 2)

// 13.3.4.3 Settings:Alarm:SF Alert Mask B
#define SFAlertMaskB_OTF (1 << 7)
#define SFAlertMaskB_OTINT (1 << 6)
#define SFAlertMaskB_OTD (1 << 5)
#define SFAlertMaskB_OTC (1 << 4)
#define SFAlertMaskB_UTINT (1 << 2)
#define SFAlertMaskB_UTD (1 << 1)
#define SFAlertMaskB_UTC (1 << 0)

// 13.3.4.4 Settings:Alarm:SF Alert Mask C
#define SFAlertMaskC_OCD3 (1 << 7)
#define SFAlertMaskC_SCDL (1 << 6)
#define SFAlertMaskC_OCDL (1 << 5)
#define SFAlertMaskC_COVL (1 << 4)
#define SFAlertMaskC_PTO (1 << 2)

// 13.3.4.5 Settings:Alarm:PF Alert Mask A
#define PFAlertMaskA_CUDEP (1 << 7)
#define PFAlertMaskA_SOTF (1 << 6)
#define PFAlertMaskA_SOT (1 << 4)
#define PFAlertMaskA_SOCD (1 << 3)
#define PFAlertMaskA_SOCC (1 << 2)
#define PFAlertMaskA_SOV (1 << 1)
#define PFAlertMaskA_SUV (1 << 0)

// 13.3.4.6 Settings:Alarm:PF Alert Mask B
#define PFAlertMaskB_SCDL (1 << 7)
#define PFAlertMaskB_VIMA (1 << 4)
#define PFAlertMaskB_VIMR (1 << 3)
#define PFAlertMaskB_2LVL (1 << 2)
#define PFAlertMaskB_DFETF (1 << 1)
#define PFAlertMaskB_CFETF (1 << 0)

// 13.3.4.7 Settings:Alarm:PF Alert Mask C
#define PFAlertMaskC_HWMX (1 << 6)
#define PFAlertMaskC_VSSF (1 << 5)
#define PFAlertMaskC_VREF (1 << 4)
#define PFAlertMaskC_LFOF (1 << 3)

// 13.3.4.8 Settings:Alarm:PF Alert Mask D
#define PFAlertMaskD_TOSF (1 << 0)

// 13.3.5 Settings:Permanent Failure
// 13.3.5.1 Settings:Permanent Failure:Enabled PF A
#define EnabledPFA_CUDEP (1 << 7)
#define EnabledPFA_SOTF (1 << 6)
#define EnabledPFA_SOT (1 << 4)
#define EnabledPFA_SOCD (1 << 3)
#define EnabledPFA_SOCC (1 << 2)
#define EnabledPFA_SOV (1 << 1)
#define EnabledPFA_SUV (1 << 0)

// 13.3.5.2 Settings:Permanent Failure:Enabled PF B
#define EnabledPFB_SCDL (1 << 7)
#define EnabledPFB_VIMA (1 << 4)
#define EnabledPFB_VIMR (1 << 3)
#define EnabledPFB_2LVL (1 << 2)
#define EnabledPFB_DFETF (1 << 1)
#define EnabledPFB_CFETF (1 << 0)

// 13.3.5.3 Settings:Permanent Failure:Enabled PF C
#define EnabledPFC_CMDF (1 << 7)
#define EnabledPFC_HWMX (1 << 6)
#define EnabledPFC_VSSF (1 << 5)
#define EnabledPFC_VREF (1 << 4)
#define EnabledPFC_LFOF (1 << 3)
#define EnabledPFC_IRMF (1 << 2)
#define EnabledPFC_DRMF (1 << 1)
#define EnabledPFC_OTPF (1 << 0)

// 13.3.5.4 Settings:Permanent Failure:Enabled PF D
#define EnabledPFD_TOSF (1 << 0)

// 13.3.6 Settings:FET
// 13.3.6.1 Settings:FET:FET Options
#define FETOptions_FET_INIT_OFF (1 << 5)
#define FETOptions_PDSG_EN (1 << 4)
#define FETOptions_FET_CTRL_EN (1 << 3)
#define FETOptions_HOST_FET_EN (1 << 2)
#define FETOptions_SLEEPCHG (1 << 1)
#define FETOptions_SFET (1 << 0)

// 13.3.6.2 Settings:FET:Chg Pump Control
#define ChgPumpControl_SFMODE_SLEEP (1 << 2)
#define ChgPumpControl_LVEN (1 << 1)
#define ChgPumpControl_CPEN (1 << 0)

// 13.3.10 Settings:Manufacturing
// 13.3.10.1 Settings:Manufacturing:Mfg Status Init
#define MfgStatusInit_OTPW_EN (1 << 7)
#define MfgStatusInit_PF_EN (1 << 6)
#define MfgStatusInit_FET_EN (1 << 4)

// 13.3.11 Settings:Cell Balancing Config
// 13.3.11.1 Settings:Cell Balancing Config:Balancing Configuration
#define BalancingConfiguration_CB_NO_CMD (1 << 4)
#define BalancingConfiguration_CB_NOSLEEP (1 << 3)
#define BalancingConfiguration_CB_SLEEP (1 << 2)
#define BalancingConfiguration_CB_RLX (1 << 1)
#define BalancingConfiguration_CB_CHG (1 << 0)

// 13.6.7 Protections:SCD
// 13.6.7.1 Protections:SCD:Threshold
#define SCDThreshold_10mV (0)
#define SCDThreshold_20mV (1)
#define SCDThreshold_40mV (2)
#define SCDThreshold_60mV (3)
#define SCDThreshold_80mV (4)
#define SCDThreshold_100mV (5)
#define SCDThreshold_125mV (6)
#define SCDThreshold_150mV (7)
#define SCDThreshold_175mV (8)
#define SCDThreshold_200mV (9)
#define SCDThreshold_250mV (10)
#define SCDThreshold_300mV (11)
#define SCDThreshold_350mV (12)
#define SCDThreshold_400mV (13)
#define SCDThreshold_450mV (14)
#define SCDThreshold_500mV (15)

// AUTO-GENERATED FROM TABLE.CSV
#define Cell1Gain_T I2
#define Cell2Gain_T I2
#define Cell3Gain_T I2
#define Cell4Gain_T I2
#define Cell5Gain_T I2
#define Cell6Gain_T I2
#define Cell7Gain_T I2
#define Cell8Gain_T I2
#define Cell9Gain_T I2
#define Cell10Gain_T I2
#define Cell11Gain_T I2
#define Cell12Gain_T I2
#define Cell13Gain_T I2
#define Cell14Gain_T I2
#define Cell15Gain_T I2
#define Cell16Gain_T I2
#define PackGain_T U2
#define TOSGain_T U2
#define LDGain_T U2
#define ADCGain_T I2
#define CCGain_T F4
#define CapacityGain_T F4
#define VcellOffset_T I2
#define VdivOffset_T I2
#define CoulombCounterOffsetSamples_T U2
#define BoardOffset_T I2
#define InternalTempOffset_T I1
#define CFETOFFTempOffset_T I1
#define DFETOFFTempOffset_T I1
#define ALERTTempOffset_T I1
#define TS1TempOffset_T I1
#define TS2TempOffset_T I1
#define TS3TempOffset_T I1
#define HDQTempOffset_T I1
#define DCHGTempOffset_T I1
#define DDSGTempOffset_T I1
#define IntGain_T I2
#define Intbaseoffset_T I2
#define IntMaximumAD_T I2
#define IntMaximumTemp_T I2
#define T18kCoeffa1_T I2
#define T18kCoeffa2_T I2
#define T18kCoeffa3_T I2
#define T18kCoeffa4_T I2
#define T18kCoeffa5_T I2
#define T18kCoeffb1_T I2
#define T18kCoeffb2_T I2
#define T18kCoeffb3_T I2
#define T18kCoeffb4_T I2
#define T18kAdc0_T I2
#define T180kCoeffa1_T I2
#define T180kCoeffa2_T I2
#define T180kCoeffa3_T I2
#define T180kCoeffa4_T I2
#define T180kCoeffa5_T I2
#define T180kCoeffb1_T I2
#define T180kCoeffb2_T I2
#define T180kCoeffb3_T I2
#define T180kCoeffb4_T I2
#define T180kAdc0_T I2
#define CustomCoeffa1_T I2
#define CustomCoeffa2_T I2
#define CustomCoeffa3_T I2
#define CustomCoeffa4_T I2
#define CustomCoeffa5_T I2
#define CustomCoeffb1_T I2
#define CustomCoeffb2_T I2
#define CustomCoeffb3_T I2
#define CustomCoeffb4_T I2
#define CustomRc0_T I2
#define CustomAdc0_T I2
#define CoulombCounterDeadband_T U1
#define CUVThresholdOverride_T U2
#define COVThresholdOverride_T U2
#define MinBlowFuseVoltage_T I2
#define FuseBlowTimeout_T U1
#define PowerConfig_T H2
#define REG12Config_T H1
#define REG0Config_T H1
#define HWDRegulatorOptions_T H1
#define CommType_T U1
#define I2CAddress_T U1
#define SPIConfiguration_T H1
#define CommIdleTime_T U1
#define CFETOFFPinConfig_T H1
#define DFETOFFPinConfig_T H1
#define ALERTPinConfig_T H1
#define TS1Config_T H1
#define TS2Config_T H1
#define TS3Config_T H1
#define HDQPinConfig_T H1
#define DCHGPinConfig_T H1
#define DDSGPinConfig_T H1
#define DAConfiguration_T H1
#define VCellMode_T H2
#define CC3Samples_T U1
#define ProtectionConfiguration_T H2
#define EnabledProtectionsA_T U1
#define EnabledProtectionsB_T U1
#define EnabledProtectionsC_T U1
#define CHGFETProtectionsA_T U1
#define CHGFETProtectionsB_T U1
#define CHGFETProtectionsC_T U1
#define DSGFETProtectionsA_T U1
#define DSGFETProtectionsB_T U1
#define DSGFETProtectionsC_T U1
#define BodyDiodeThreshold_T I2
#define DefaultAlarmMask_T H2
#define SFAlertMaskA_T U1
#define SFAlertMaskB_T U1
#define SFAlertMaskC_T U1
#define PFAlertMaskA_T U1
#define PFAlertMaskB_T U1
#define PFAlertMaskC_T U1
#define PFAlertMaskD_T U1
#define EnabledPFA_T U1
#define EnabledPFB_T U1
#define EnabledPFC_T U1
#define EnabledPFD_T U1
#define FETOptions_T H1
#define ChgPumpControl_T U1
#define PrechargeStartVoltage_T I2
#define PrechargeStopVoltage_T I2
#define PredischargeTimeout_T U1
#define PredischargeStopDelta_T U1
#define DsgCurrentThreshold_T I2
#define ChgCurrentThreshold_T I2
#define CheckTime_T U1
#define Cell1Interconnect_T I2
#define Cell2Interconnect_T I2
#define Cell3Interconnect_T I2
#define Cell4Interconnect_T I2
#define Cell5Interconnect_T I2
#define Cell6Interconnect_T I2
#define Cell7Interconnect_T I2
#define Cell8Interconnect_T I2
#define Cell9Interconnect_T I2
#define Cell10Interconnect_T I2
#define Cell11Interconnect_T I2
#define Cell12Interconnect_T I2
#define Cell13Interconnect_T I2
#define Cell14Interconnect_T I2
#define Cell15Interconnect_T I2
#define Cell16Interconnect_T I2
#define MfgStatusInit_T H2
#define BalancingConfiguration_T H1
#define MinCellTemp_T I1
#define MaxCellTemp_T I1
#define MaxInternalTemp_T I1
#define CellBalanceInterval_T U1
#define CellBalanceMaxCells_T U1
#define CellBalanceMinCellVCharge_T I2
#define CellBalanceMinDeltaCharge_T U1
#define CellBalanceStopDeltaCharge_T U1
#define CellBalanceMinCellVRelax_T I2
#define CellBalanceMinDeltaRelax_T U1
#define CellBalanceStopDeltaRelax_T U1
#define ShutdownCellVoltage_T I2
#define ShutdownStackVoltage_T I2
#define LowVShutdownDelay_T U1
#define ShutdownTemperature_T U1
#define ShutdownTemperatureDelay_T U1
#define FETOffDelay_T U1
#define ShutdownCommandDelay_T U1
#define AutoShutdownTime_T U1
#define RAMFailShutdownTime_T U1
#define SleepCurrent_T I2
#define VoltageTime_T U1
#define WakeComparatorCurrent_T I2
#define SleepHysteresisTime_T U1
#define SleepChargerVoltageThreshold_T I2
#define SleepChargerPACKTOSDelta_T I2
#define ConfigRAMSignature_T U2
#define CUVThreshold_T U1
#define CUVDelay_T U2
#define CUVRecoveryHysteresis_T U1
#define COVThreshold_T U1
#define COVDelay_T U2
#define COVRecoveryHysteresis_T U1
#define COVLLatchLimit_T U1
#define COVLCounterDecDelay_T U1
#define COVLRecoveryTime_T U1
#define OCCThreshold_T U1
#define OCCDelay_T U1
#define OCCRecoveryThreshold_T I2
#define OCCPACKTOSDelta_T I2
#define OCD1Threshold_T U1
#define OCD1Delay_T U1
#define OCD2Threshold_T U1
#define OCD2Delay_T U1
#define SCDThreshold_T U1
#define SCDDelay_T U1
#define SCDRecoveryTime_T U1
#define OCD3Threshold_T I2
#define OCD3Delay_T U1
#define OCDRecoveryThreshold_T I2
#define OCDLLatchLimit_T U1
#define OCDLCounterDecDelay_T U1
#define OCDLRecoveryTime_T U1
#define OCDLRecoveryThreshold_T I2
#define SCDLLatchLimit_T U1
#define SCDLCounterDecDelay_T U1
#define SCDLRecoveryTime_T U1
#define SCDLRecoveryThreshold_T I2
#define OTCThreshold_T I1
#define OTCDelay_T U1
#define OTCRecovery_T I1
#define OTDThreshold_T I1
#define OTDDelay_T U1
#define OTDRecovery_T I1
#define OTFThreshold_T U1
#define OTFDelay_T U1
#define OTFRecovery_T U1
#define OTINTThreshold_T I1
#define OTINTDelay_T U1
#define OTINTRecovery_T I1
#define UTCThreshold_T I1
#define UTCDelay_T U1
#define UTCRecovery_T I1
#define UTDThreshold_T I1
#define UTDDelay_T U1
#define UTDRecovery_T I1
#define UTINTThreshold_T I1
#define UTINTDelay_T U1
#define UTINTRecovery_T I1
#define ProtectionsRecoveryTime_T U1
#define HWDDelay_T U2
#define LoadDetectActiveTime_T U1
#define LoadDetectRetryDelay_T U1
#define LoadDetectTimeout_T U2
#define PTOChargeThreshold_T I2
#define PTODelay_T U2
#define PTOReset_T I2
#define CUDEPThreshold_T I2
#define CUDEPDelay_T U1
#define SUVThreshold_T I2
#define SUVDelay_T U1
#define SOVThreshold_T I2
#define SOVDelay_T U1
#define TOSSThreshold_T I2
#define TOSSDelay_T U1
#define SOCCThreshold_T I2
#define SOCCDelay_T U1
#define SOCDThreshold_T I2
#define SOCDDelay_T U1
#define SOTThreshold_T I1
#define SOTDelay_T U1
#define SOTFThreshold_T U1
#define SOTFDelay_T U1
#define VIMRCheckVoltage_T I2
#define VIMRMaxRelaxCurrent_T I2
#define VIMRThreshold_T I2
#define VIMRDelay_T U1
#define VIMRRelaxMinDuration_T U2
#define VIMACheckVoltage_T I2
#define VIMAMinActiveCurrent_T I2
#define VIMAThreshold_T I2
#define VIMADelay_T U1
#define CFETFOFFThreshold_T I2
#define CFETFOFFDelay_T U1
#define DFETFOFFThreshold_T I2
#define DFETFOFFDelay_T U1
#define VSSFFailThreshold_T I2
#define VSSFDelay_T U1
#define PF2LVLDelay_T U1
#define LFOFDelay_T U1
#define HWMXDelay_T U1
#define SecuritySettings_T H1
#define UnsealKeyStep1_T U2
#define UnsealKeyStep2_T U2
#define FullAccessKeyStep1_T U2
#define FullAccessKeyStep2_T U2

#define Cell1Gain_MIN (-32767)
#define Cell2Gain_MIN (-32767)
#define Cell3Gain_MIN (-32767)
#define Cell4Gain_MIN (-32767)
#define Cell5Gain_MIN (-32767)
#define Cell6Gain_MIN (-32767)
#define Cell7Gain_MIN (-32767)
#define Cell8Gain_MIN (-32767)
#define Cell9Gain_MIN (-32767)
#define Cell10Gain_MIN (-32767)
#define Cell11Gain_MIN (-32767)
#define Cell12Gain_MIN (-32767)
#define Cell13Gain_MIN (-32767)
#define Cell14Gain_MIN (-32767)
#define Cell15Gain_MIN (-32767)
#define Cell16Gain_MIN (-32767)
#define PackGain_MIN (0)
#define TOSGain_MIN (0)
#define LDGain_MIN (0)
#define ADCGain_MIN (-32767)
#define CCGain_MIN (1.00E-02F)
#define CapacityGain_MIN (2.98262E+03F)
#define VcellOffset_MIN (-32767)
#define VdivOffset_MIN (-32767)
#define CoulombCounterOffsetSamples_MIN (0)
#define BoardOffset_MIN (-32768)
#define InternalTempOffset_MIN (-128)
#define CFETOFFTempOffset_MIN (-128)
#define DFETOFFTempOffset_MIN (-128)
#define ALERTTempOffset_MIN (-128)
#define TS1TempOffset_MIN (-128)
#define TS2TempOffset_MIN (-128)
#define TS3TempOffset_MIN (-128)
#define HDQTempOffset_MIN (-128)
#define DCHGTempOffset_MIN (-128)
#define DDSGTempOffset_MIN (-128)
#define IntGain_MIN (-32768)
#define Intbaseoffset_MIN (-32768)
#define IntMaximumAD_MIN (-32768)
#define IntMaximumTemp_MIN (0)
#define T18kCoeffa1_MIN (-32768)
#define T18kCoeffa2_MIN (-32768)
#define T18kCoeffa3_MIN (-32768)
#define T18kCoeffa4_MIN (-32768)
#define T18kCoeffa5_MIN (-32768)
#define T18kCoeffb1_MIN (-32768)
#define T18kCoeffb2_MIN (-32768)
#define T18kCoeffb3_MIN (-32768)
#define T18kCoeffb4_MIN (-32768)
#define T18kAdc0_MIN (-32768)
#define T180kCoeffa1_MIN (-32768)
#define T180kCoeffa2_MIN (-32768)
#define T180kCoeffa3_MIN (-32768)
#define T180kCoeffa4_MIN (-32768)
#define T180kCoeffa5_MIN (-32768)
#define T180kCoeffb1_MIN (-32768)
#define T180kCoeffb2_MIN (-32768)
#define T180kCoeffb3_MIN (-32768)
#define T180kCoeffb4_MIN (-32768)
#define T180kAdc0_MIN (-32768)
#define CustomCoeffa1_MIN (-32768)
#define CustomCoeffa2_MIN (-32768)
#define CustomCoeffa3_MIN (-32768)
#define CustomCoeffa4_MIN (-32768)
#define CustomCoeffa5_MIN (-32768)
#define CustomCoeffb1_MIN (-32768)
#define CustomCoeffb2_MIN (-32768)
#define CustomCoeffb3_MIN (-32768)
#define CustomCoeffb4_MIN (-32768)
#define CustomRc0_MIN (-32768)
#define CustomAdc0_MIN (-32768)
#define CoulombCounterDeadband_MIN (0)
#define CUVThresholdOverride_MIN (0x0000)
#define COVThresholdOverride_MIN (0x0000)
#define MinBlowFuseVoltage_MIN (0)
#define FuseBlowTimeout_MIN (0)
#define PowerConfig_MIN (0x0000)
#define REG12Config_MIN (0x00)
#define REG0Config_MIN (0x00)
#define HWDRegulatorOptions_MIN (0x00)
#define CommType_MIN (0x00)
#define I2CAddress_MIN (0x00)
#define SPIConfiguration_MIN (0x00)
#define CommIdleTime_MIN (0)
#define CFETOFFPinConfig_MIN (0x00)
#define DFETOFFPinConfig_MIN (0x00)
#define ALERTPinConfig_MIN (0x00)
#define TS1Config_MIN (0x00)
#define TS2Config_MIN (0x00)
#define TS3Config_MIN (0x00)
#define HDQPinConfig_MIN (0x00)
#define DCHGPinConfig_MIN (0x00)
#define DDSGPinConfig_MIN (0x00)
#define DAConfiguration_MIN (0x00)
#define VCellMode_MIN (0x0000)
#define CC3Samples_MIN (2)
#define ProtectionConfiguration_MIN (0x0000)
#define EnabledProtectionsA_MIN (0x00)
#define EnabledProtectionsB_MIN (0x00)
#define EnabledProtectionsC_MIN (0x00)
#define CHGFETProtectionsA_MIN (0x00)
#define CHGFETProtectionsB_MIN (0x00)
#define CHGFETProtectionsC_MIN (0x00)
#define DSGFETProtectionsA_MIN (0x00)
#define DSGFETProtectionsB_MIN (0x00)
#define DSGFETProtectionsC_MIN (0x00)
#define BodyDiodeThreshold_MIN (0)
#define DefaultAlarmMask_MIN (0x0000)
#define SFAlertMaskA_MIN (0x00)
#define SFAlertMaskB_MIN (0x00)
#define SFAlertMaskC_MIN (0x00)
#define PFAlertMaskA_MIN (0x00)
#define PFAlertMaskB_MIN (0x00)
#define PFAlertMaskC_MIN (0x00)
#define PFAlertMaskD_MIN (0x00)
#define EnabledPFA_MIN (0x00)
#define EnabledPFB_MIN (0x00)
#define EnabledPFC_MIN (0x00)
#define EnabledPFD_MIN (0x00)
#define FETOptions_MIN (0x00)
#define ChgPumpControl_MIN (0x00)
#define PrechargeStartVoltage_MIN (0)
#define PrechargeStopVoltage_MIN (0)
#define PredischargeTimeout_MIN (0)
#define PredischargeStopDelta_MIN (0)
#define DsgCurrentThreshold_MIN (0)
#define ChgCurrentThreshold_MIN (0)
#define CheckTime_MIN (0)
#define Cell1Interconnect_MIN (0)
#define Cell2Interconnect_MIN (0)
#define Cell3Interconnect_MIN (0)
#define Cell4Interconnect_MIN (0)
#define Cell5Interconnect_MIN (0)
#define Cell6Interconnect_MIN (0)
#define Cell7Interconnect_MIN (0)
#define Cell8Interconnect_MIN (0)
#define Cell9Interconnect_MIN (0)
#define Cell10Interconnect_MIN (0)
#define Cell11Interconnect_MIN (0)
#define Cell12Interconnect_MIN (0)
#define Cell13Interconnect_MIN (0)
#define Cell14Interconnect_MIN (0)
#define Cell15Interconnect_MIN (0)
#define Cell16Interconnect_MIN (0)
#define MfgStatusInit_MIN (0x0000)
#define BalancingConfiguration_MIN (0x00)
#define MinCellTemp_MIN (-128)
#define MaxCellTemp_MIN (-128)
#define MaxInternalTemp_MIN (-128)
#define CellBalanceInterval_MIN (1)
#define CellBalanceMaxCells_MIN (0)
#define CellBalanceMinCellVCharge_MIN (0)
#define CellBalanceMinDeltaCharge_MIN (0)
#define CellBalanceStopDeltaCharge_MIN (0)
#define CellBalanceMinCellVRelax_MIN (0)
#define CellBalanceMinDeltaRelax_MIN (0)
#define CellBalanceStopDeltaRelax_MIN (0)
#define ShutdownCellVoltage_MIN (0)
#define ShutdownStackVoltage_MIN (0)
#define LowVShutdownDelay_MIN (0)
#define ShutdownTemperature_MIN (0)
#define ShutdownTemperatureDelay_MIN (0)
#define FETOffDelay_MIN (0)
#define ShutdownCommandDelay_MIN (0)
#define AutoShutdownTime_MIN (0)
#define RAMFailShutdownTime_MIN (0)
#define SleepCurrent_MIN (0)
#define VoltageTime_MIN (1)
#define WakeComparatorCurrent_MIN (500)
#define SleepHysteresisTime_MIN (0)
#define SleepChargerVoltageThreshold_MIN (0)
#define SleepChargerPACKTOSDelta_MIN (10)
#define ConfigRAMSignature_MIN (0x0000)
#define CUVThreshold_MIN (20)
#define CUVDelay_MIN (1)
#define CUVRecoveryHysteresis_MIN (2)
#define COVThreshold_MIN (20)
#define COVDelay_MIN (1)
#define COVRecoveryHysteresis_MIN (2)
#define COVLLatchLimit_MIN (0)
#define COVLCounterDecDelay_MIN (0)
#define COVLRecoveryTime_MIN (0)
#define OCCThreshold_MIN (2)
#define OCCDelay_MIN (1)
#define OCCRecoveryThreshold_MIN (-32768)
#define OCCPACKTOSDelta_MIN (10)
#define OCD1Threshold_MIN (2)
#define OCD1Delay_MIN (1)
#define OCD2Threshold_MIN (2)
#define OCD2Delay_MIN (1)
#define SCDThreshold_MIN (0)
#define SCDDelay_MIN (1)
#define SCDRecoveryTime_MIN (0)
#define OCD3Threshold_MIN (-32768)
#define OCD3Delay_MIN (0)
#define OCDRecoveryThreshold_MIN (-32768)
#define OCDLLatchLimit_MIN (0)
#define OCDLCounterDecDelay_MIN (0)
#define OCDLRecoveryTime_MIN (0)
#define OCDLRecoveryThreshold_MIN (-32768)
#define SCDLLatchLimit_MIN (0)
#define SCDLCounterDecDelay_MIN (0)
#define SCDLRecoveryTime_MIN (0)
#define SCDLRecoveryThreshold_MIN (-32768)
#define OTCThreshold_MIN (-40)
#define OTCDelay_MIN (0)
#define OTCRecovery_MIN (-40)
#define OTDThreshold_MIN (-40)
#define OTDDelay_MIN (0)
#define OTDRecovery_MIN (-40)
#define OTFThreshold_MIN (0)
#define OTFDelay_MIN (0)
#define OTFRecovery_MIN (0)
#define OTINTThreshold_MIN (-40)
#define OTINTDelay_MIN (0)
#define OTINTRecovery_MIN (-40)
#define UTCThreshold_MIN (-40)
#define UTCDelay_MIN (0)
#define UTCRecovery_MIN (-40)
#define UTDThreshold_MIN (-40)
#define UTDDelay_MIN (0)
#define UTDRecovery_MIN (-40)
#define UTINTThreshold_MIN (-40)
#define UTINTDelay_MIN (0)
#define UTINTRecovery_MIN (-40)
#define ProtectionsRecoveryTime_MIN (0)
#define HWDDelay_MIN (0)
#define LoadDetectActiveTime_MIN (0)
#define LoadDetectRetryDelay_MIN (0)
#define LoadDetectTimeout_MIN (0)
#define PTOChargeThreshold_MIN (-32768)
#define PTODelay_MIN (0)
#define PTOReset_MIN (0)
#define CUDEPThreshold_MIN (0)
#define CUDEPDelay_MIN (0)
#define SUVThreshold_MIN (0)
#define SUVDelay_MIN (0)
#define SOVThreshold_MIN (0)
#define SOVDelay_MIN (0)
#define TOSSThreshold_MIN (0)
#define TOSSDelay_MIN (0)
#define SOCCThreshold_MIN (-32768)
#define SOCCDelay_MIN (0)
#define SOCDThreshold_MIN (-32768)
#define SOCDDelay_MIN (0)
#define SOTThreshold_MIN (-40)
#define SOTDelay_MIN (0)
#define SOTFThreshold_MIN (0)
#define SOTFDelay_MIN (0)
#define VIMRCheckVoltage_MIN (0)
#define VIMRMaxRelaxCurrent_MIN (10)
#define VIMRThreshold_MIN (0)
#define VIMRDelay_MIN (0)
#define VIMRRelaxMinDuration_MIN (0)
#define VIMACheckVoltage_MIN (0)
#define VIMAMinActiveCurrent_MIN (10)
#define VIMAThreshold_MIN (0)
#define VIMADelay_MIN (0)
#define CFETFOFFThreshold_MIN (10)
#define CFETFOFFDelay_MIN (0)
#define DFETFOFFThreshold_MIN (-5000)
#define DFETFOFFDelay_MIN (0)
#define VSSFFailThreshold_MIN (1)
#define VSSFDelay_MIN (0)
#define PF2LVLDelay_MIN (0)
#define LFOFDelay_MIN (0)
#define HWMXDelay_MIN (0)
#define SecuritySettings_MIN (0x00)
#define UnsealKeyStep1_MIN (0x0100)
#define UnsealKeyStep2_MIN (0x0100)
#define FullAccessKeyStep1_MIN (0x0100)
#define FullAccessKeyStep2_MIN (0x0100)

#define Cell1Gain_MAX (32767)
#define Cell2Gain_MAX (32767)
#define Cell3Gain_MAX (32767)
#define Cell4Gain_MAX (32767)
#define Cell5Gain_MAX (32767)
#define Cell6Gain_MAX (32767)
#define Cell7Gain_MAX (32767)
#define Cell8Gain_MAX (32767)
#define Cell9Gain_MAX (32767)
#define Cell10Gain_MAX (32767)
#define Cell11Gain_MAX (32767)
#define Cell12Gain_MAX (32767)
#define Cell13Gain_MAX (32767)
#define Cell14Gain_MAX (32767)
#define Cell15Gain_MAX (32767)
#define Cell16Gain_MAX (32767)
#define PackGain_MAX (65535)
#define TOSGain_MAX (65535)
#define LDGain_MAX (65535)
#define ADCGain_MAX (32767)
#define CCGain_MAX (10.00E+02F)
#define CapacityGain_MAX (4.193046E+08F)
#define VcellOffset_MAX (32767)
#define VdivOffset_MAX (32767)
#define CoulombCounterOffsetSamples_MAX (65535)
#define BoardOffset_MAX (32767)
#define InternalTempOffset_MAX (127)
#define CFETOFFTempOffset_MAX (127)
#define DFETOFFTempOffset_MAX (127)
#define ALERTTempOffset_MAX (127)
#define TS1TempOffset_MAX (127)
#define TS2TempOffset_MAX (127)
#define TS3TempOffset_MAX (127)
#define HDQTempOffset_MAX (127)
#define DCHGTempOffset_MAX (127)
#define DDSGTempOffset_MAX (127)
#define IntGain_MAX (32767)
#define Intbaseoffset_MAX (32767)
#define IntMaximumAD_MAX (32767)
#define IntMaximumTemp_MAX (32767)
#define T18kCoeffa1_MAX (32767)
#define T18kCoeffa2_MAX (32767)
#define T18kCoeffa3_MAX (32767)
#define T18kCoeffa4_MAX (32767)
#define T18kCoeffa5_MAX (32767)
#define T18kCoeffb1_MAX (32767)
#define T18kCoeffb2_MAX (32767)
#define T18kCoeffb3_MAX (32767)
#define T18kCoeffb4_MAX (32767)
#define T18kAdc0_MAX (32767)
#define T180kCoeffa1_MAX (32767)
#define T180kCoeffa2_MAX (32767)
#define T180kCoeffa3_MAX (32767)
#define T180kCoeffa4_MAX (32767)
#define T180kCoeffa5_MAX (32767)
#define T180kCoeffb1_MAX (32767)
#define T180kCoeffb2_MAX (32767)
#define T180kCoeffb3_MAX (32767)
#define T180kCoeffb4_MAX (32767)
#define T180kAdc0_MAX (32767)
#define CustomCoeffa1_MAX (32767)
#define CustomCoeffa2_MAX (32767)
#define CustomCoeffa3_MAX (32767)
#define CustomCoeffa4_MAX (32767)
#define CustomCoeffa5_MAX (32767)
#define CustomCoeffb1_MAX (32767)
#define CustomCoeffb2_MAX (32767)
#define CustomCoeffb3_MAX (32767)
#define CustomCoeffb4_MAX (32767)
#define CustomRc0_MAX (32767)
#define CustomAdc0_MAX (32767)
#define CoulombCounterDeadband_MAX (255)
#define CUVThresholdOverride_MAX (0xFFFF)
#define COVThresholdOverride_MAX (0xFFFF)
#define MinBlowFuseVoltage_MAX (32767)
#define FuseBlowTimeout_MAX (255)
#define PowerConfig_MAX (0xFFFF)
#define REG12Config_MAX (0xFF)
#define REG0Config_MAX (0x03)
#define HWDRegulatorOptions_MAX (0xFF)
#define CommType_MAX (0x1F)
#define I2CAddress_MAX (0xFF)
#define SPIConfiguration_MAX (0x7F)
#define CommIdleTime_MAX (255)
#define CFETOFFPinConfig_MAX (0xFF)
#define DFETOFFPinConfig_MAX (0xFF)
#define ALERTPinConfig_MAX (0xFF)
#define TS1Config_MAX (0xFF)
#define TS2Config_MAX (0xFF)
#define TS3Config_MAX (0xFF)
#define HDQPinConfig_MAX (0xFF)
#define DCHGPinConfig_MAX (0xFF)
#define DDSGPinConfig_MAX (0xFF)
#define DAConfiguration_MAX (0xFF)
#define VCellMode_MAX (0xFFFF)
#define CC3Samples_MAX (255)
#define ProtectionConfiguration_MAX (0x07FF)
#define EnabledProtectionsA_MAX (0xFF)
#define EnabledProtectionsB_MAX (0xFF)
#define EnabledProtectionsC_MAX (0xFF)
#define CHGFETProtectionsA_MAX (0xFF)
#define CHGFETProtectionsB_MAX (0xFF)
#define CHGFETProtectionsC_MAX (0xFF)
#define DSGFETProtectionsA_MAX (0xFF)
#define DSGFETProtectionsB_MAX (0xFF)
#define DSGFETProtectionsC_MAX (0xFF)
#define BodyDiodeThreshold_MAX (32767)
#define DefaultAlarmMask_MAX (0xFFFF)
#define SFAlertMaskA_MAX (0xFF)
#define SFAlertMaskB_MAX (0xFF)
#define SFAlertMaskC_MAX (0xFF)
#define PFAlertMaskA_MAX (0xFF)
#define PFAlertMaskB_MAX (0xFF)
#define PFAlertMaskC_MAX (0xFF)
#define PFAlertMaskD_MAX (0xFF)
#define EnabledPFA_MAX (0xFF)
#define EnabledPFB_MAX (0xFF)
#define EnabledPFC_MAX (0xFF)
#define EnabledPFD_MAX (0xFF)
#define FETOptions_MAX (0xFF)
#define ChgPumpControl_MAX (0xFF)
#define PrechargeStartVoltage_MAX (32767)
#define PrechargeStopVoltage_MAX (32767)
#define PredischargeTimeout_MAX (255)
#define PredischargeStopDelta_MAX (255)
#define DsgCurrentThreshold_MAX (32767)
#define ChgCurrentThreshold_MAX (32767)
#define CheckTime_MAX (255)
#define Cell1Interconnect_MAX (32767)
#define Cell2Interconnect_MAX (32767)
#define Cell3Interconnect_MAX (32767)
#define Cell4Interconnect_MAX (32767)
#define Cell5Interconnect_MAX (32767)
#define Cell6Interconnect_MAX (32767)
#define Cell7Interconnect_MAX (32767)
#define Cell8Interconnect_MAX (32767)
#define Cell9Interconnect_MAX (32767)
#define Cell10Interconnect_MAX (32767)
#define Cell11Interconnect_MAX (32767)
#define Cell12Interconnect_MAX (32767)
#define Cell13Interconnect_MAX (32767)
#define Cell14Interconnect_MAX (32767)
#define Cell15Interconnect_MAX (32767)
#define Cell16Interconnect_MAX (32767)
#define MfgStatusInit_MAX (0xFFFF)
#define BalancingConfiguration_MAX (0xFF)
#define MinCellTemp_MAX (127)
#define MaxCellTemp_MAX (127)
#define MaxInternalTemp_MAX (127)
#define CellBalanceInterval_MAX (255)
#define CellBalanceMaxCells_MAX (16)
#define CellBalanceMinCellVCharge_MAX (5000)
#define CellBalanceMinDeltaCharge_MAX (255)
#define CellBalanceStopDeltaCharge_MAX (255)
#define CellBalanceMinCellVRelax_MAX (5000)
#define CellBalanceMinDeltaRelax_MAX (255)
#define CellBalanceStopDeltaRelax_MAX (255)
#define ShutdownCellVoltage_MAX (32767)
#define ShutdownStackVoltage_MAX (32767)
#define LowVShutdownDelay_MAX (63)
#define ShutdownTemperature_MAX (150)
#define ShutdownTemperatureDelay_MAX (254)
#define FETOffDelay_MAX (127)
#define ShutdownCommandDelay_MAX (254)
#define AutoShutdownTime_MAX (250)
#define RAMFailShutdownTime_MAX (255)
#define SleepCurrent_MAX (32767)
#define VoltageTime_MAX (255)
#define WakeComparatorCurrent_MAX (32767)
#define SleepHysteresisTime_MAX (255)
#define SleepChargerVoltageThreshold_MAX (32767)
#define SleepChargerPACKTOSDelta_MAX (8500)
#define ConfigRAMSignature_MAX (0x7FFF)
#define CUVThreshold_MAX (90)
#define CUVDelay_MAX (2047)
#define CUVRecoveryHysteresis_MAX (20)
#define COVThreshold_MAX (110)
#define COVDelay_MAX (2047)
#define COVRecoveryHysteresis_MAX (20)
#define COVLLatchLimit_MAX (255)
#define COVLCounterDecDelay_MAX (255)
#define COVLRecoveryTime_MAX (255)
#define OCCThreshold_MAX (62)
#define OCCDelay_MAX (127)
#define OCCRecoveryThreshold_MAX (32767)
#define OCCPACKTOSDelta_MAX (8500)
#define OCD1Threshold_MAX (100)
#define OCD1Delay_MAX (127)
#define OCD2Threshold_MAX (100)
#define OCD2Delay_MAX (127)
#define SCDThreshold_MAX (15)
#define SCDDelay_MAX (31)
#define SCDRecoveryTime_MAX (255)
#define OCD3Threshold_MAX (0)
#define OCD3Delay_MAX (255)
#define OCDRecoveryThreshold_MAX (32767)
#define OCDLLatchLimit_MAX (255)
#define OCDLCounterDecDelay_MAX (255)
#define OCDLRecoveryTime_MAX (255)
#define OCDLRecoveryThreshold_MAX (32767)
#define SCDLLatchLimit_MAX (255)
#define SCDLCounterDecDelay_MAX (255)
#define SCDLRecoveryTime_MAX (255)
#define SCDLRecoveryThreshold_MAX (32767)
#define OTCThreshold_MAX (120)
#define OTCDelay_MAX (255)
#define OTCRecovery_MAX (120)
#define OTDThreshold_MAX (120)
#define OTDDelay_MAX (255)
#define OTDRecovery_MAX (120)
#define OTFThreshold_MAX (150)
#define OTFDelay_MAX (255)
#define OTFRecovery_MAX (150)
#define OTINTThreshold_MAX (120)
#define OTINTDelay_MAX (255)
#define OTINTRecovery_MAX (120)
#define UTCThreshold_MAX (120)
#define UTCDelay_MAX (255)
#define UTCRecovery_MAX (120)
#define UTDThreshold_MAX (120)
#define UTDDelay_MAX (255)
#define UTDRecovery_MAX (120)
#define UTINTThreshold_MAX (120)
#define UTINTDelay_MAX (255)
#define UTINTRecovery_MAX (120)
#define ProtectionsRecoveryTime_MAX (255)
#define HWDDelay_MAX (65535)
#define LoadDetectActiveTime_MAX (255)
#define LoadDetectRetryDelay_MAX (255)
#define LoadDetectTimeout_MAX (65535)
#define PTOChargeThreshold_MAX (32767)
#define PTODelay_MAX (65535)
#define PTOReset_MAX (10000)
#define CUDEPThreshold_MAX (32767)
#define CUDEPDelay_MAX (255)
#define SUVThreshold_MAX (32767)
#define SUVDelay_MAX (255)
#define SOVThreshold_MAX (32767)
#define SOVDelay_MAX (255)
#define TOSSThreshold_MAX (32767)
#define TOSSDelay_MAX (255)
#define SOCCThreshold_MAX (32767)
#define SOCCDelay_MAX (255)
#define SOCDThreshold_MAX (32767)
#define SOCDDelay_MAX (255)
#define SOTThreshold_MAX (120)
#define SOTDelay_MAX (255)
#define SOTFThreshold_MAX (150)
#define SOTFDelay_MAX (255)
#define VIMRCheckVoltage_MAX (5500)
#define VIMRMaxRelaxCurrent_MAX (32767)
#define VIMRThreshold_MAX (5500)
#define VIMRDelay_MAX (255)
#define VIMRRelaxMinDuration_MAX (65535)
#define VIMACheckVoltage_MAX (5500)
#define VIMAMinActiveCurrent_MAX (32767)
#define VIMAThreshold_MAX (5500)
#define VIMADelay_MAX (255)
#define CFETFOFFThreshold_MAX (5000)
#define CFETFOFFDelay_MAX (255)
#define DFETFOFFThreshold_MAX (-10)
#define DFETFOFFDelay_MAX (255)
#define VSSFFailThreshold_MAX (32767)
#define VSSFDelay_MAX (255)
#define PF2LVLDelay_MAX (255)
#define LFOFDelay_MAX (255)
#define HWMXDelay_MAX (255)
#define SecuritySettings_MAX (0x07)
#define UnsealKeyStep1_MAX (0xFFFF)
#define UnsealKeyStep2_MAX (0xFFFF)
#define FullAccessKeyStep1_MAX (0xFFFF)
#define FullAccessKeyStep2_MAX (0xFFFF)

#define Cell1Gain_DEFAULTRAW (0)
#define Cell2Gain_DEFAULTRAW (0)
#define Cell3Gain_DEFAULTRAW (0)
#define Cell4Gain_DEFAULTRAW (0)
#define Cell5Gain_DEFAULTRAW (0)
#define Cell6Gain_DEFAULTRAW (0)
#define Cell7Gain_DEFAULTRAW (0)
#define Cell8Gain_DEFAULTRAW (0)
#define Cell9Gain_DEFAULTRAW (0)
#define Cell10Gain_DEFAULTRAW (0)
#define Cell11Gain_DEFAULTRAW (0)
#define Cell12Gain_DEFAULTRAW (0)
#define Cell13Gain_DEFAULTRAW (0)
#define Cell14Gain_DEFAULTRAW (0)
#define Cell15Gain_DEFAULTRAW (0)
#define Cell16Gain_DEFAULTRAW (0)
#define PackGain_DEFAULTRAW (0)
#define TOSGain_DEFAULTRAW (0)
#define LDGain_DEFAULTRAW (0)
#define ADCGain_DEFAULTRAW (0)
#define CCGain_DEFAULTRAW (7.4768F)
#define CapacityGain_DEFAULTRAW (2230042.463F)
#define VcellOffset_DEFAULTRAW (0)
#define VdivOffset_DEFAULTRAW (0)
#define CoulombCounterOffsetSamples_DEFAULTRAW (64)
#define BoardOffset_DEFAULTRAW (0)
#define InternalTempOffset_DEFAULTRAW (0)
#define CFETOFFTempOffset_DEFAULTRAW (0)
#define DFETOFFTempOffset_DEFAULTRAW (0)
#define ALERTTempOffset_DEFAULTRAW (0)
#define TS1TempOffset_DEFAULTRAW (0)
#define TS2TempOffset_DEFAULTRAW (0)
#define TS3TempOffset_DEFAULTRAW (0)
#define HDQTempOffset_DEFAULTRAW (0)
#define DCHGTempOffset_DEFAULTRAW (0)
#define DDSGTempOffset_DEFAULTRAW (0)
#define IntGain_DEFAULTRAW (25390)
#define Intbaseoffset_DEFAULTRAW (3032)
#define IntMaximumAD_DEFAULTRAW (16383)
#define IntMaximumTemp_DEFAULTRAW (6379)
#define T18kCoeffa1_DEFAULTRAW (-15524)
#define T18kCoeffa2_DEFAULTRAW (26423)
#define T18kCoeffa3_DEFAULTRAW (-22664)
#define T18kCoeffa4_DEFAULTRAW (28834)
#define T18kCoeffa5_DEFAULTRAW (672)
#define T18kCoeffb1_DEFAULTRAW (-371)
#define T18kCoeffb2_DEFAULTRAW (708)
#define T18kCoeffb3_DEFAULTRAW (-3498)
#define T18kCoeffb4_DEFAULTRAW (5051)
#define T18kAdc0_DEFAULTRAW (11703)
#define T180kCoeffa1_DEFAULTRAW (-17513)
#define T180kCoeffa2_DEFAULTRAW (25759)
#define T180kCoeffa3_DEFAULTRAW (-23593)
#define T180kCoeffa4_DEFAULTRAW (32175)
#define T180kCoeffa5_DEFAULTRAW (2090)
#define T180kCoeffb1_DEFAULTRAW (-2055)
#define T180kCoeffb2_DEFAULTRAW (2955)
#define T180kCoeffb3_DEFAULTRAW (-3427)
#define T180kCoeffb4_DEFAULTRAW (4385)
#define T180kAdc0_DEFAULTRAW (17246)
#define CustomCoeffa1_DEFAULTRAW (0)
#define CustomCoeffa2_DEFAULTRAW (0)
#define CustomCoeffa3_DEFAULTRAW (0)
#define CustomCoeffa4_DEFAULTRAW (0)
#define CustomCoeffa5_DEFAULTRAW (0)
#define CustomCoeffb1_DEFAULTRAW (0)
#define CustomCoeffb2_DEFAULTRAW (0)
#define CustomCoeffb3_DEFAULTRAW (0)
#define CustomCoeffb4_DEFAULTRAW (0)
#define CustomRc0_DEFAULTRAW (0)
#define CustomAdc0_DEFAULTRAW (0)
#define CoulombCounterDeadband_DEFAULTRAW (9)
#define CUVThresholdOverride_DEFAULTRAW (0xFFFF)
#define COVThresholdOverride_DEFAULTRAW (0xFFFF)
#define MinBlowFuseVoltage_DEFAULTRAW (500)
#define FuseBlowTimeout_DEFAULTRAW (30)
#define PowerConfig_DEFAULTRAW (PowerConfig_DPSLP_OT | PowerConfig_DPSLP_PD | PowerConfig_SLEEP | PowerConfig_OTSD | PowerConfig_WK_SPD_12ms)
#define REG12Config_DEFAULTRAW (0x00)
#define REG0Config_DEFAULTRAW (0x00)
#define HWDRegulatorOptions_DEFAULTRAW (0x00)
#define CommType_DEFAULTRAW (0x00)
#define I2CAddress_DEFAULTRAW (I2CAddress_DEFAULT_0x10)
#define SPIConfiguration_DEFAULTRAW (SPIConfiguration_FILT_EN)
#define CommIdleTime_DEFAULTRAW (0)
#define CFETOFFPinConfig_DEFAULTRAW (0x00)
#define DFETOFFPinConfig_DEFAULTRAW (0x00)
#define ALERTPinConfig_DEFAULTRAW (0x00)
#define TS1Config_DEFAULTRAW (PIN_FXN_ADC_THERMISTOR | PIN_ANALOG_TYPE_CELL_PROT)
#define TS2Config_DEFAULTRAW (0x00)
#define TS3Config_DEFAULTRAW (0x00)
#define HDQPinConfig_DEFAULTRAW (0x00)
#define DCHGPinConfig_DEFAULTRAW (0x00)
#define DDSGPinConfig_DEFAULTRAW (0x00)
#define DAConfiguration_DEFAULTRAW (DAConfiguration_USER_AMPS_mA | DAConfiguration_USER_VOLTS_cV)
#define VCellMode_DEFAULTRAW (0x0000)
#define CC3Samples_DEFAULTRAW (80)
#define ProtectionConfiguration_DEFAULTRAW (ProtectionConfiguration_PF_FETS)
#define EnabledProtectionsA_DEFAULTRAW (EnabledProtectionsA_SCD | EnabledProtectionsA_COV)
#define EnabledProtectionsB_DEFAULTRAW (0x00)
#define EnabledProtectionsC_DEFAULTRAW (0x00)
#define CHGFETProtectionsA_DEFAULTRAW (CHGFETProtectionsA_SCD | CHGFETProtectionsA_OCC | CHGFETProtectionsA_COV)
#define CHGFETProtectionsB_DEFAULTRAW (CHGFETProtectionsB_OTF | CHGFETProtectionsB_OTINT | CHGFETProtectionsB_OTC | CHGFETProtectionsB_UTINT | CHGFETProtectionsB_UTC)
#define CHGFETProtectionsC_DEFAULTRAW (CHGFETProtectionsC_SCDL | CHGFETProtectionsC_COVL | CHGFETProtectionsC_PTO | CHGFETProtectionsC_HWDF)
#define DSGFETProtectionsA_DEFAULTRAW (DSGFETProtectionsA_SCD | DSGFETProtectionsA_OCD2 | DSGFETProtectionsA_OCD1 | DSGFETProtectionsA_CUV)
#define DSGFETProtectionsB_DEFAULTRAW (DSGFETProtectionsB_OTF | DSGFETProtectionsB_OTINT | DSGFETProtectionsB_OTD | DSGFETProtectionsB_UTINT | DSGFETProtectionsB_UTD)
#define DSGFETProtectionsC_DEFAULTRAW (DSGFETProtectionsC_OCD3 | DSGFETProtectionsC_SCDL | DSGFETProtectionsC_OCDL | DSGFETProtectionsC_HWDF)
#define BodyDiodeThreshold_DEFAULTRAW (50)
#define DefaultAlarmMask_DEFAULTRAW (DefaultAlarmMask_SSBC | DefaultAlarmMask_SSA | DefaultAlarmMask_PF | DefaultAlarmMask_MSK_SFALERT | DefaultAlarmMask_MSK_PFALERT)
#define SFAlertMaskA_DEFAULTRAW (SFAlertMaskA_SCD | SFAlertMaskA_OCD2 | SFAlertMaskA_OCD1 | SFAlertMaskA_OCC | SFAlertMaskA_COV | SFAlertMaskA_CUV)
#define SFAlertMaskB_DEFAULTRAW (SFAlertMaskB_OTF | SFAlertMaskB_OTINT | SFAlertMaskB_OTD | SFAlertMaskB_OTC | SFAlertMaskB_UTINT | SFAlertMaskB_UTD | SFAlertMaskB_UTC)
#define SFAlertMaskC_DEFAULTRAW (SFAlertMaskC_OCD3 | SFAlertMaskC_SCDL | SFAlertMaskC_OCDL | SFAlertMaskC_COVL | SFAlertMaskC_PTO)
#define PFAlertMaskA_DEFAULTRAW (PFAlertMaskA_SOTF | PFAlertMaskA_SOT | PFAlertMaskA_SOCD | PFAlertMaskA_SOCC | PFAlertMaskA_SOV | PFAlertMaskA_SUV)
#define PFAlertMaskB_DEFAULTRAW (PFAlertMaskB_SCDL | PFAlertMaskB_VIMA | PFAlertMaskB_VIMR | PFAlertMaskB_2LVL | PFAlertMaskB_DFETF | PFAlertMaskB_CFETF)
#define PFAlertMaskC_DEFAULTRAW (0x00)
#define PFAlertMaskD_DEFAULTRAW (0x00)
#define EnabledPFA_DEFAULTRAW (0x00)
#define EnabledPFB_DEFAULTRAW (0x00)
#define EnabledPFC_DEFAULTRAW (EnabledPFC_IRMF | EnabledPFC_DRMF | EnabledPFC_OTPF)
#define EnabledPFD_DEFAULTRAW (0x00)
#define FETOptions_DEFAULTRAW (FETOptions_FET_CTRL_EN | FETOptions_HOST_FET_EN | FETOptions_SFET)
#define ChgPumpControl_DEFAULTRAW (ChgPumpControl_CPEN)
#define PrechargeStartVoltage_DEFAULTRAW (0)
#define PrechargeStopVoltage_DEFAULTRAW (0)
#define PredischargeTimeout_DEFAULTRAW (5)
#define PredischargeStopDelta_DEFAULTRAW (50)
#define DsgCurrentThreshold_DEFAULTRAW (100)
#define ChgCurrentThreshold_DEFAULTRAW (50)
#define CheckTime_DEFAULTRAW (5)
#define Cell1Interconnect_DEFAULTRAW (0)
#define Cell2Interconnect_DEFAULTRAW (0)
#define Cell3Interconnect_DEFAULTRAW (0)
#define Cell4Interconnect_DEFAULTRAW (0)
#define Cell5Interconnect_DEFAULTRAW (0)
#define Cell6Interconnect_DEFAULTRAW (0)
#define Cell7Interconnect_DEFAULTRAW (0)
#define Cell8Interconnect_DEFAULTRAW (0)
#define Cell9Interconnect_DEFAULTRAW (0)
#define Cell10Interconnect_DEFAULTRAW (0)
#define Cell11Interconnect_DEFAULTRAW (0)
#define Cell12Interconnect_DEFAULTRAW (0)
#define Cell13Interconnect_DEFAULTRAW (0)
#define Cell14Interconnect_DEFAULTRAW (0)
#define Cell15Interconnect_DEFAULTRAW (0)
#define Cell16Interconnect_DEFAULTRAW (0)
#define MfgStatusInit_DEFAULTRAW (MfgStatusInit_PF_EN)
#define BalancingConfiguration_DEFAULTRAW (0x00)
#define MinCellTemp_DEFAULTRAW (-20)
#define MaxCellTemp_DEFAULTRAW (60)
#define MaxInternalTemp_DEFAULTRAW (70)
#define CellBalanceInterval_DEFAULTRAW (20)
#define CellBalanceMaxCells_DEFAULTRAW (1)
#define CellBalanceMinCellVCharge_DEFAULTRAW (3900)
#define CellBalanceMinDeltaCharge_DEFAULTRAW (40)
#define CellBalanceStopDeltaCharge_DEFAULTRAW (20)
#define CellBalanceMinCellVRelax_DEFAULTRAW (3900)
#define CellBalanceMinDeltaRelax_DEFAULTRAW (40)
#define CellBalanceStopDeltaRelax_DEFAULTRAW (20)
#define ShutdownCellVoltage_DEFAULTRAW (0)
#define ShutdownStackVoltage_DEFAULTRAW (600)
#define LowVShutdownDelay_DEFAULTRAW (1)
#define ShutdownTemperature_DEFAULTRAW (85)
#define ShutdownTemperatureDelay_DEFAULTRAW (5)
#define FETOffDelay_DEFAULTRAW (0)
#define ShutdownCommandDelay_DEFAULTRAW (0)
#define AutoShutdownTime_DEFAULTRAW (0)
#define RAMFailShutdownTime_DEFAULTRAW (5)
#define SleepCurrent_DEFAULTRAW (20)
#define VoltageTime_DEFAULTRAW (5)
#define WakeComparatorCurrent_DEFAULTRAW (500)
#define SleepHysteresisTime_DEFAULTRAW (10)
#define SleepChargerVoltageThreshold_DEFAULTRAW (2000)
#define SleepChargerPACKTOSDelta_DEFAULTRAW (200)
#define ConfigRAMSignature_DEFAULTRAW (0)
#define CUVThreshold_DEFAULTRAW (50)
#define CUVDelay_DEFAULTRAW (74)
#define CUVRecoveryHysteresis_DEFAULTRAW (2)
#define COVThreshold_DEFAULTRAW (86)
#define COVDelay_DEFAULTRAW (74)
#define COVRecoveryHysteresis_DEFAULTRAW (2)
#define COVLLatchLimit_DEFAULTRAW (0)
#define COVLCounterDecDelay_DEFAULTRAW (10)
#define COVLRecoveryTime_DEFAULTRAW (15)
#define OCCThreshold_DEFAULTRAW (2)
#define OCCDelay_DEFAULTRAW (4)
#define OCCRecoveryThreshold_DEFAULTRAW (-200)
#define OCCPACKTOSDelta_DEFAULTRAW (200)
#define OCD1Threshold_DEFAULTRAW (4)
#define OCD1Delay_DEFAULTRAW (1)
#define OCD2Threshold_DEFAULTRAW (3)
#define OCD2Delay_DEFAULTRAW (7)
#define SCDThreshold_DEFAULTRAW (SCDThreshold_10mV)
#define SCDDelay_DEFAULTRAW (2)
#define SCDRecoveryTime_DEFAULTRAW (5)
#define OCD3Threshold_DEFAULTRAW (-4000)
#define OCD3Delay_DEFAULTRAW (2)
#define OCDRecoveryThreshold_DEFAULTRAW (200)
#define OCDLLatchLimit_DEFAULTRAW (0)
#define OCDLCounterDecDelay_DEFAULTRAW (10)
#define OCDLRecoveryTime_DEFAULTRAW (15)
#define OCDLRecoveryThreshold_DEFAULTRAW (200)
#define SCDLLatchLimit_DEFAULTRAW (0)
#define SCDLCounterDecDelay_DEFAULTRAW (10)
#define SCDLRecoveryTime_DEFAULTRAW (15)
#define SCDLRecoveryThreshold_DEFAULTRAW (200)
#define OTCThreshold_DEFAULTRAW (55)
#define OTCDelay_DEFAULTRAW (2)
#define OTCRecovery_DEFAULTRAW (50)
#define OTDThreshold_DEFAULTRAW (60)
#define OTDDelay_DEFAULTRAW (2)
#define OTDRecovery_DEFAULTRAW (55)
#define OTFThreshold_DEFAULTRAW (80)
#define OTFDelay_DEFAULTRAW (2)
#define OTFRecovery_DEFAULTRAW (65)
#define OTINTThreshold_DEFAULTRAW (85)
#define OTINTDelay_DEFAULTRAW (2)
#define OTINTRecovery_DEFAULTRAW (80)
#define UTCThreshold_DEFAULTRAW (0)
#define UTCDelay_DEFAULTRAW (2)
#define UTCRecovery_DEFAULTRAW (5)
#define UTDThreshold_DEFAULTRAW (0)
#define UTDDelay_DEFAULTRAW (2)
#define UTDRecovery_DEFAULTRAW (5)
#define UTINTThreshold_DEFAULTRAW (-20)
#define UTINTDelay_DEFAULTRAW (2)
#define UTINTRecovery_DEFAULTRAW (-15)
#define ProtectionsRecoveryTime_DEFAULTRAW (3)
#define HWDDelay_DEFAULTRAW (60)
#define LoadDetectActiveTime_DEFAULTRAW (0)
#define LoadDetectRetryDelay_DEFAULTRAW (50)
#define LoadDetectTimeout_DEFAULTRAW (1)
#define PTOChargeThreshold_DEFAULTRAW (250)
#define PTODelay_DEFAULTRAW (1800)
#define PTOReset_DEFAULTRAW (2)
#define CUDEPThreshold_DEFAULTRAW (1500)
#define CUDEPDelay_DEFAULTRAW (2)
#define SUVThreshold_DEFAULTRAW (2200)
#define SUVDelay_DEFAULTRAW (5)
#define SOVThreshold_DEFAULTRAW (4500)
#define SOVDelay_DEFAULTRAW (5)
#define TOSSThreshold_DEFAULTRAW (500)
#define TOSSDelay_DEFAULTRAW (5)
#define SOCCThreshold_DEFAULTRAW (10000)
#define SOCCDelay_DEFAULTRAW (5)
#define SOCDThreshold_DEFAULTRAW (-32000)
#define SOCDDelay_DEFAULTRAW (5)
#define SOTThreshold_DEFAULTRAW (65)
#define SOTDelay_DEFAULTRAW (5)
#define SOTFThreshold_DEFAULTRAW (85)
#define SOTFDelay_DEFAULTRAW (5)
#define VIMRCheckVoltage_DEFAULTRAW (3500)
#define VIMRMaxRelaxCurrent_DEFAULTRAW (10)
#define VIMRThreshold_DEFAULTRAW (500)
#define VIMRDelay_DEFAULTRAW (5)
#define VIMRRelaxMinDuration_DEFAULTRAW (100)
#define VIMACheckVoltage_DEFAULTRAW (3700)
#define VIMAMinActiveCurrent_DEFAULTRAW (50)
#define VIMAThreshold_DEFAULTRAW (200)
#define VIMADelay_DEFAULTRAW (5)
#define CFETFOFFThreshold_DEFAULTRAW (20)
#define CFETFOFFDelay_DEFAULTRAW (5)
#define DFETFOFFThreshold_DEFAULTRAW (-20)
#define DFETFOFFDelay_DEFAULTRAW (5)
#define VSSFFailThreshold_DEFAULTRAW (100)
#define VSSFDelay_DEFAULTRAW (5)
#define PF2LVLDelay_DEFAULTRAW (5)
#define LFOFDelay_DEFAULTRAW (5)
#define HWMXDelay_DEFAULTRAW (5)
#define SecuritySettings_DEFAULTRAW (0x00)
#define UnsealKeyStep1_DEFAULTRAW (0x0414)
#define UnsealKeyStep2_DEFAULTRAW (0x3672)
#define FullAccessKeyStep1_DEFAULTRAW (0xFFFF)
#define FullAccessKeyStep2_DEFAULTRAW (0xFFFF)

// #define Cell1Gain_UNIT dimensionless
// #define Cell2Gain_UNIT dimensionless
// #define Cell3Gain_UNIT dimensionless
// #define Cell4Gain_UNIT dimensionless
// #define Cell5Gain_UNIT dimensionless
// #define Cell6Gain_UNIT dimensionless
// #define Cell7Gain_UNIT dimensionless
// #define Cell8Gain_UNIT dimensionless
// #define Cell9Gain_UNIT dimensionless
// #define Cell10Gain_UNIT dimensionless
// #define Cell11Gain_UNIT dimensionless
// #define Cell12Gain_UNIT dimensionless
// #define Cell13Gain_UNIT dimensionless
// #define Cell14Gain_UNIT dimensionless
// #define Cell15Gain_UNIT dimensionless
// #define Cell16Gain_UNIT dimensionless
// #define PackGain_UNIT dimensionless
// #define TOSGain_UNIT dimensionless
// #define LDGain_UNIT dimensionless
// #define ADCGain_UNIT dimensionless
// #define CCGain_UNIT dimensionless
// #define CapacityGain_UNIT dimensionless
#define VcellOffset_UNIT mV
#define VdivOffset_UNIT userV
// #define CoulombCounterOffsetSamples_UNIT dimensionless
// #define BoardOffset_UNIT dimensionless
#define InternalTempOffset_UNIT degC
#define CFETOFFTempOffset_UNIT degC
#define DFETOFFTempOffset_UNIT degC
#define ALERTTempOffset_UNIT degC
#define TS1TempOffset_UNIT degC
#define TS2TempOffset_UNIT degC
#define TS3TempOffset_UNIT degC
#define HDQTempOffset_UNIT degC
#define DCHGTempOffset_UNIT degC
#define DDSGTempOffset_UNIT degC
// #define IntGain_UNIT dimensionless
// #define Intbaseoffset_UNIT dimensionless
// #define IntMaximumAD_UNIT dimensionless
#define IntMaximumTemp_UNIT K
// #define T18kCoeffa1_UNIT dimensionless
// #define T18kCoeffa2_UNIT dimensionless
// #define T18kCoeffa3_UNIT dimensionless
// #define T18kCoeffa4_UNIT dimensionless
// #define T18kCoeffa5_UNIT dimensionless
// #define T18kCoeffb1_UNIT dimensionless
// #define T18kCoeffb2_UNIT dimensionless
// #define T18kCoeffb3_UNIT dimensionless
// #define T18kCoeffb4_UNIT dimensionless
// #define T18kAdc0_UNIT dimensionless
// #define T180kCoeffa1_UNIT dimensionless
// #define T180kCoeffa2_UNIT dimensionless
// #define T180kCoeffa3_UNIT dimensionless
// #define T180kCoeffa4_UNIT dimensionless
// #define T180kCoeffa5_UNIT dimensionless
// #define T180kCoeffb1_UNIT dimensionless
// #define T180kCoeffb2_UNIT dimensionless
// #define T180kCoeffb3_UNIT dimensionless
// #define T180kCoeffb4_UNIT dimensionless
// #define T180kAdc0_UNIT dimensionless
// #define CustomCoeffa1_UNIT dimensionless
// #define CustomCoeffa2_UNIT dimensionless
// #define CustomCoeffa3_UNIT dimensionless
// #define CustomCoeffa4_UNIT dimensionless
// #define CustomCoeffa5_UNIT dimensionless
// #define CustomCoeffb1_UNIT dimensionless
// #define CustomCoeffb2_UNIT dimensionless
// #define CustomCoeffb3_UNIT dimensionless
// #define CustomCoeffb4_UNIT dimensionless
// #define CustomRc0_UNIT dimensionless
// #define CustomAdc0_UNIT dimensionless
#define CoulombCounterDeadband_UNIT nV
// #define CUVThresholdOverride_UNIT Raw
// #define COVThresholdOverride_UNIT Raw
#define MinBlowFuseVoltage_UNIT mV
#define FuseBlowTimeout_UNIT s
// #define PowerConfig_UNIT Raw
// #define REG12Config_UNIT Raw
// #define REG0Config_UNIT Raw
// #define HWDRegulatorOptions_UNIT Raw
// #define CommType_UNIT dimensionless
// #define I2CAddress_UNIT dimensionless
// #define SPIConfiguration_UNIT dimensionless
#define CommIdleTime_UNIT s
// #define CFETOFFPinConfig_UNIT Raw
// #define DFETOFFPinConfig_UNIT Raw
// #define ALERTPinConfig_UNIT Raw
// #define TS1Config_UNIT Raw
// #define TS2Config_UNIT Raw
// #define TS3Config_UNIT Raw
// #define HDQPinConfig_UNIT Raw
// #define DCHGPinConfig_UNIT Raw
// #define DDSGPinConfig_UNIT Raw
// #define DAConfiguration_UNIT Raw
// #define VCellMode_UNIT Raw
#define CC3Samples_UNIT Num
// #define ProtectionConfiguration_UNIT Raw
// #define EnabledProtectionsA_UNIT Raw
// #define EnabledProtectionsB_UNIT Raw
// #define EnabledProtectionsC_UNIT Raw
// #define CHGFETProtectionsA_UNIT Raw
// #define CHGFETProtectionsB_UNIT Raw
// #define CHGFETProtectionsC_UNIT Raw
// #define DSGFETProtectionsA_UNIT Raw
// #define DSGFETProtectionsB_UNIT Raw
// #define DSGFETProtectionsC_UNIT Raw
#define BodyDiodeThreshold_UNIT mA
// #define DefaultAlarmMask_UNIT Raw
// #define SFAlertMaskA_UNIT Raw
// #define SFAlertMaskB_UNIT Raw
// #define SFAlertMaskC_UNIT Raw
// #define PFAlertMaskA_UNIT Raw
// #define PFAlertMaskB_UNIT Raw
// #define PFAlertMaskC_UNIT Raw
// #define PFAlertMaskD_UNIT Raw
// #define EnabledPFA_UNIT Raw
// #define EnabledPFB_UNIT Raw
// #define EnabledPFC_UNIT Raw
// #define EnabledPFD_UNIT Raw
// #define FETOptions_UNIT Raw
// #define ChgPumpControl_UNIT Raw
#define PrechargeStartVoltage_UNIT mV
#define PrechargeStopVoltage_UNIT mV
#define PredischargeTimeout_UNIT ms
#define PredischargeStopDelta_UNIT mV
#define DsgCurrentThreshold_UNIT userA
#define ChgCurrentThreshold_UNIT userA
#define CheckTime_UNIT s
#define Cell1Interconnect_UNIT mOhm
#define Cell2Interconnect_UNIT mOhm
#define Cell3Interconnect_UNIT mOhm
#define Cell4Interconnect_UNIT mOhm
#define Cell5Interconnect_UNIT mOhm
#define Cell6Interconnect_UNIT mOhm
#define Cell7Interconnect_UNIT mOhm
#define Cell8Interconnect_UNIT mOhm
#define Cell9Interconnect_UNIT mOhm
#define Cell10Interconnect_UNIT mOhm
#define Cell11Interconnect_UNIT mOhm
#define Cell12Interconnect_UNIT mOhm
#define Cell13Interconnect_UNIT mOhm
#define Cell14Interconnect_UNIT mOhm
#define Cell15Interconnect_UNIT mOhm
#define Cell16Interconnect_UNIT mOhm
// #define MfgStatusInit_UNIT Raw
// #define BalancingConfiguration_UNIT Raw
#define MinCellTemp_UNIT degC
#define MaxCellTemp_UNIT degC
#define MaxInternalTemp_UNIT degC
#define CellBalanceInterval_UNIT s
#define CellBalanceMaxCells_UNIT Num
#define CellBalanceMinCellVCharge_UNIT mV
#define CellBalanceMinDeltaCharge_UNIT mV
#define CellBalanceStopDeltaCharge_UNIT mV
#define CellBalanceMinCellVRelax_UNIT mV
#define CellBalanceMinDeltaRelax_UNIT mV
#define CellBalanceStopDeltaRelax_UNIT mV
#define ShutdownCellVoltage_UNIT mV
#define ShutdownStackVoltage_UNIT mV
#define LowVShutdownDelay_UNIT s
#define ShutdownTemperature_UNIT degC
#define ShutdownTemperatureDelay_UNIT s
#define FETOffDelay_UNIT s
#define ShutdownCommandDelay_UNIT s
#define AutoShutdownTime_UNIT min
#define RAMFailShutdownTime_UNIT s
#define SleepCurrent_UNIT mA
#define VoltageTime_UNIT s
#define WakeComparatorCurrent_UNIT mA
#define SleepHysteresisTime_UNIT s
#define SleepChargerVoltageThreshold_UNIT mV
#define SleepChargerPACKTOSDelta_UNIT mV
// #define ConfigRAMSignature_UNIT Raw
#define CUVThreshold_UNIT mV
#define CUVDelay_UNIT ms
#define CUVRecoveryHysteresis_UNIT mV
#define COVThreshold_UNIT mV
#define COVDelay_UNIT ms
#define COVRecoveryHysteresis_UNIT mV
// #define COVLLatchLimit_UNIT dimensionless
#define COVLCounterDecDelay_UNIT s
#define COVLRecoveryTime_UNIT s
#define OCCThreshold_UNIT mV
#define OCCDelay_UNIT ms
#define OCCRecoveryThreshold_UNIT mA
#define OCCPACKTOSDelta_UNIT mV
#define OCD1Threshold_UNIT mV
#define OCD1Delay_UNIT ms
#define OCD2Threshold_UNIT mV
#define OCD2Delay_UNIT ms
// #define SCDThreshold_UNIT dimensionless
#define SCDDelay_UNIT us
#define SCDRecoveryTime_UNIT s
#define OCD3Threshold_UNIT userA
#define OCD3Delay_UNIT s
#define OCDRecoveryThreshold_UNIT mA
// #define OCDLLatchLimit_UNIT dimensionless
#define OCDLCounterDecDelay_UNIT s
#define OCDLRecoveryTime_UNIT s
#define OCDLRecoveryThreshold_UNIT mA
// #define SCDLLatchLimit_UNIT dimensionless
#define SCDLCounterDecDelay_UNIT s
#define SCDLRecoveryTime_UNIT s
#define SCDLRecoveryThreshold_UNIT mA
#define OTCThreshold_UNIT degC
#define OTCDelay_UNIT s
#define OTCRecovery_UNIT degC
#define OTDThreshold_UNIT degC
#define OTDDelay_UNIT s
#define OTDRecovery_UNIT degC
#define OTFThreshold_UNIT degC
#define OTFDelay_UNIT s
#define OTFRecovery_UNIT degC
#define OTINTThreshold_UNIT degC
#define OTINTDelay_UNIT s
#define OTINTRecovery_UNIT degC
#define UTCThreshold_UNIT degC
#define UTCDelay_UNIT s
#define UTCRecovery_UNIT degC
#define UTDThreshold_UNIT degC
#define UTDDelay_UNIT s
#define UTDRecovery_UNIT degC
#define UTINTThreshold_UNIT degC
#define UTINTDelay_UNIT s
#define UTINTRecovery_UNIT degC
#define ProtectionsRecoveryTime_UNIT s
#define HWDDelay_UNIT s
#define LoadDetectActiveTime_UNIT s
#define LoadDetectRetryDelay_UNIT s
#define LoadDetectTimeout_UNIT hrs
#define PTOChargeThreshold_UNIT mA
#define PTODelay_UNIT s
#define PTOReset_UNIT userAh
#define CUDEPThreshold_UNIT mV
#define CUDEPDelay_UNIT s
#define SUVThreshold_UNIT mV
#define SUVDelay_UNIT s
#define SOVThreshold_UNIT mV
#define SOVDelay_UNIT s
#define TOSSThreshold_UNIT mV
#define TOSSDelay_UNIT s
#define SOCCThreshold_UNIT userA
#define SOCCDelay_UNIT s
#define SOCDThreshold_UNIT userA
#define SOCDDelay_UNIT s
#define SOTThreshold_UNIT degC
#define SOTDelay_UNIT s
#define SOTFThreshold_UNIT degC
#define SOTFDelay_UNIT s
#define VIMRCheckVoltage_UNIT mV
#define VIMRMaxRelaxCurrent_UNIT mA
#define VIMRThreshold_UNIT mV
#define VIMRDelay_UNIT s
#define VIMRRelaxMinDuration_UNIT s
#define VIMACheckVoltage_UNIT mV
#define VIMAMinActiveCurrent_UNIT mA
#define VIMAThreshold_UNIT mV
#define VIMADelay_UNIT s
#define CFETFOFFThreshold_UNIT mA
#define CFETFOFFDelay_UNIT s
#define DFETFOFFThreshold_UNIT mA
#define DFETFOFFDelay_UNIT s
// #define VSSFFailThreshold_UNIT dimensionless
#define VSSFDelay_UNIT s
#define PF2LVLDelay_UNIT s
#define LFOFDelay_UNIT s
#define HWMXDelay_UNIT s
// #define SecuritySettings_UNIT Raw
// #define UnsealKeyStep1_UNIT Raw
// #define UnsealKeyStep2_UNIT Raw
// #define FullAccessKeyStep1_UNIT Raw
// #define FullAccessKeyStep2_UNIT Raw

#define Cell1Gain_UNITMULOFF 1
#define Cell2Gain_UNITMULOFF 1
#define Cell3Gain_UNITMULOFF 1
#define Cell4Gain_UNITMULOFF 1
#define Cell5Gain_UNITMULOFF 1
#define Cell6Gain_UNITMULOFF 1
#define Cell7Gain_UNITMULOFF 1
#define Cell8Gain_UNITMULOFF 1
#define Cell9Gain_UNITMULOFF 1
#define Cell10Gain_UNITMULOFF 1
#define Cell11Gain_UNITMULOFF 1
#define Cell12Gain_UNITMULOFF 1
#define Cell13Gain_UNITMULOFF 1
#define Cell14Gain_UNITMULOFF 1
#define Cell15Gain_UNITMULOFF 1
#define Cell16Gain_UNITMULOFF 1
#define PackGain_UNITMULOFF 1
#define TOSGain_UNITMULOFF 1
#define LDGain_UNITMULOFF 1
#define ADCGain_UNITMULOFF 1
#define CCGain_UNITMULOFF 1
#define CapacityGain_UNITMULOFF 1
#define VcellOffset_UNITMULOFF 1
#define VdivOffset_UNITMULOFF 1
#define CoulombCounterOffsetSamples_UNITMULOFF 1
#define BoardOffset_UNITMULOFF 1
#define InternalTempOffset_UNITMULOFF 0.1F
#define CFETOFFTempOffset_UNITMULOFF 0.1F
#define DFETOFFTempOffset_UNITMULOFF 0.1F
#define ALERTTempOffset_UNITMULOFF 0.1F
#define TS1TempOffset_UNITMULOFF 0.1F
#define TS2TempOffset_UNITMULOFF 0.1F
#define TS3TempOffset_UNITMULOFF 0.1F
#define HDQTempOffset_UNITMULOFF 0.1F
#define DCHGTempOffset_UNITMULOFF 0.1F
#define DDSGTempOffset_UNITMULOFF 0.1F
#define IntGain_UNITMULOFF 1
#define Intbaseoffset_UNITMULOFF 1
#define IntMaximumAD_UNITMULOFF 1
#define IntMaximumTemp_UNITMULOFF 0.1F
#define T18kCoeffa1_UNITMULOFF 1
#define T18kCoeffa2_UNITMULOFF 1
#define T18kCoeffa3_UNITMULOFF 1
#define T18kCoeffa4_UNITMULOFF 1
#define T18kCoeffa5_UNITMULOFF 1
#define T18kCoeffb1_UNITMULOFF 1
#define T18kCoeffb2_UNITMULOFF 1
#define T18kCoeffb3_UNITMULOFF 1
#define T18kCoeffb4_UNITMULOFF 1
#define T18kAdc0_UNITMULOFF 1
#define T180kCoeffa1_UNITMULOFF 1
#define T180kCoeffa2_UNITMULOFF 1
#define T180kCoeffa3_UNITMULOFF 1
#define T180kCoeffa4_UNITMULOFF 1
#define T180kCoeffa5_UNITMULOFF 1
#define T180kCoeffb1_UNITMULOFF 1
#define T180kCoeffb2_UNITMULOFF 1
#define T180kCoeffb3_UNITMULOFF 1
#define T180kCoeffb4_UNITMULOFF 1
#define T180kAdc0_UNITMULOFF 1
#define CustomCoeffa1_UNITMULOFF 1
#define CustomCoeffa2_UNITMULOFF 1
#define CustomCoeffa3_UNITMULOFF 1
#define CustomCoeffa4_UNITMULOFF 1
#define CustomCoeffa5_UNITMULOFF 1
#define CustomCoeffb1_UNITMULOFF 1
#define CustomCoeffb2_UNITMULOFF 1
#define CustomCoeffb3_UNITMULOFF 1
#define CustomCoeffb4_UNITMULOFF 1
#define CustomRc0_UNITMULOFF 1
#define CustomAdc0_UNITMULOFF 1
#define CoulombCounterDeadband_UNITMULOFF 234
#define CUVThresholdOverride_UNITMULOFF 1
#define COVThresholdOverride_UNITMULOFF 1
#define MinBlowFuseVoltage_UNITMULOFF 10
#define FuseBlowTimeout_UNITMULOFF 1
#define PowerConfig_UNITMULOFF 1
#define REG12Config_UNITMULOFF 1
#define REG0Config_UNITMULOFF 1
#define HWDRegulatorOptions_UNITMULOFF 1
#define CommType_UNITMULOFF 1
#define I2CAddress_UNITMULOFF 1
#define SPIConfiguration_UNITMULOFF 1
#define CommIdleTime_UNITMULOFF 1
#define CFETOFFPinConfig_UNITMULOFF 1
#define DFETOFFPinConfig_UNITMULOFF 1
#define ALERTPinConfig_UNITMULOFF 1
#define TS1Config_UNITMULOFF 1
#define TS2Config_UNITMULOFF 1
#define TS3Config_UNITMULOFF 1
#define HDQPinConfig_UNITMULOFF 1
#define DCHGPinConfig_UNITMULOFF 1
#define DDSGPinConfig_UNITMULOFF 1
#define DAConfiguration_UNITMULOFF 1
#define VCellMode_UNITMULOFF 1
#define CC3Samples_UNITMULOFF 1
#define ProtectionConfiguration_UNITMULOFF 1
#define EnabledProtectionsA_UNITMULOFF 1
#define EnabledProtectionsB_UNITMULOFF 1
#define EnabledProtectionsC_UNITMULOFF 1
#define CHGFETProtectionsA_UNITMULOFF 1
#define CHGFETProtectionsB_UNITMULOFF 1
#define CHGFETProtectionsC_UNITMULOFF 1
#define DSGFETProtectionsA_UNITMULOFF 1
#define DSGFETProtectionsB_UNITMULOFF 1
#define DSGFETProtectionsC_UNITMULOFF 1
#define BodyDiodeThreshold_UNITMULOFF 1
#define DefaultAlarmMask_UNITMULOFF 1
#define SFAlertMaskA_UNITMULOFF 1
#define SFAlertMaskB_UNITMULOFF 1
#define SFAlertMaskC_UNITMULOFF 1
#define PFAlertMaskA_UNITMULOFF 1
#define PFAlertMaskB_UNITMULOFF 1
#define PFAlertMaskC_UNITMULOFF 1
#define PFAlertMaskD_UNITMULOFF 1
#define EnabledPFA_UNITMULOFF 1
#define EnabledPFB_UNITMULOFF 1
#define EnabledPFC_UNITMULOFF 1
#define EnabledPFD_UNITMULOFF 1
#define FETOptions_UNITMULOFF 1
#define ChgPumpControl_UNITMULOFF 1
#define PrechargeStartVoltage_UNITMULOFF 1
#define PrechargeStopVoltage_UNITMULOFF 1
#define PredischargeTimeout_UNITMULOFF 10
#define PredischargeStopDelta_UNITMULOFF 10
#define DsgCurrentThreshold_UNITMULOFF 1
#define ChgCurrentThreshold_UNITMULOFF 1
#define CheckTime_UNITMULOFF 1
#define Cell1Interconnect_UNITMULOFF 1
#define Cell2Interconnect_UNITMULOFF 1
#define Cell3Interconnect_UNITMULOFF 1
#define Cell4Interconnect_UNITMULOFF 1
#define Cell5Interconnect_UNITMULOFF 1
#define Cell6Interconnect_UNITMULOFF 1
#define Cell7Interconnect_UNITMULOFF 1
#define Cell8Interconnect_UNITMULOFF 1
#define Cell9Interconnect_UNITMULOFF 1
#define Cell10Interconnect_UNITMULOFF 1
#define Cell11Interconnect_UNITMULOFF 1
#define Cell12Interconnect_UNITMULOFF 1
#define Cell13Interconnect_UNITMULOFF 1
#define Cell14Interconnect_UNITMULOFF 1
#define Cell15Interconnect_UNITMULOFF 1
#define Cell16Interconnect_UNITMULOFF 1
#define MfgStatusInit_UNITMULOFF 1
#define BalancingConfiguration_UNITMULOFF 1
#define MinCellTemp_UNITMULOFF 1
#define MaxCellTemp_UNITMULOFF 1
#define MaxInternalTemp_UNITMULOFF 1
#define CellBalanceInterval_UNITMULOFF 1
#define CellBalanceMaxCells_UNITMULOFF 1
#define CellBalanceMinCellVCharge_UNITMULOFF 1
#define CellBalanceMinDeltaCharge_UNITMULOFF 1
#define CellBalanceStopDeltaCharge_UNITMULOFF 1
#define CellBalanceMinCellVRelax_UNITMULOFF 1
#define CellBalanceMinDeltaRelax_UNITMULOFF 1
#define CellBalanceStopDeltaRelax_UNITMULOFF 1
#define ShutdownCellVoltage_UNITMULOFF 1
#define ShutdownStackVoltage_UNITMULOFF 10
#define LowVShutdownDelay_UNITMULOFF 1
#define ShutdownTemperature_UNITMULOFF 1
#define ShutdownTemperatureDelay_UNITMULOFF 1
#define FETOffDelay_UNITMULOFF 0.25F
#define ShutdownCommandDelay_UNITMULOFF 0.25F
#define AutoShutdownTime_UNITMULOFF 1
#define RAMFailShutdownTime_UNITMULOFF 1
#define SleepCurrent_UNITMULOFF 1
#define VoltageTime_UNITMULOFF 1
#define WakeComparatorCurrent_UNITMULOFF 1
#define SleepHysteresisTime_UNITMULOFF 1
#define SleepChargerVoltageThreshold_UNITMULOFF 10
#define SleepChargerPACKTOSDelta_UNITMULOFF 10
#define ConfigRAMSignature_UNITMULOFF 1
#define CUVThreshold_UNITMULOFF 50.6F
#define CUVDelay_UNITMULOFF 3.3F - 2 /*3.3 ms with a 6.6ms offset*/
#define CUVRecoveryHysteresis_UNITMULOFF 50.6F
#define COVThreshold_UNITMULOFF 50.6F
#define COVDelay_UNITMULOFF 3.3F - 2 /*3.3 ms with a 6.6ms offset*/
#define COVRecoveryHysteresis_UNITMULOFF 50.6F
#define COVLLatchLimit_UNITMULOFF 1
#define COVLCounterDecDelay_UNITMULOFF 1
#define COVLRecoveryTime_UNITMULOFF 1
#define OCCThreshold_UNITMULOFF 2
#define OCCDelay_UNITMULOFF 3.3F - 2 /*3.3 ms with a 6.6ms offset*/
#define OCCRecoveryThreshold_UNITMULOFF 1
#define OCCPACKTOSDelta_UNITMULOFF 10
#define OCD1Threshold_UNITMULOFF 2
#define OCD1Delay_UNITMULOFF 3.3F - 2 /*3.3 ms with a 6.6ms offset*/
#define OCD2Threshold_UNITMULOFF 2
#define OCD2Delay_UNITMULOFF 3.3F - 2 /*3.3 ms with a 6.6ms offset*/
#define SCDThreshold_UNITMULOFF 1
#define SCDDelay_UNITMULOFF 15 + 1 /*(value - 1) * 15 µs*/
#define SCDRecoveryTime_UNITMULOFF 1
#define OCD3Threshold_UNITMULOFF 1
#define OCD3Delay_UNITMULOFF 1
#define OCDRecoveryThreshold_UNITMULOFF 1
#define OCDLLatchLimit_UNITMULOFF 1
#define OCDLCounterDecDelay_UNITMULOFF 1
#define OCDLRecoveryTime_UNITMULOFF 1
#define OCDLRecoveryThreshold_UNITMULOFF 1
#define SCDLLatchLimit_UNITMULOFF 1
#define SCDLCounterDecDelay_UNITMULOFF 1
#define SCDLRecoveryTime_UNITMULOFF 1
#define SCDLRecoveryThreshold_UNITMULOFF 1
#define OTCThreshold_UNITMULOFF 1
#define OTCDelay_UNITMULOFF 1
#define OTCRecovery_UNITMULOFF 1
#define OTDThreshold_UNITMULOFF 1
#define OTDDelay_UNITMULOFF 1
#define OTDRecovery_UNITMULOFF 1
#define OTFThreshold_UNITMULOFF 1
#define OTFDelay_UNITMULOFF 1
#define OTFRecovery_UNITMULOFF 1
#define OTINTThreshold_UNITMULOFF 1
#define OTINTDelay_UNITMULOFF 1
#define OTINTRecovery_UNITMULOFF 1
#define UTCThreshold_UNITMULOFF 1
#define UTCDelay_UNITMULOFF 1
#define UTCRecovery_UNITMULOFF 1
#define UTDThreshold_UNITMULOFF 1
#define UTDDelay_UNITMULOFF 1
#define UTDRecovery_UNITMULOFF 1
#define UTINTThreshold_UNITMULOFF 1
#define UTINTDelay_UNITMULOFF 1
#define UTINTRecovery_UNITMULOFF 1
#define ProtectionsRecoveryTime_UNITMULOFF 1
#define HWDDelay_UNITMULOFF 1
#define LoadDetectActiveTime_UNITMULOFF 1
#define LoadDetectRetryDelay_UNITMULOFF 1
#define LoadDetectTimeout_UNITMULOFF 1
#define PTOChargeThreshold_UNITMULOFF 1
#define PTODelay_UNITMULOFF 1
#define PTOReset_UNITMULOFF 1
#define CUDEPThreshold_UNITMULOFF 1
#define CUDEPDelay_UNITMULOFF 1
#define SUVThreshold_UNITMULOFF 1
#define SUVDelay_UNITMULOFF 1
#define SOVThreshold_UNITMULOFF 1
#define SOVDelay_UNITMULOFF 1
#define TOSSThreshold_UNITMULOFF 1
#define TOSSDelay_UNITMULOFF 1
#define SOCCThreshold_UNITMULOFF 1
#define SOCCDelay_UNITMULOFF 1
#define SOCDThreshold_UNITMULOFF 1
#define SOCDDelay_UNITMULOFF 1
#define SOTThreshold_UNITMULOFF 1
#define SOTDelay_UNITMULOFF 1
#define SOTFThreshold_UNITMULOFF 1
#define SOTFDelay_UNITMULOFF 1
#define VIMRCheckVoltage_UNITMULOFF 1
#define VIMRMaxRelaxCurrent_UNITMULOFF 1
#define VIMRThreshold_UNITMULOFF 1
#define VIMRDelay_UNITMULOFF 1
#define VIMRRelaxMinDuration_UNITMULOFF 1
#define VIMACheckVoltage_UNITMULOFF 1
#define VIMAMinActiveCurrent_UNITMULOFF 1
#define VIMAThreshold_UNITMULOFF 1
#define VIMADelay_UNITMULOFF 1
#define CFETFOFFThreshold_UNITMULOFF 1
#define CFETFOFFDelay_UNITMULOFF 1
#define DFETFOFFThreshold_UNITMULOFF 1
#define DFETFOFFDelay_UNITMULOFF 1
#define VSSFFailThreshold_UNITMULOFF 1
#define VSSFDelay_UNITMULOFF 1
#define PF2LVLDelay_UNITMULOFF 1
#define LFOFDelay_UNITMULOFF 1
#define HWMXDelay_UNITMULOFF 1
#define SecuritySettings_UNITMULOFF 1
#define UnsealKeyStep1_UNITMULOFF 1
#define UnsealKeyStep2_UNITMULOFF 1
#define FullAccessKeyStep1_UNITMULOFF 1
#define FullAccessKeyStep2_UNITMULOFF 1

#define BQ769X2_STR_(s) #s
#define BQ769X2_STR(s) BQ769X2_STR_(s)

#define BQ769X2_CONV(v, mul) ((uint32_t)((v) / 1.0F / mul + 0.5F))

#define BQ769X2_APPLY_CONV(register_addr, register_name, reg_v)                 \
    static_assert((register_name##MIN) <= (reg_v), #register_name "MIN ERROR"); \
    static_assert((reg_v) <= (register_name##MAX), #register_name "MAX ERROR"); \
    bq769x2_set_register(register_addr, reg_v, register_name##T)

#define BQ769X2_APPLY_FLOAT(register_name, reg_v)                                \
    static_assert((register_name##_MIN) <= (reg_v), #register_name "MIN ERROR"); \
    static_assert((reg_v) <= (register_name##_MAX), #register_name "MAX ERROR"); \
    bq769x2_set_register(register_name, bq769x2_float_to_uint32_t(reg_v), register_name##_T)

#define BQ769X2_APPLY(register_name, v) \
    BQ769X2_APPLY_CONV(register_name, register_name##_, BQ769X2_CONV((v), register_name##_UNITMULOFF))

#define BQ769X2_APPLY_NOCONV(register_name, v) \
    BQ769X2_APPLY_CONV(register_name, register_name##_, v)

#define SET_Cell1Gain_dimensionless(dimensionless) BQ769X2_APPLY_NOCONV(Cell1Gain, dimensionless)
#define SET_Cell2Gain_dimensionless(dimensionless) BQ769X2_APPLY_NOCONV(Cell2Gain, dimensionless)
#define SET_Cell3Gain_dimensionless(dimensionless) BQ769X2_APPLY_NOCONV(Cell3Gain, dimensionless)
#define SET_Cell4Gain_dimensionless(dimensionless) BQ769X2_APPLY_NOCONV(Cell4Gain, dimensionless)
#define SET_Cell5Gain_dimensionless(dimensionless) BQ769X2_APPLY_NOCONV(Cell5Gain, dimensionless)
#define SET_Cell6Gain_dimensionless(dimensionless) BQ769X2_APPLY_NOCONV(Cell6Gain, dimensionless)
#define SET_Cell7Gain_dimensionless(dimensionless) BQ769X2_APPLY_NOCONV(Cell7Gain, dimensionless)
#define SET_Cell8Gain_dimensionless(dimensionless) BQ769X2_APPLY_NOCONV(Cell8Gain, dimensionless)
#define SET_Cell9Gain_dimensionless(dimensionless) BQ769X2_APPLY_NOCONV(Cell9Gain, dimensionless)
#define SET_Cell10Gain_dimensionless(dimensionless) BQ769X2_APPLY_NOCONV(Cell10Gain, dimensionless)
#define SET_Cell11Gain_dimensionless(dimensionless) BQ769X2_APPLY_NOCONV(Cell11Gain, dimensionless)
#define SET_Cell12Gain_dimensionless(dimensionless) BQ769X2_APPLY_NOCONV(Cell12Gain, dimensionless)
#define SET_Cell13Gain_dimensionless(dimensionless) BQ769X2_APPLY_NOCONV(Cell13Gain, dimensionless)
#define SET_Cell14Gain_dimensionless(dimensionless) BQ769X2_APPLY_NOCONV(Cell14Gain, dimensionless)
#define SET_Cell15Gain_dimensionless(dimensionless) BQ769X2_APPLY_NOCONV(Cell15Gain, dimensionless)
#define SET_Cell16Gain_dimensionless(dimensionless) BQ769X2_APPLY_NOCONV(Cell16Gain, dimensionless)
#define SET_PackGain_dimensionless(dimensionless) BQ769X2_APPLY_NOCONV(PackGain, dimensionless)
#define SET_TOSGain_dimensionless(dimensionless) BQ769X2_APPLY_NOCONV(TOSGain, dimensionless)
#define SET_LDGain_dimensionless(dimensionless) BQ769X2_APPLY_NOCONV(LDGain, dimensionless)
#define SET_ADCGain_dimensionless(dimensionless) BQ769X2_APPLY_NOCONV(ADCGain, dimensionless)
#define SET_CCGain_float(float_) BQ769X2_APPLY_FLOAT(CCGain, float_)
#define SET_CapacityGain_float(float_) BQ769X2_APPLY_FLOAT(CapacityGain, float_)
#define SET_VcellOffset_mV(mV) BQ769X2_APPLY_NOCONV(VcellOffset, mV)
#define SET_VdivOffset_userV(userV) BQ769X2_APPLY_NOCONV(VdivOffset, userV)
#define SET_CoulombCounterOffsetSamples_dimensionless(dimensionless) BQ769X2_APPLY_NOCONV(CoulombCounterOffsetSamples, dimensionless)
#define SET_BoardOffset_dimensionless(dimensionless) BQ769X2_APPLY_NOCONV(BoardOffset, dimensionless)
#define SET_InternalTempOffset_degC(degC) BQ769X2_APPLY(InternalTempOffset, degC)
#define SET_CFETOFFTempOffset_degC(degC) BQ769X2_APPLY(CFETOFFTempOffset, degC)
#define SET_DFETOFFTempOffset_degC(degC) BQ769X2_APPLY(DFETOFFTempOffset, degC)
#define SET_ALERTTempOffset_degC(degC) BQ769X2_APPLY(ALERTTempOffset, degC)
#define SET_TS1TempOffset_degC(degC) BQ769X2_APPLY(TS1TempOffset, degC)
#define SET_TS2TempOffset_degC(degC) BQ769X2_APPLY(TS2TempOffset, degC)
#define SET_TS3TempOffset_degC(degC) BQ769X2_APPLY(TS3TempOffset, degC)
#define SET_HDQTempOffset_degC(degC) BQ769X2_APPLY(HDQTempOffset, degC)
#define SET_DCHGTempOffset_degC(degC) BQ769X2_APPLY(DCHGTempOffset, degC)
#define SET_DDSGTempOffset_degC(degC) BQ769X2_APPLY(DDSGTempOffset, degC)
#define SET_IntGain_dimensionless(dimensionless) BQ769X2_APPLY_NOCONV(IntGain, dimensionless)
#define SET_Intbaseoffset_dimensionless(dimensionless) BQ769X2_APPLY_NOCONV(Intbaseoffset, dimensionless)
#define SET_IntMaximumAD_dimensionless(dimensionless) BQ769X2_APPLY_NOCONV(IntMaximumAD, dimensionless)
#define SET_IntMaximumTemp_K(K) BQ769X2_APPLY(IntMaximumTemp, K)
#define SET_T18kCoeffa1_dimensionless(dimensionless) BQ769X2_APPLY_NOCONV(T18kCoeffa1, dimensionless)
#define SET_T18kCoeffa2_dimensionless(dimensionless) BQ769X2_APPLY_NOCONV(T18kCoeffa2, dimensionless)
#define SET_T18kCoeffa3_dimensionless(dimensionless) BQ769X2_APPLY_NOCONV(T18kCoeffa3, dimensionless)
#define SET_T18kCoeffa4_dimensionless(dimensionless) BQ769X2_APPLY_NOCONV(T18kCoeffa4, dimensionless)
#define SET_T18kCoeffa5_dimensionless(dimensionless) BQ769X2_APPLY_NOCONV(T18kCoeffa5, dimensionless)
#define SET_T18kCoeffb1_dimensionless(dimensionless) BQ769X2_APPLY_NOCONV(T18kCoeffb1, dimensionless)
#define SET_T18kCoeffb2_dimensionless(dimensionless) BQ769X2_APPLY_NOCONV(T18kCoeffb2, dimensionless)
#define SET_T18kCoeffb3_dimensionless(dimensionless) BQ769X2_APPLY_NOCONV(T18kCoeffb3, dimensionless)
#define SET_T18kCoeffb4_dimensionless(dimensionless) BQ769X2_APPLY_NOCONV(T18kCoeffb4, dimensionless)
#define SET_T18kAdc0_dimensionless(dimensionless) BQ769X2_APPLY_NOCONV(T18kAdc0, dimensionless)
#define SET_T180kCoeffa1_dimensionless(dimensionless) BQ769X2_APPLY_NOCONV(T180kCoeffa1, dimensionless)
#define SET_T180kCoeffa2_dimensionless(dimensionless) BQ769X2_APPLY_NOCONV(T180kCoeffa2, dimensionless)
#define SET_T180kCoeffa3_dimensionless(dimensionless) BQ769X2_APPLY_NOCONV(T180kCoeffa3, dimensionless)
#define SET_T180kCoeffa4_dimensionless(dimensionless) BQ769X2_APPLY_NOCONV(T180kCoeffa4, dimensionless)
#define SET_T180kCoeffa5_dimensionless(dimensionless) BQ769X2_APPLY_NOCONV(T180kCoeffa5, dimensionless)
#define SET_T180kCoeffb1_dimensionless(dimensionless) BQ769X2_APPLY_NOCONV(T180kCoeffb1, dimensionless)
#define SET_T180kCoeffb2_dimensionless(dimensionless) BQ769X2_APPLY_NOCONV(T180kCoeffb2, dimensionless)
#define SET_T180kCoeffb3_dimensionless(dimensionless) BQ769X2_APPLY_NOCONV(T180kCoeffb3, dimensionless)
#define SET_T180kCoeffb4_dimensionless(dimensionless) BQ769X2_APPLY_NOCONV(T180kCoeffb4, dimensionless)
#define SET_T180kAdc0_dimensionless(dimensionless) BQ769X2_APPLY_NOCONV(T180kAdc0, dimensionless)
#define SET_CustomCoeffa1_dimensionless(dimensionless) BQ769X2_APPLY_NOCONV(CustomCoeffa1, dimensionless)
#define SET_CustomCoeffa2_dimensionless(dimensionless) BQ769X2_APPLY_NOCONV(CustomCoeffa2, dimensionless)
#define SET_CustomCoeffa3_dimensionless(dimensionless) BQ769X2_APPLY_NOCONV(CustomCoeffa3, dimensionless)
#define SET_CustomCoeffa4_dimensionless(dimensionless) BQ769X2_APPLY_NOCONV(CustomCoeffa4, dimensionless)
#define SET_CustomCoeffa5_dimensionless(dimensionless) BQ769X2_APPLY_NOCONV(CustomCoeffa5, dimensionless)
#define SET_CustomCoeffb1_dimensionless(dimensionless) BQ769X2_APPLY_NOCONV(CustomCoeffb1, dimensionless)
#define SET_CustomCoeffb2_dimensionless(dimensionless) BQ769X2_APPLY_NOCONV(CustomCoeffb2, dimensionless)
#define SET_CustomCoeffb3_dimensionless(dimensionless) BQ769X2_APPLY_NOCONV(CustomCoeffb3, dimensionless)
#define SET_CustomCoeffb4_dimensionless(dimensionless) BQ769X2_APPLY_NOCONV(CustomCoeffb4, dimensionless)
#define SET_CustomRc0_dimensionless(dimensionless) BQ769X2_APPLY_NOCONV(CustomRc0, dimensionless)
#define SET_CustomAdc0_dimensionless(dimensionless) BQ769X2_APPLY_NOCONV(CustomAdc0, dimensionless)
#define SET_CoulombCounterDeadband_nV(nV) BQ769X2_APPLY(CoulombCounterDeadband, nV)
#define SET_CUVThresholdOverride_Raw(Raw) BQ769X2_APPLY_NOCONV(CUVThresholdOverride, Raw)
#define SET_COVThresholdOverride_Raw(Raw) BQ769X2_APPLY_NOCONV(COVThresholdOverride, Raw)
#define SET_MinBlowFuseVoltage_mV(mV) BQ769X2_APPLY(MinBlowFuseVoltage, mV)
#define SET_FuseBlowTimeout_s(s) BQ769X2_APPLY_NOCONV(FuseBlowTimeout, s)
#define SET_PowerConfig_Raw(Raw) BQ769X2_APPLY_NOCONV(PowerConfig, Raw)
#define SET_REG12Config_Raw(Raw) BQ769X2_APPLY_NOCONV(REG12Config, Raw)
#define SET_REG0Config_Raw(Raw) BQ769X2_APPLY_NOCONV(REG0Config, Raw)
#define SET_HWDRegulatorOptions_Raw(Raw) BQ769X2_APPLY_NOCONV(HWDRegulatorOptions, Raw)
#define SET_CommType_dimensionless(dimensionless) BQ769X2_APPLY_NOCONV(CommType, dimensionless)
#define SET_I2CAddress_dimensionless(dimensionless) BQ769X2_APPLY_NOCONV(I2CAddress, dimensionless)
#define SET_SPIConfiguration_dimensionless(dimensionless) BQ769X2_APPLY_NOCONV(SPIConfiguration, dimensionless)
#define SET_CommIdleTime_s(s) BQ769X2_APPLY_NOCONV(CommIdleTime, s)
#define SET_CFETOFFPinConfig_Raw(Raw) BQ769X2_APPLY_NOCONV(CFETOFFPinConfig, Raw)
#define SET_DFETOFFPinConfig_Raw(Raw) BQ769X2_APPLY_NOCONV(DFETOFFPinConfig, Raw)
#define SET_ALERTPinConfig_Raw(Raw) BQ769X2_APPLY_NOCONV(ALERTPinConfig, Raw)
#define SET_TS1Config_Raw(Raw) BQ769X2_APPLY_NOCONV(TS1Config, Raw)
#define SET_TS2Config_Raw(Raw) BQ769X2_APPLY_NOCONV(TS2Config, Raw)
#define SET_TS3Config_Raw(Raw) BQ769X2_APPLY_NOCONV(TS3Config, Raw)
#define SET_HDQPinConfig_Raw(Raw) BQ769X2_APPLY_NOCONV(HDQPinConfig, Raw)
#define SET_DCHGPinConfig_Raw(Raw) BQ769X2_APPLY_NOCONV(DCHGPinConfig, Raw)
#define SET_DDSGPinConfig_Raw(Raw) BQ769X2_APPLY_NOCONV(DDSGPinConfig, Raw)
#define SET_DAConfiguration_Raw(Raw) BQ769X2_APPLY_NOCONV(DAConfiguration, Raw)
#define SET_VCellMode_Raw(Raw) BQ769X2_APPLY_NOCONV(VCellMode, Raw)
#define SET_CC3Samples_Num(Num) BQ769X2_APPLY_NOCONV(CC3Samples, Num)
#define SET_ProtectionConfiguration_Raw(Raw) BQ769X2_APPLY_NOCONV(ProtectionConfiguration, Raw)
#define SET_EnabledProtectionsA_Raw(Raw) BQ769X2_APPLY_NOCONV(EnabledProtectionsA, Raw)
#define SET_EnabledProtectionsB_Raw(Raw) BQ769X2_APPLY_NOCONV(EnabledProtectionsB, Raw)
#define SET_EnabledProtectionsC_Raw(Raw) BQ769X2_APPLY_NOCONV(EnabledProtectionsC, Raw)
#define SET_CHGFETProtectionsA_Raw(Raw) BQ769X2_APPLY_NOCONV(CHGFETProtectionsA, Raw)
#define SET_CHGFETProtectionsB_Raw(Raw) BQ769X2_APPLY_NOCONV(CHGFETProtectionsB, Raw)
#define SET_CHGFETProtectionsC_Raw(Raw) BQ769X2_APPLY_NOCONV(CHGFETProtectionsC, Raw)
#define SET_DSGFETProtectionsA_Raw(Raw) BQ769X2_APPLY_NOCONV(DSGFETProtectionsA, Raw)
#define SET_DSGFETProtectionsB_Raw(Raw) BQ769X2_APPLY_NOCONV(DSGFETProtectionsB, Raw)
#define SET_DSGFETProtectionsC_Raw(Raw) BQ769X2_APPLY_NOCONV(DSGFETProtectionsC, Raw)
#define SET_BodyDiodeThreshold_mA(mA) BQ769X2_APPLY_NOCONV(BodyDiodeThreshold, mA)
#define SET_DefaultAlarmMask_Raw(Raw) BQ769X2_APPLY_NOCONV(DefaultAlarmMask, Raw)
#define SET_SFAlertMaskA_Raw(Raw) BQ769X2_APPLY_NOCONV(SFAlertMaskA, Raw)
#define SET_SFAlertMaskB_Raw(Raw) BQ769X2_APPLY_NOCONV(SFAlertMaskB, Raw)
#define SET_SFAlertMaskC_Raw(Raw) BQ769X2_APPLY_NOCONV(SFAlertMaskC | (1 << 1), Raw)
#define SET_PFAlertMaskA_Raw(Raw) BQ769X2_APPLY_NOCONV(PFAlertMaskA, Raw)
#define SET_PFAlertMaskB_Raw(Raw) BQ769X2_APPLY_NOCONV(PFAlertMaskB, Raw)
#define SET_PFAlertMaskC_Raw(Raw) BQ769X2_APPLY_NOCONV(PFAlertMaskC, Raw)
#define SET_PFAlertMaskD_Raw(Raw) BQ769X2_APPLY_NOCONV(PFAlertMaskD, Raw)
#define SET_EnabledPFA_Raw(Raw) BQ769X2_APPLY_NOCONV(EnabledPFA, Raw)
#define SET_EnabledPFB_Raw(Raw) BQ769X2_APPLY_NOCONV(EnabledPFB, Raw)
#define SET_EnabledPFC_Raw(Raw) BQ769X2_APPLY_NOCONV(EnabledPFC, Raw)
#define SET_EnabledPFD_Raw(Raw) BQ769X2_APPLY_NOCONV(EnabledPFD, Raw)
#define SET_FETOptions_Raw(Raw) BQ769X2_APPLY_NOCONV(FETOptions, Raw)
#define SET_ChgPumpControl_Raw(Raw) BQ769X2_APPLY_NOCONV(ChgPumpControl, Raw)
#define SET_PrechargeStartVoltage_mV(mV) BQ769X2_APPLY_NOCONV(PrechargeStartVoltage, mV)
#define SET_PrechargeStopVoltage_mV(mV) BQ769X2_APPLY_NOCONV(PrechargeStopVoltage, mV)
#define SET_PredischargeTimeout_ms(ms) BQ769X2_APPLY(PredischargeTimeout, ms)
#define SET_PredischargeStopDelta_mV(mV) BQ769X2_APPLY(PredischargeStopDelta, mV)
#define SET_DsgCurrentThreshold_userA(userA) BQ769X2_APPLY_NOCONV(DsgCurrentThreshold, userA)
#define SET_ChgCurrentThreshold_userA(userA) BQ769X2_APPLY_NOCONV(ChgCurrentThreshold, userA)
#define SET_CheckTime_s(s) BQ769X2_APPLY_NOCONV(CheckTime, s)
#define SET_Cell1Interconnect_mOhm(mOhm) BQ769X2_APPLY_NOCONV(Cell1Interconnect, mOhm)
#define SET_Cell2Interconnect_mOhm(mOhm) BQ769X2_APPLY_NOCONV(Cell2Interconnect, mOhm)
#define SET_Cell3Interconnect_mOhm(mOhm) BQ769X2_APPLY_NOCONV(Cell3Interconnect, mOhm)
#define SET_Cell4Interconnect_mOhm(mOhm) BQ769X2_APPLY_NOCONV(Cell4Interconnect, mOhm)
#define SET_Cell5Interconnect_mOhm(mOhm) BQ769X2_APPLY_NOCONV(Cell5Interconnect, mOhm)
#define SET_Cell6Interconnect_mOhm(mOhm) BQ769X2_APPLY_NOCONV(Cell6Interconnect, mOhm)
#define SET_Cell7Interconnect_mOhm(mOhm) BQ769X2_APPLY_NOCONV(Cell7Interconnect, mOhm)
#define SET_Cell8Interconnect_mOhm(mOhm) BQ769X2_APPLY_NOCONV(Cell8Interconnect, mOhm)
#define SET_Cell9Interconnect_mOhm(mOhm) BQ769X2_APPLY_NOCONV(Cell9Interconnect, mOhm)
#define SET_Cell10Interconnect_mOhm(mOhm) BQ769X2_APPLY_NOCONV(Cell10Interconnect, mOhm)
#define SET_Cell11Interconnect_mOhm(mOhm) BQ769X2_APPLY_NOCONV(Cell11Interconnect, mOhm)
#define SET_Cell12Interconnect_mOhm(mOhm) BQ769X2_APPLY_NOCONV(Cell12Interconnect, mOhm)
#define SET_Cell13Interconnect_mOhm(mOhm) BQ769X2_APPLY_NOCONV(Cell13Interconnect, mOhm)
#define SET_Cell14Interconnect_mOhm(mOhm) BQ769X2_APPLY_NOCONV(Cell14Interconnect, mOhm)
#define SET_Cell15Interconnect_mOhm(mOhm) BQ769X2_APPLY_NOCONV(Cell15Interconnect, mOhm)
#define SET_Cell16Interconnect_mOhm(mOhm) BQ769X2_APPLY_NOCONV(Cell16Interconnect, mOhm)
#define SET_MfgStatusInit_Raw(Raw) BQ769X2_APPLY_NOCONV(MfgStatusInit, Raw)
#define SET_BalancingConfiguration_Raw(Raw) BQ769X2_APPLY_NOCONV(BalancingConfiguration, Raw)
#define SET_MinCellTemp_degC(degC) BQ769X2_APPLY_NOCONV(MinCellTemp, degC)
#define SET_MaxCellTemp_degC(degC) BQ769X2_APPLY_NOCONV(MaxCellTemp, degC)
#define SET_MaxInternalTemp_degC(degC) BQ769X2_APPLY_NOCONV(MaxInternalTemp, degC)
#define SET_CellBalanceInterval_s(s) BQ769X2_APPLY_NOCONV(CellBalanceInterval, s)
#define SET_CellBalanceMaxCells_Num(Num) BQ769X2_APPLY_NOCONV(CellBalanceMaxCells, Num)
#define SET_CellBalanceMinCellVCharge_mV(mV) BQ769X2_APPLY_NOCONV(CellBalanceMinCellVCharge, mV)
#define SET_CellBalanceMinDeltaCharge_mV(mV) BQ769X2_APPLY_NOCONV(CellBalanceMinDeltaCharge, mV)
#define SET_CellBalanceStopDeltaCharge_mV(mV) BQ769X2_APPLY_NOCONV(CellBalanceStopDeltaCharge, mV)
#define SET_CellBalanceMinCellVRelax_mV(mV) BQ769X2_APPLY_NOCONV(CellBalanceMinCellVRelax, mV)
#define SET_CellBalanceMinDeltaRelax_mV(mV) BQ769X2_APPLY_NOCONV(CellBalanceMinDeltaRelax, mV)
#define SET_CellBalanceStopDeltaRelax_mV(mV) BQ769X2_APPLY_NOCONV(CellBalanceStopDeltaRelax, mV)
#define SET_ShutdownCellVoltage_mV(mV) BQ769X2_APPLY_NOCONV(ShutdownCellVoltage, mV)
#define SET_ShutdownStackVoltage_mV(mV) BQ769X2_APPLY(ShutdownStackVoltage, mV)
#define SET_LowVShutdownDelay_s(s) BQ769X2_APPLY_NOCONV(LowVShutdownDelay, s)
#define SET_ShutdownTemperature_degC(degC) BQ769X2_APPLY_NOCONV(ShutdownTemperature, degC)
#define SET_ShutdownTemperatureDelay_s(s) BQ769X2_APPLY_NOCONV(ShutdownTemperatureDelay, s)
#define SET_FETOffDelay_s(s) BQ769X2_APPLY(FETOffDelay, s)
#define SET_ShutdownCommandDelay_s(s) BQ769X2_APPLY(ShutdownCommandDelay, s)
#define SET_AutoShutdownTime_min(min) BQ769X2_APPLY_NOCONV(AutoShutdownTime, min)
#define SET_RAMFailShutdownTime_s(s) BQ769X2_APPLY_NOCONV(RAMFailShutdownTime, s)
#define SET_SleepCurrent_mA(mA) BQ769X2_APPLY_NOCONV(SleepCurrent, mA)
#define SET_VoltageTime_s(s) BQ769X2_APPLY_NOCONV(VoltageTime, s)
#define SET_WakeComparatorCurrent_mA(mA) BQ769X2_APPLY_NOCONV(WakeComparatorCurrent, mA)
#define SET_SleepHysteresisTime_s(s) BQ769X2_APPLY_NOCONV(SleepHysteresisTime, s)
#define SET_SleepChargerVoltageThreshold_mV(mV) BQ769X2_APPLY(SleepChargerVoltageThreshold, mV)
#define SET_SleepChargerPACKTOSDelta_mV(mV) BQ769X2_APPLY(SleepChargerPACKTOSDelta, mV)
#define SET_ConfigRAMSignature_Raw(Raw) BQ769X2_APPLY_NOCONV(ConfigRAMSignature, Raw)
#define SET_CUVThreshold_mV(mV) BQ769X2_APPLY(CUVThreshold, mV)
#define SET_CUVDelay_ms(ms) BQ769X2_APPLY(CUVDelay, ms)
#define SET_CUVRecoveryHysteresis_mV(mV) BQ769X2_APPLY(CUVRecoveryHysteresis, mV)
#define SET_COVThreshold_mV(mV) BQ769X2_APPLY(COVThreshold, mV)
#define SET_COVDelay_ms(ms) BQ769X2_APPLY(COVDelay, ms)
#define SET_COVRecoveryHysteresis_mV(mV) BQ769X2_APPLY(COVRecoveryHysteresis, mV)
#define SET_COVLLatchLimit_dimensionless(dimensionless) BQ769X2_APPLY_NOCONV(COVLLatchLimit, dimensionless)
#define SET_COVLCounterDecDelay_s(s) BQ769X2_APPLY_NOCONV(COVLCounterDecDelay, s)
#define SET_COVLRecoveryTime_s(s) BQ769X2_APPLY_NOCONV(COVLRecoveryTime, s)
#define SET_OCCThreshold_mV(mV) BQ769X2_APPLY(OCCThreshold, mV)
#define SET_OCCDelay_ms(ms) BQ769X2_APPLY(OCCDelay, ms)
#define SET_OCCRecoveryThreshold_mA(mA) BQ769X2_APPLY_NOCONV(OCCRecoveryThreshold, mA)
#define SET_OCCPACKTOSDelta_mV(mV) BQ769X2_APPLY(OCCPACKTOSDelta, mV)
#define SET_OCD1Threshold_mV(mV) BQ769X2_APPLY(OCD1Threshold, mV)
#define SET_OCD1Delay_ms(ms) BQ769X2_APPLY(OCD1Delay, ms)
#define SET_OCD2Threshold_mV(mV) BQ769X2_APPLY(OCD2Threshold, mV)
#define SET_OCD2Delay_ms(ms) BQ769X2_APPLY(OCD2Delay, ms)
#define SET_SCDThreshold_dimensionless(dimensionless) BQ769X2_APPLY_NOCONV(SCDThreshold, dimensionless)
#define SET_SCDDelay_us(us) BQ769X2_APPLY(SCDDelay, us)
#define SET_SCDRecoveryTime_s(s) BQ769X2_APPLY_NOCONV(SCDRecoveryTime, s)
#define SET_OCD3Threshold_userA(userA) BQ769X2_APPLY_NOCONV(OCD3Threshold, userA)
#define SET_OCD3Delay_s(s) BQ769X2_APPLY_NOCONV(OCD3Delay, s)
#define SET_OCDRecoveryThreshold_mA(mA) BQ769X2_APPLY_NOCONV(OCDRecoveryThreshold, mA)
#define SET_OCDLLatchLimit_dimensionless(dimensionless) BQ769X2_APPLY_NOCONV(OCDLLatchLimit, dimensionless)
#define SET_OCDLCounterDecDelay_s(s) BQ769X2_APPLY_NOCONV(OCDLCounterDecDelay, s)
#define SET_OCDLRecoveryTime_s(s) BQ769X2_APPLY_NOCONV(OCDLRecoveryTime, s)
#define SET_OCDLRecoveryThreshold_mA(mA) BQ769X2_APPLY_NOCONV(OCDLRecoveryThreshold, mA)
#define SET_SCDLLatchLimit_dimensionless(dimensionless) BQ769X2_APPLY_NOCONV(SCDLLatchLimit, dimensionless)
#define SET_SCDLCounterDecDelay_s(s) BQ769X2_APPLY_NOCONV(SCDLCounterDecDelay, s)
#define SET_SCDLRecoveryTime_s(s) BQ769X2_APPLY_NOCONV(SCDLRecoveryTime, s)
#define SET_SCDLRecoveryThreshold_mA(mA) BQ769X2_APPLY_NOCONV(SCDLRecoveryThreshold, mA)
#define SET_OTCThreshold_degC(degC) BQ769X2_APPLY_NOCONV(OTCThreshold, degC)
#define SET_OTCDelay_s(s) BQ769X2_APPLY_NOCONV(OTCDelay, s)
#define SET_OTCRecovery_degC(degC) BQ769X2_APPLY_NOCONV(OTCRecovery, degC)
#define SET_OTDThreshold_degC(degC) BQ769X2_APPLY_NOCONV(OTDThreshold, degC)
#define SET_OTDDelay_s(s) BQ769X2_APPLY_NOCONV(OTDDelay, s)
#define SET_OTDRecovery_degC(degC) BQ769X2_APPLY_NOCONV(OTDRecovery, degC)
#define SET_OTFThreshold_degC(degC) BQ769X2_APPLY_NOCONV(OTFThreshold, degC)
#define SET_OTFDelay_s(s) BQ769X2_APPLY_NOCONV(OTFDelay, s)
#define SET_OTFRecovery_degC(degC) BQ769X2_APPLY_NOCONV(OTFRecovery, degC)
#define SET_OTINTThreshold_degC(degC) BQ769X2_APPLY_NOCONV(OTINTThreshold, degC)
#define SET_OTINTDelay_s(s) BQ769X2_APPLY_NOCONV(OTINTDelay, s)
#define SET_OTINTRecovery_degC(degC) BQ769X2_APPLY_NOCONV(OTINTRecovery, degC)
#define SET_UTCThreshold_degC(degC) BQ769X2_APPLY_NOCONV(UTCThreshold, degC)
#define SET_UTCDelay_s(s) BQ769X2_APPLY_NOCONV(UTCDelay, s)
#define SET_UTCRecovery_degC(degC) BQ769X2_APPLY_NOCONV(UTCRecovery, degC)
#define SET_UTDThreshold_degC(degC) BQ769X2_APPLY_NOCONV(UTDThreshold, degC)
#define SET_UTDDelay_s(s) BQ769X2_APPLY_NOCONV(UTDDelay, s)
#define SET_UTDRecovery_degC(degC) BQ769X2_APPLY_NOCONV(UTDRecovery, degC)
#define SET_UTINTThreshold_degC(degC) BQ769X2_APPLY_NOCONV(UTINTThreshold, degC)
#define SET_UTINTDelay_s(s) BQ769X2_APPLY_NOCONV(UTINTDelay, s)
#define SET_UTINTRecovery_degC(degC) BQ769X2_APPLY_NOCONV(UTINTRecovery, degC)
#define SET_ProtectionsRecoveryTime_s(s) BQ769X2_APPLY_NOCONV(ProtectionsRecoveryTime, s)
#define SET_HWDDelay_s(s) BQ769X2_APPLY_NOCONV(HWDDelay, s)
#define SET_LoadDetectActiveTime_s(s) BQ769X2_APPLY_NOCONV(LoadDetectActiveTime, s)
#define SET_LoadDetectRetryDelay_s(s) BQ769X2_APPLY_NOCONV(LoadDetectRetryDelay, s)
#define SET_LoadDetectTimeout_hrs(hrs) BQ769X2_APPLY_NOCONV(LoadDetectTimeout, hrs)
#define SET_PTOChargeThreshold_mA(mA) BQ769X2_APPLY_NOCONV(PTOChargeThreshold, mA)
#define SET_PTODelay_s(s) BQ769X2_APPLY_NOCONV(PTODelay, s)
#define SET_PTOReset_userAh(userAh) BQ769X2_APPLY_NOCONV(PTOReset, userAh)
#define SET_CUDEPThreshold_mV(mV) BQ769X2_APPLY_NOCONV(CUDEPThreshold, mV)
#define SET_CUDEPDelay_s(s) BQ769X2_APPLY_NOCONV(CUDEPDelay, s)
#define SET_SUVThreshold_mV(mV) BQ769X2_APPLY_NOCONV(SUVThreshold, mV)
#define SET_SUVDelay_s(s) BQ769X2_APPLY_NOCONV(SUVDelay, s)
#define SET_SOVThreshold_mV(mV) BQ769X2_APPLY_NOCONV(SOVThreshold, mV)
#define SET_SOVDelay_s(s) BQ769X2_APPLY_NOCONV(SOVDelay, s)
#define SET_TOSSThreshold_mV(mV) BQ769X2_APPLY_NOCONV(TOSSThreshold, mV)
#define SET_TOSSDelay_s(s) BQ769X2_APPLY_NOCONV(TOSSDelay, s)
#define SET_SOCCThreshold_userA(userA) BQ769X2_APPLY_NOCONV(SOCCThreshold, userA)
#define SET_SOCCDelay_s(s) BQ769X2_APPLY_NOCONV(SOCCDelay, s)
#define SET_SOCDThreshold_userA(userA) BQ769X2_APPLY_NOCONV(SOCDThreshold, userA)
#define SET_SOCDDelay_s(s) BQ769X2_APPLY_NOCONV(SOCDDelay, s)
#define SET_SOTThreshold_degC(degC) BQ769X2_APPLY_NOCONV(SOTThreshold, degC)
#define SET_SOTDelay_s(s) BQ769X2_APPLY_NOCONV(SOTDelay, s)
#define SET_SOTFThreshold_degC(degC) BQ769X2_APPLY_NOCONV(SOTFThreshold, degC)
#define SET_SOTFDelay_s(s) BQ769X2_APPLY_NOCONV(SOTFDelay, s)
#define SET_VIMRCheckVoltage_mV(mV) BQ769X2_APPLY_NOCONV(VIMRCheckVoltage, mV)
#define SET_VIMRMaxRelaxCurrent_mA(mA) BQ769X2_APPLY_NOCONV(VIMRMaxRelaxCurrent, mA)
#define SET_VIMRThreshold_mV(mV) BQ769X2_APPLY_NOCONV(VIMRThreshold, mV)
#define SET_VIMRDelay_s(s) BQ769X2_APPLY_NOCONV(VIMRDelay, s)
#define SET_VIMRRelaxMinDuration_s(s) BQ769X2_APPLY_NOCONV(VIMRRelaxMinDuration, s)
#define SET_VIMACheckVoltage_mV(mV) BQ769X2_APPLY_NOCONV(VIMACheckVoltage, mV)
#define SET_VIMAMinActiveCurrent_mA(mA) BQ769X2_APPLY_NOCONV(VIMAMinActiveCurrent, mA)
#define SET_VIMAThreshold_mV(mV) BQ769X2_APPLY_NOCONV(VIMAThreshold, mV)
#define SET_VIMADelay_s(s) BQ769X2_APPLY_NOCONV(VIMADelay, s)
#define SET_CFETFOFFThreshold_mA(mA) BQ769X2_APPLY_NOCONV(CFETFOFFThreshold, mA)
#define SET_CFETFOFFDelay_s(s) BQ769X2_APPLY_NOCONV(CFETFOFFDelay, s)
#define SET_DFETFOFFThreshold_mA(mA) BQ769X2_APPLY_NOCONV(DFETFOFFThreshold, mA)
#define SET_DFETFOFFDelay_s(s) BQ769X2_APPLY_NOCONV(DFETFOFFDelay, s)
#define SET_VSSFFailThreshold_dimensionless(dimensionless) BQ769X2_APPLY_NOCONV(VSSFFailThreshold, dimensionless)
#define SET_VSSFDelay_s(s) BQ769X2_APPLY_NOCONV(VSSFDelay, s)
#define SET_PF2LVLDelay_s(s) BQ769X2_APPLY_NOCONV(PF2LVLDelay, s)
#define SET_LFOFDelay_s(s) BQ769X2_APPLY_NOCONV(LFOFDelay, s)
#define SET_HWMXDelay_s(s) BQ769X2_APPLY_NOCONV(HWMXDelay, s)
#define SET_SecuritySettings_Raw(Raw) BQ769X2_APPLY_NOCONV(SecuritySettings, Raw)
#define SET_UnsealKeyStep1_Raw(Raw) BQ769X2_APPLY_NOCONV(UnsealKeyStep1, Raw)
#define SET_UnsealKeyStep2_Raw(Raw) BQ769X2_APPLY_NOCONV(UnsealKeyStep2, Raw)
#define SET_FullAccessKeyStep1_Raw(Raw) BQ769X2_APPLY_NOCONV(FullAccessKeyStep1, Raw)
#define SET_FullAccessKeyStep2_Raw(Raw) BQ769X2_APPLY_NOCONV(FullAccessKeyStep2, Raw)

// RAW
#define SET_Cell1Gain_DEFAULT() BQ769X2_APPLY_NOCONV(Cell1Gain, Cell1Gain_DEFAULTRAW)
#define SET_Cell2Gain_DEFAULT() BQ769X2_APPLY_NOCONV(Cell2Gain, Cell2Gain_DEFAULTRAW)
#define SET_Cell3Gain_DEFAULT() BQ769X2_APPLY_NOCONV(Cell3Gain, Cell3Gain_DEFAULTRAW)
#define SET_Cell4Gain_DEFAULT() BQ769X2_APPLY_NOCONV(Cell4Gain, Cell4Gain_DEFAULTRAW)
#define SET_Cell5Gain_DEFAULT() BQ769X2_APPLY_NOCONV(Cell5Gain, Cell5Gain_DEFAULTRAW)
#define SET_Cell6Gain_DEFAULT() BQ769X2_APPLY_NOCONV(Cell6Gain, Cell6Gain_DEFAULTRAW)
#define SET_Cell7Gain_DEFAULT() BQ769X2_APPLY_NOCONV(Cell7Gain, Cell7Gain_DEFAULTRAW)
#define SET_Cell8Gain_DEFAULT() BQ769X2_APPLY_NOCONV(Cell8Gain, Cell8Gain_DEFAULTRAW)
#define SET_Cell9Gain_DEFAULT() BQ769X2_APPLY_NOCONV(Cell9Gain, Cell9Gain_DEFAULTRAW)
#define SET_Cell10Gain_DEFAULT() BQ769X2_APPLY_NOCONV(Cell10Gain, Cell10Gain_DEFAULTRAW)
#define SET_Cell11Gain_DEFAULT() BQ769X2_APPLY_NOCONV(Cell11Gain, Cell11Gain_DEFAULTRAW)
#define SET_Cell12Gain_DEFAULT() BQ769X2_APPLY_NOCONV(Cell12Gain, Cell12Gain_DEFAULTRAW)
#define SET_Cell13Gain_DEFAULT() BQ769X2_APPLY_NOCONV(Cell13Gain, Cell13Gain_DEFAULTRAW)
#define SET_Cell14Gain_DEFAULT() BQ769X2_APPLY_NOCONV(Cell14Gain, Cell14Gain_DEFAULTRAW)
#define SET_Cell15Gain_DEFAULT() BQ769X2_APPLY_NOCONV(Cell15Gain, Cell15Gain_DEFAULTRAW)
#define SET_Cell16Gain_DEFAULT() BQ769X2_APPLY_NOCONV(Cell16Gain, Cell16Gain_DEFAULTRAW)
#define SET_PackGain_DEFAULT() BQ769X2_APPLY_NOCONV(PackGain, PackGain_DEFAULTRAW)
#define SET_TOSGain_DEFAULT() BQ769X2_APPLY_NOCONV(TOSGain, TOSGain_DEFAULTRAW)
#define SET_LDGain_DEFAULT() BQ769X2_APPLY_NOCONV(LDGain, LDGain_DEFAULTRAW)
#define SET_ADCGain_DEFAULT() BQ769X2_APPLY_NOCONV(ADCGain, ADCGain_DEFAULTRAW)
#define SET_CCGain_DEFAULT() BQ769X2_APPLY_FLOAT(CCGain, CCGain_DEFAULTRAW)
#define SET_CapacityGain_DEFAULT() BQ769X2_APPLY_FLOAT(CapacityGain, CapacityGain_DEFAULTRAW)
#define SET_VcellOffset_DEFAULT() BQ769X2_APPLY_NOCONV(VcellOffset, VcellOffset_DEFAULTRAW)
#define SET_VdivOffset_DEFAULT() BQ769X2_APPLY_NOCONV(VdivOffset, VdivOffset_DEFAULTRAW)
#define SET_CoulombCounterOffsetSamples_DEFAULT() BQ769X2_APPLY_NOCONV(CoulombCounterOffsetSamples, CoulombCounterOffsetSamples_DEFAULTRAW)
#define SET_BoardOffset_DEFAULT() BQ769X2_APPLY_NOCONV(BoardOffset, BoardOffset_DEFAULTRAW)
#define SET_InternalTempOffset_DEFAULT() BQ769X2_APPLY_NOCONV(InternalTempOffset, InternalTempOffset_DEFAULTRAW)
#define SET_CFETOFFTempOffset_DEFAULT() BQ769X2_APPLY_NOCONV(CFETOFFTempOffset, CFETOFFTempOffset_DEFAULTRAW)
#define SET_DFETOFFTempOffset_DEFAULT() BQ769X2_APPLY_NOCONV(DFETOFFTempOffset, DFETOFFTempOffset_DEFAULTRAW)
#define SET_ALERTTempOffset_DEFAULT() BQ769X2_APPLY_NOCONV(ALERTTempOffset, ALERTTempOffset_DEFAULTRAW)
#define SET_TS1TempOffset_DEFAULT() BQ769X2_APPLY_NOCONV(TS1TempOffset, TS1TempOffset_DEFAULTRAW)
#define SET_TS2TempOffset_DEFAULT() BQ769X2_APPLY_NOCONV(TS2TempOffset, TS2TempOffset_DEFAULTRAW)
#define SET_TS3TempOffset_DEFAULT() BQ769X2_APPLY_NOCONV(TS3TempOffset, TS3TempOffset_DEFAULTRAW)
#define SET_HDQTempOffset_DEFAULT() BQ769X2_APPLY_NOCONV(HDQTempOffset, HDQTempOffset_DEFAULTRAW)
#define SET_DCHGTempOffset_DEFAULT() BQ769X2_APPLY_NOCONV(DCHGTempOffset, DCHGTempOffset_DEFAULTRAW)
#define SET_DDSGTempOffset_DEFAULT() BQ769X2_APPLY_NOCONV(DDSGTempOffset, DDSGTempOffset_DEFAULTRAW)
#define SET_IntGain_DEFAULT() BQ769X2_APPLY_NOCONV(IntGain, IntGain_DEFAULTRAW)
#define SET_Intbaseoffset_DEFAULT() BQ769X2_APPLY_NOCONV(Intbaseoffset, Intbaseoffset_DEFAULTRAW)
#define SET_IntMaximumAD_DEFAULT() BQ769X2_APPLY_NOCONV(IntMaximumAD, IntMaximumAD_DEFAULTRAW)
#define SET_IntMaximumTemp_DEFAULT() BQ769X2_APPLY_NOCONV(IntMaximumTemp, IntMaximumTemp_DEFAULTRAW)
#define SET_T18kCoeffa1_DEFAULT() BQ769X2_APPLY_NOCONV(T18kCoeffa1, T18kCoeffa1_DEFAULTRAW)
#define SET_T18kCoeffa2_DEFAULT() BQ769X2_APPLY_NOCONV(T18kCoeffa2, T18kCoeffa2_DEFAULTRAW)
#define SET_T18kCoeffa3_DEFAULT() BQ769X2_APPLY_NOCONV(T18kCoeffa3, T18kCoeffa3_DEFAULTRAW)
#define SET_T18kCoeffa4_DEFAULT() BQ769X2_APPLY_NOCONV(T18kCoeffa4, T18kCoeffa4_DEFAULTRAW)
#define SET_T18kCoeffa5_DEFAULT() BQ769X2_APPLY_NOCONV(T18kCoeffa5, T18kCoeffa5_DEFAULTRAW)
#define SET_T18kCoeffb1_DEFAULT() BQ769X2_APPLY_NOCONV(T18kCoeffb1, T18kCoeffb1_DEFAULTRAW)
#define SET_T18kCoeffb2_DEFAULT() BQ769X2_APPLY_NOCONV(T18kCoeffb2, T18kCoeffb2_DEFAULTRAW)
#define SET_T18kCoeffb3_DEFAULT() BQ769X2_APPLY_NOCONV(T18kCoeffb3, T18kCoeffb3_DEFAULTRAW)
#define SET_T18kCoeffb4_DEFAULT() BQ769X2_APPLY_NOCONV(T18kCoeffb4, T18kCoeffb4_DEFAULTRAW)
#define SET_T18kAdc0_DEFAULT() BQ769X2_APPLY_NOCONV(T18kAdc0, T18kAdc0_DEFAULTRAW)
#define SET_T180kCoeffa1_DEFAULT() BQ769X2_APPLY_NOCONV(T180kCoeffa1, T180kCoeffa1_DEFAULTRAW)
#define SET_T180kCoeffa2_DEFAULT() BQ769X2_APPLY_NOCONV(T180kCoeffa2, T180kCoeffa2_DEFAULTRAW)
#define SET_T180kCoeffa3_DEFAULT() BQ769X2_APPLY_NOCONV(T180kCoeffa3, T180kCoeffa3_DEFAULTRAW)
#define SET_T180kCoeffa4_DEFAULT() BQ769X2_APPLY_NOCONV(T180kCoeffa4, T180kCoeffa4_DEFAULTRAW)
#define SET_T180kCoeffa5_DEFAULT() BQ769X2_APPLY_NOCONV(T180kCoeffa5, T180kCoeffa5_DEFAULTRAW)
#define SET_T180kCoeffb1_DEFAULT() BQ769X2_APPLY_NOCONV(T180kCoeffb1, T180kCoeffb1_DEFAULTRAW)
#define SET_T180kCoeffb2_DEFAULT() BQ769X2_APPLY_NOCONV(T180kCoeffb2, T180kCoeffb2_DEFAULTRAW)
#define SET_T180kCoeffb3_DEFAULT() BQ769X2_APPLY_NOCONV(T180kCoeffb3, T180kCoeffb3_DEFAULTRAW)
#define SET_T180kCoeffb4_DEFAULT() BQ769X2_APPLY_NOCONV(T180kCoeffb4, T180kCoeffb4_DEFAULTRAW)
#define SET_T180kAdc0_DEFAULT() BQ769X2_APPLY_NOCONV(T180kAdc0, T180kAdc0_DEFAULTRAW)
#define SET_CustomCoeffa1_DEFAULT() BQ769X2_APPLY_NOCONV(CustomCoeffa1, CustomCoeffa1_DEFAULTRAW)
#define SET_CustomCoeffa2_DEFAULT() BQ769X2_APPLY_NOCONV(CustomCoeffa2, CustomCoeffa2_DEFAULTRAW)
#define SET_CustomCoeffa3_DEFAULT() BQ769X2_APPLY_NOCONV(CustomCoeffa3, CustomCoeffa3_DEFAULTRAW)
#define SET_CustomCoeffa4_DEFAULT() BQ769X2_APPLY_NOCONV(CustomCoeffa4, CustomCoeffa4_DEFAULTRAW)
#define SET_CustomCoeffa5_DEFAULT() BQ769X2_APPLY_NOCONV(CustomCoeffa5, CustomCoeffa5_DEFAULTRAW)
#define SET_CustomCoeffb1_DEFAULT() BQ769X2_APPLY_NOCONV(CustomCoeffb1, CustomCoeffb1_DEFAULTRAW)
#define SET_CustomCoeffb2_DEFAULT() BQ769X2_APPLY_NOCONV(CustomCoeffb2, CustomCoeffb2_DEFAULTRAW)
#define SET_CustomCoeffb3_DEFAULT() BQ769X2_APPLY_NOCONV(CustomCoeffb3, CustomCoeffb3_DEFAULTRAW)
#define SET_CustomCoeffb4_DEFAULT() BQ769X2_APPLY_NOCONV(CustomCoeffb4, CustomCoeffb4_DEFAULTRAW)
#define SET_CustomRc0_DEFAULT() BQ769X2_APPLY_NOCONV(CustomRc0, CustomRc0_DEFAULTRAW)
#define SET_CustomAdc0_DEFAULT() BQ769X2_APPLY_NOCONV(CustomAdc0, CustomAdc0_DEFAULTRAW)
#define SET_CoulombCounterDeadband_DEFAULT() BQ769X2_APPLY_NOCONV(CoulombCounterDeadband, CoulombCounterDeadband_DEFAULTRAW)
#define SET_CUVThresholdOverride_DEFAULT() BQ769X2_APPLY_NOCONV(CUVThresholdOverride, CUVThresholdOverride_DEFAULTRAW)
#define SET_COVThresholdOverride_DEFAULT() BQ769X2_APPLY_NOCONV(COVThresholdOverride, COVThresholdOverride_DEFAULTRAW)
#define SET_MinBlowFuseVoltage_DEFAULT() BQ769X2_APPLY_NOCONV(MinBlowFuseVoltage, MinBlowFuseVoltage_DEFAULTRAW)
#define SET_FuseBlowTimeout_DEFAULT() BQ769X2_APPLY_NOCONV(FuseBlowTimeout, FuseBlowTimeout_DEFAULTRAW)
#define SET_PowerConfig_DEFAULT() BQ769X2_APPLY_NOCONV(PowerConfig, PowerConfig_DEFAULTRAW)
#define SET_REG12Config_DEFAULT() BQ769X2_APPLY_NOCONV(REG12Config, REG12Config_DEFAULTRAW)
#define SET_REG0Config_DEFAULT() BQ769X2_APPLY_NOCONV(REG0Config, REG0Config_DEFAULTRAW)
#define SET_HWDRegulatorOptions_DEFAULT() BQ769X2_APPLY_NOCONV(HWDRegulatorOptions, HWDRegulatorOptions_DEFAULTRAW)
#define SET_CommType_DEFAULT() BQ769X2_APPLY_NOCONV(CommType, CommType_DEFAULTRAW)
#define SET_I2CAddress_DEFAULT() BQ769X2_APPLY_NOCONV(I2CAddress, I2CAddress_DEFAULTRAW)
#define SET_SPIConfiguration_DEFAULT() BQ769X2_APPLY_NOCONV(SPIConfiguration, SPIConfiguration_DEFAULTRAW)
#define SET_CommIdleTime_DEFAULT() BQ769X2_APPLY_NOCONV(CommIdleTime, CommIdleTime_DEFAULTRAW)
#define SET_CFETOFFPinConfig_DEFAULT() BQ769X2_APPLY_NOCONV(CFETOFFPinConfig, CFETOFFPinConfig_DEFAULTRAW)
#define SET_DFETOFFPinConfig_DEFAULT() BQ769X2_APPLY_NOCONV(DFETOFFPinConfig, DFETOFFPinConfig_DEFAULTRAW)
#define SET_ALERTPinConfig_DEFAULT() BQ769X2_APPLY_NOCONV(ALERTPinConfig, ALERTPinConfig_DEFAULTRAW)
#define SET_TS1Config_DEFAULT() BQ769X2_APPLY_NOCONV(TS1Config, TS1Config_DEFAULTRAW)
#define SET_TS2Config_DEFAULT() BQ769X2_APPLY_NOCONV(TS2Config, TS2Config_DEFAULTRAW)
#define SET_TS3Config_DEFAULT() BQ769X2_APPLY_NOCONV(TS3Config, TS3Config_DEFAULTRAW)
#define SET_HDQPinConfig_DEFAULT() BQ769X2_APPLY_NOCONV(HDQPinConfig, HDQPinConfig_DEFAULTRAW)
#define SET_DCHGPinConfig_DEFAULT() BQ769X2_APPLY_NOCONV(DCHGPinConfig, DCHGPinConfig_DEFAULTRAW)
#define SET_DDSGPinConfig_DEFAULT() BQ769X2_APPLY_NOCONV(DDSGPinConfig, DDSGPinConfig_DEFAULTRAW)
#define SET_DAConfiguration_DEFAULT() BQ769X2_APPLY_NOCONV(DAConfiguration, DAConfiguration_DEFAULTRAW)
#define SET_VCellMode_DEFAULT() BQ769X2_APPLY_NOCONV(VCellMode, VCellMode_DEFAULTRAW)
#define SET_CC3Samples_DEFAULT() BQ769X2_APPLY_NOCONV(CC3Samples, CC3Samples_DEFAULTRAW)
#define SET_ProtectionConfiguration_DEFAULT() BQ769X2_APPLY_NOCONV(ProtectionConfiguration, ProtectionConfiguration_DEFAULTRAW)
#define SET_EnabledProtectionsA_DEFAULT() BQ769X2_APPLY_NOCONV(EnabledProtectionsA, EnabledProtectionsA_DEFAULTRAW)
#define SET_EnabledProtectionsB_DEFAULT() BQ769X2_APPLY_NOCONV(EnabledProtectionsB, EnabledProtectionsB_DEFAULTRAW)
#define SET_EnabledProtectionsC_DEFAULT() BQ769X2_APPLY_NOCONV(EnabledProtectionsC, EnabledProtectionsC_DEFAULTRAW)
#define SET_CHGFETProtectionsA_DEFAULT() BQ769X2_APPLY_NOCONV(CHGFETProtectionsA, CHGFETProtectionsA_DEFAULTRAW)
#define SET_CHGFETProtectionsB_DEFAULT() BQ769X2_APPLY_NOCONV(CHGFETProtectionsB, CHGFETProtectionsB_DEFAULTRAW)
#define SET_CHGFETProtectionsC_DEFAULT() BQ769X2_APPLY_NOCONV(CHGFETProtectionsC, CHGFETProtectionsC_DEFAULTRAW)
#define SET_DSGFETProtectionsA_DEFAULT() BQ769X2_APPLY_NOCONV(DSGFETProtectionsA, DSGFETProtectionsA_DEFAULTRAW)
#define SET_DSGFETProtectionsB_DEFAULT() BQ769X2_APPLY_NOCONV(DSGFETProtectionsB, DSGFETProtectionsB_DEFAULTRAW)
#define SET_DSGFETProtectionsC_DEFAULT() BQ769X2_APPLY_NOCONV(DSGFETProtectionsC, DSGFETProtectionsC_DEFAULTRAW)
#define SET_BodyDiodeThreshold_DEFAULT() BQ769X2_APPLY_NOCONV(BodyDiodeThreshold, BodyDiodeThreshold_DEFAULTRAW)
#define SET_DefaultAlarmMask_DEFAULT() BQ769X2_APPLY_NOCONV(DefaultAlarmMask, DefaultAlarmMask_DEFAULTRAW)
#define SET_SFAlertMaskA_DEFAULT() BQ769X2_APPLY_NOCONV(SFAlertMaskA, SFAlertMaskA_DEFAULTRAW)
#define SET_SFAlertMaskB_DEFAULT() BQ769X2_APPLY_NOCONV(SFAlertMaskB, SFAlertMaskB_DEFAULTRAW)
#define SET_SFAlertMaskC_DEFAULT() BQ769X2_APPLY_NOCONV(SFAlertMaskC, SFAlertMaskC_DEFAULTRAW)
#define SET_PFAlertMaskA_DEFAULT() BQ769X2_APPLY_NOCONV(PFAlertMaskA, PFAlertMaskA_DEFAULTRAW)
#define SET_PFAlertMaskB_DEFAULT() BQ769X2_APPLY_NOCONV(PFAlertMaskB, PFAlertMaskB_DEFAULTRAW)
#define SET_PFAlertMaskC_DEFAULT() BQ769X2_APPLY_NOCONV(PFAlertMaskC, PFAlertMaskC_DEFAULTRAW)
#define SET_PFAlertMaskD_DEFAULT() BQ769X2_APPLY_NOCONV(PFAlertMaskD, PFAlertMaskD_DEFAULTRAW)
#define SET_EnabledPFA_DEFAULT() BQ769X2_APPLY_NOCONV(EnabledPFA, EnabledPFA_DEFAULTRAW)
#define SET_EnabledPFB_DEFAULT() BQ769X2_APPLY_NOCONV(EnabledPFB, EnabledPFB_DEFAULTRAW)
#define SET_EnabledPFC_DEFAULT() BQ769X2_APPLY_NOCONV(EnabledPFC, EnabledPFC_DEFAULTRAW)
#define SET_EnabledPFD_DEFAULT() BQ769X2_APPLY_NOCONV(EnabledPFD, EnabledPFD_DEFAULTRAW)
#define SET_FETOptions_DEFAULT() BQ769X2_APPLY_NOCONV(FETOptions, FETOptions_DEFAULTRAW)
#define SET_ChgPumpControl_DEFAULT() BQ769X2_APPLY_NOCONV(ChgPumpControl, ChgPumpControl_DEFAULTRAW)
#define SET_PrechargeStartVoltage_DEFAULT() BQ769X2_APPLY_NOCONV(PrechargeStartVoltage, PrechargeStartVoltage_DEFAULTRAW)
#define SET_PrechargeStopVoltage_DEFAULT() BQ769X2_APPLY_NOCONV(PrechargeStopVoltage, PrechargeStopVoltage_DEFAULTRAW)
#define SET_PredischargeTimeout_DEFAULT() BQ769X2_APPLY_NOCONV(PredischargeTimeout, PredischargeTimeout_DEFAULTRAW)
#define SET_PredischargeStopDelta_DEFAULT() BQ769X2_APPLY_NOCONV(PredischargeStopDelta, PredischargeStopDelta_DEFAULTRAW)
#define SET_DsgCurrentThreshold_DEFAULT() BQ769X2_APPLY_NOCONV(DsgCurrentThreshold, DsgCurrentThreshold_DEFAULTRAW)
#define SET_ChgCurrentThreshold_DEFAULT() BQ769X2_APPLY_NOCONV(ChgCurrentThreshold, ChgCurrentThreshold_DEFAULTRAW)
#define SET_CheckTime_DEFAULT() BQ769X2_APPLY_NOCONV(CheckTime, CheckTime_DEFAULTRAW)
#define SET_Cell1Interconnect_DEFAULT() BQ769X2_APPLY_NOCONV(Cell1Interconnect, Cell1Interconnect_DEFAULTRAW)
#define SET_Cell2Interconnect_DEFAULT() BQ769X2_APPLY_NOCONV(Cell2Interconnect, Cell2Interconnect_DEFAULTRAW)
#define SET_Cell3Interconnect_DEFAULT() BQ769X2_APPLY_NOCONV(Cell3Interconnect, Cell3Interconnect_DEFAULTRAW)
#define SET_Cell4Interconnect_DEFAULT() BQ769X2_APPLY_NOCONV(Cell4Interconnect, Cell4Interconnect_DEFAULTRAW)
#define SET_Cell5Interconnect_DEFAULT() BQ769X2_APPLY_NOCONV(Cell5Interconnect, Cell5Interconnect_DEFAULTRAW)
#define SET_Cell6Interconnect_DEFAULT() BQ769X2_APPLY_NOCONV(Cell6Interconnect, Cell6Interconnect_DEFAULTRAW)
#define SET_Cell7Interconnect_DEFAULT() BQ769X2_APPLY_NOCONV(Cell7Interconnect, Cell7Interconnect_DEFAULTRAW)
#define SET_Cell8Interconnect_DEFAULT() BQ769X2_APPLY_NOCONV(Cell8Interconnect, Cell8Interconnect_DEFAULTRAW)
#define SET_Cell9Interconnect_DEFAULT() BQ769X2_APPLY_NOCONV(Cell9Interconnect, Cell9Interconnect_DEFAULTRAW)
#define SET_Cell10Interconnect_DEFAULT() BQ769X2_APPLY_NOCONV(Cell10Interconnect, Cell10Interconnect_DEFAULTRAW)
#define SET_Cell11Interconnect_DEFAULT() BQ769X2_APPLY_NOCONV(Cell11Interconnect, Cell11Interconnect_DEFAULTRAW)
#define SET_Cell12Interconnect_DEFAULT() BQ769X2_APPLY_NOCONV(Cell12Interconnect, Cell12Interconnect_DEFAULTRAW)
#define SET_Cell13Interconnect_DEFAULT() BQ769X2_APPLY_NOCONV(Cell13Interconnect, Cell13Interconnect_DEFAULTRAW)
#define SET_Cell14Interconnect_DEFAULT() BQ769X2_APPLY_NOCONV(Cell14Interconnect, Cell14Interconnect_DEFAULTRAW)
#define SET_Cell15Interconnect_DEFAULT() BQ769X2_APPLY_NOCONV(Cell15Interconnect, Cell15Interconnect_DEFAULTRAW)
#define SET_Cell16Interconnect_DEFAULT() BQ769X2_APPLY_NOCONV(Cell16Interconnect, Cell16Interconnect_DEFAULTRAW)
#define SET_MfgStatusInit_DEFAULT() BQ769X2_APPLY_NOCONV(MfgStatusInit, MfgStatusInit_DEFAULTRAW)
#define SET_BalancingConfiguration_DEFAULT() BQ769X2_APPLY_NOCONV(BalancingConfiguration, BalancingConfiguration_DEFAULTRAW)
#define SET_MinCellTemp_DEFAULT() BQ769X2_APPLY_NOCONV(MinCellTemp, MinCellTemp_DEFAULTRAW)
#define SET_MaxCellTemp_DEFAULT() BQ769X2_APPLY_NOCONV(MaxCellTemp, MaxCellTemp_DEFAULTRAW)
#define SET_MaxInternalTemp_DEFAULT() BQ769X2_APPLY_NOCONV(MaxInternalTemp, MaxInternalTemp_DEFAULTRAW)
#define SET_CellBalanceInterval_DEFAULT() BQ769X2_APPLY_NOCONV(CellBalanceInterval, CellBalanceInterval_DEFAULTRAW)
#define SET_CellBalanceMaxCells_DEFAULT() BQ769X2_APPLY_NOCONV(CellBalanceMaxCells, CellBalanceMaxCells_DEFAULTRAW)
#define SET_CellBalanceMinCellVCharge_DEFAULT() BQ769X2_APPLY_NOCONV(CellBalanceMinCellVCharge, CellBalanceMinCellVCharge_DEFAULTRAW)
#define SET_CellBalanceMinDeltaCharge_DEFAULT() BQ769X2_APPLY_NOCONV(CellBalanceMinDeltaCharge, CellBalanceMinDeltaCharge_DEFAULTRAW)
#define SET_CellBalanceStopDeltaCharge_DEFAULT() BQ769X2_APPLY_NOCONV(CellBalanceStopDeltaCharge, CellBalanceStopDeltaCharge_DEFAULTRAW)
#define SET_CellBalanceMinCellVRelax_DEFAULT() BQ769X2_APPLY_NOCONV(CellBalanceMinCellVRelax, CellBalanceMinCellVRelax_DEFAULTRAW)
#define SET_CellBalanceMinDeltaRelax_DEFAULT() BQ769X2_APPLY_NOCONV(CellBalanceMinDeltaRelax, CellBalanceMinDeltaRelax_DEFAULTRAW)
#define SET_CellBalanceStopDeltaRelax_DEFAULT() BQ769X2_APPLY_NOCONV(CellBalanceStopDeltaRelax, CellBalanceStopDeltaRelax_DEFAULTRAW)
#define SET_ShutdownCellVoltage_DEFAULT() BQ769X2_APPLY_NOCONV(ShutdownCellVoltage, ShutdownCellVoltage_DEFAULTRAW)
#define SET_ShutdownStackVoltage_DEFAULT() BQ769X2_APPLY_NOCONV(ShutdownStackVoltage, ShutdownStackVoltage_DEFAULTRAW)
#define SET_LowVShutdownDelay_DEFAULT() BQ769X2_APPLY_NOCONV(LowVShutdownDelay, LowVShutdownDelay_DEFAULTRAW)
#define SET_ShutdownTemperature_DEFAULT() BQ769X2_APPLY_NOCONV(ShutdownTemperature, ShutdownTemperature_DEFAULTRAW)
#define SET_ShutdownTemperatureDelay_DEFAULT() BQ769X2_APPLY_NOCONV(ShutdownTemperatureDelay, ShutdownTemperatureDelay_DEFAULTRAW)
#define SET_FETOffDelay_DEFAULT() BQ769X2_APPLY_NOCONV(FETOffDelay, FETOffDelay_DEFAULTRAW)
#define SET_ShutdownCommandDelay_DEFAULT() BQ769X2_APPLY_NOCONV(ShutdownCommandDelay, ShutdownCommandDelay_DEFAULTRAW)
#define SET_AutoShutdownTime_DEFAULT() BQ769X2_APPLY_NOCONV(AutoShutdownTime, AutoShutdownTime_DEFAULTRAW)
#define SET_RAMFailShutdownTime_DEFAULT() BQ769X2_APPLY_NOCONV(RAMFailShutdownTime, RAMFailShutdownTime_DEFAULTRAW)
#define SET_SleepCurrent_DEFAULT() BQ769X2_APPLY_NOCONV(SleepCurrent, SleepCurrent_DEFAULTRAW)
#define SET_VoltageTime_DEFAULT() BQ769X2_APPLY_NOCONV(VoltageTime, VoltageTime_DEFAULTRAW)
#define SET_WakeComparatorCurrent_DEFAULT() BQ769X2_APPLY_NOCONV(WakeComparatorCurrent, WakeComparatorCurrent_DEFAULTRAW)
#define SET_SleepHysteresisTime_DEFAULT() BQ769X2_APPLY_NOCONV(SleepHysteresisTime, SleepHysteresisTime_DEFAULTRAW)
#define SET_SleepChargerVoltageThreshold_DEFAULT() BQ769X2_APPLY_NOCONV(SleepChargerVoltageThreshold, SleepChargerVoltageThreshold_DEFAULTRAW)
#define SET_SleepChargerPACKTOSDelta_DEFAULT() BQ769X2_APPLY_NOCONV(SleepChargerPACKTOSDelta, SleepChargerPACKTOSDelta_DEFAULTRAW)
#define SET_ConfigRAMSignature_DEFAULT() BQ769X2_APPLY_NOCONV(ConfigRAMSignature, ConfigRAMSignature_DEFAULTRAW)
#define SET_CUVThreshold_DEFAULT() BQ769X2_APPLY_NOCONV(CUVThreshold, CUVThreshold_DEFAULTRAW)
#define SET_CUVDelay_DEFAULT() BQ769X2_APPLY_NOCONV(CUVDelay, CUVDelay_DEFAULTRAW)
#define SET_CUVRecoveryHysteresis_DEFAULT() BQ769X2_APPLY_NOCONV(CUVRecoveryHysteresis, CUVRecoveryHysteresis_DEFAULTRAW)
#define SET_COVThreshold_DEFAULT() BQ769X2_APPLY_NOCONV(COVThreshold, COVThreshold_DEFAULTRAW)
#define SET_COVDelay_DEFAULT() BQ769X2_APPLY_NOCONV(COVDelay, COVDelay_DEFAULTRAW)
#define SET_COVRecoveryHysteresis_DEFAULT() BQ769X2_APPLY_NOCONV(COVRecoveryHysteresis, COVRecoveryHysteresis_DEFAULTRAW)
#define SET_COVLLatchLimit_DEFAULT() BQ769X2_APPLY_NOCONV(COVLLatchLimit, COVLLatchLimit_DEFAULTRAW)
#define SET_COVLCounterDecDelay_DEFAULT() BQ769X2_APPLY_NOCONV(COVLCounterDecDelay, COVLCounterDecDelay_DEFAULTRAW)
#define SET_COVLRecoveryTime_DEFAULT() BQ769X2_APPLY_NOCONV(COVLRecoveryTime, COVLRecoveryTime_DEFAULTRAW)
#define SET_OCCThreshold_DEFAULT() BQ769X2_APPLY_NOCONV(OCCThreshold, OCCThreshold_DEFAULTRAW)
#define SET_OCCDelay_DEFAULT() BQ769X2_APPLY_NOCONV(OCCDelay, OCCDelay_DEFAULTRAW)
#define SET_OCCRecoveryThreshold_DEFAULT() BQ769X2_APPLY_NOCONV(OCCRecoveryThreshold, OCCRecoveryThreshold_DEFAULTRAW)
#define SET_OCCPACKTOSDelta_DEFAULT() BQ769X2_APPLY_NOCONV(OCCPACKTOSDelta, OCCPACKTOSDelta_DEFAULTRAW)
#define SET_OCD1Threshold_DEFAULT() BQ769X2_APPLY_NOCONV(OCD1Threshold, OCD1Threshold_DEFAULTRAW)
#define SET_OCD1Delay_DEFAULT() BQ769X2_APPLY_NOCONV(OCD1Delay, OCD1Delay_DEFAULTRAW)
#define SET_OCD2Threshold_DEFAULT() BQ769X2_APPLY_NOCONV(OCD2Threshold, OCD2Threshold_DEFAULTRAW)
#define SET_OCD2Delay_DEFAULT() BQ769X2_APPLY_NOCONV(OCD2Delay, OCD2Delay_DEFAULTRAW)
#define SET_SCDThreshold_DEFAULT() BQ769X2_APPLY_NOCONV(SCDThreshold, SCDThreshold_DEFAULTRAW)
#define SET_SCDDelay_DEFAULT() BQ769X2_APPLY_NOCONV(SCDDelay, SCDDelay_DEFAULTRAW)
#define SET_SCDRecoveryTime_DEFAULT() BQ769X2_APPLY_NOCONV(SCDRecoveryTime, SCDRecoveryTime_DEFAULTRAW)
#define SET_OCD3Threshold_DEFAULT() BQ769X2_APPLY_NOCONV(OCD3Threshold, OCD3Threshold_DEFAULTRAW)
#define SET_OCD3Delay_DEFAULT() BQ769X2_APPLY_NOCONV(OCD3Delay, OCD3Delay_DEFAULTRAW)
#define SET_OCDRecoveryThreshold_DEFAULT() BQ769X2_APPLY_NOCONV(OCDRecoveryThreshold, OCDRecoveryThreshold_DEFAULTRAW)
#define SET_OCDLLatchLimit_DEFAULT() BQ769X2_APPLY_NOCONV(OCDLLatchLimit, OCDLLatchLimit_DEFAULTRAW)
#define SET_OCDLCounterDecDelay_DEFAULT() BQ769X2_APPLY_NOCONV(OCDLCounterDecDelay, OCDLCounterDecDelay_DEFAULTRAW)
#define SET_OCDLRecoveryTime_DEFAULT() BQ769X2_APPLY_NOCONV(OCDLRecoveryTime, OCDLRecoveryTime_DEFAULTRAW)
#define SET_OCDLRecoveryThreshold_DEFAULT() BQ769X2_APPLY_NOCONV(OCDLRecoveryThreshold, OCDLRecoveryThreshold_DEFAULTRAW)
#define SET_SCDLLatchLimit_DEFAULT() BQ769X2_APPLY_NOCONV(SCDLLatchLimit, SCDLLatchLimit_DEFAULTRAW)
#define SET_SCDLCounterDecDelay_DEFAULT() BQ769X2_APPLY_NOCONV(SCDLCounterDecDelay, SCDLCounterDecDelay_DEFAULTRAW)
#define SET_SCDLRecoveryTime_DEFAULT() BQ769X2_APPLY_NOCONV(SCDLRecoveryTime, SCDLRecoveryTime_DEFAULTRAW)
#define SET_SCDLRecoveryThreshold_DEFAULT() BQ769X2_APPLY_NOCONV(SCDLRecoveryThreshold, SCDLRecoveryThreshold_DEFAULTRAW)
#define SET_OTCThreshold_DEFAULT() BQ769X2_APPLY_NOCONV(OTCThreshold, OTCThreshold_DEFAULTRAW)
#define SET_OTCDelay_DEFAULT() BQ769X2_APPLY_NOCONV(OTCDelay, OTCDelay_DEFAULTRAW)
#define SET_OTCRecovery_DEFAULT() BQ769X2_APPLY_NOCONV(OTCRecovery, OTCRecovery_DEFAULTRAW)
#define SET_OTDThreshold_DEFAULT() BQ769X2_APPLY_NOCONV(OTDThreshold, OTDThreshold_DEFAULTRAW)
#define SET_OTDDelay_DEFAULT() BQ769X2_APPLY_NOCONV(OTDDelay, OTDDelay_DEFAULTRAW)
#define SET_OTDRecovery_DEFAULT() BQ769X2_APPLY_NOCONV(OTDRecovery, OTDRecovery_DEFAULTRAW)
#define SET_OTFThreshold_DEFAULT() BQ769X2_APPLY_NOCONV(OTFThreshold, OTFThreshold_DEFAULTRAW)
#define SET_OTFDelay_DEFAULT() BQ769X2_APPLY_NOCONV(OTFDelay, OTFDelay_DEFAULTRAW)
#define SET_OTFRecovery_DEFAULT() BQ769X2_APPLY_NOCONV(OTFRecovery, OTFRecovery_DEFAULTRAW)
#define SET_OTINTThreshold_DEFAULT() BQ769X2_APPLY_NOCONV(OTINTThreshold, OTINTThreshold_DEFAULTRAW)
#define SET_OTINTDelay_DEFAULT() BQ769X2_APPLY_NOCONV(OTINTDelay, OTINTDelay_DEFAULTRAW)
#define SET_OTINTRecovery_DEFAULT() BQ769X2_APPLY_NOCONV(OTINTRecovery, OTINTRecovery_DEFAULTRAW)
#define SET_UTCThreshold_DEFAULT() BQ769X2_APPLY_NOCONV(UTCThreshold, UTCThreshold_DEFAULTRAW)
#define SET_UTCDelay_DEFAULT() BQ769X2_APPLY_NOCONV(UTCDelay, UTCDelay_DEFAULTRAW)
#define SET_UTCRecovery_DEFAULT() BQ769X2_APPLY_NOCONV(UTCRecovery, UTCRecovery_DEFAULTRAW)
#define SET_UTDThreshold_DEFAULT() BQ769X2_APPLY_NOCONV(UTDThreshold, UTDThreshold_DEFAULTRAW)
#define SET_UTDDelay_DEFAULT() BQ769X2_APPLY_NOCONV(UTDDelay, UTDDelay_DEFAULTRAW)
#define SET_UTDRecovery_DEFAULT() BQ769X2_APPLY_NOCONV(UTDRecovery, UTDRecovery_DEFAULTRAW)
#define SET_UTINTThreshold_DEFAULT() BQ769X2_APPLY_NOCONV(UTINTThreshold, UTINTThreshold_DEFAULTRAW)
#define SET_UTINTDelay_DEFAULT() BQ769X2_APPLY_NOCONV(UTINTDelay, UTINTDelay_DEFAULTRAW)
#define SET_UTINTRecovery_DEFAULT() BQ769X2_APPLY_NOCONV(UTINTRecovery, UTINTRecovery_DEFAULTRAW)
#define SET_ProtectionsRecoveryTime_DEFAULT() BQ769X2_APPLY_NOCONV(ProtectionsRecoveryTime, ProtectionsRecoveryTime_DEFAULTRAW)
#define SET_HWDDelay_DEFAULT() BQ769X2_APPLY_NOCONV(HWDDelay, HWDDelay_DEFAULTRAW)
#define SET_LoadDetectActiveTime_DEFAULT() BQ769X2_APPLY_NOCONV(LoadDetectActiveTime, LoadDetectActiveTime_DEFAULTRAW)
#define SET_LoadDetectRetryDelay_DEFAULT() BQ769X2_APPLY_NOCONV(LoadDetectRetryDelay, LoadDetectRetryDelay_DEFAULTRAW)
#define SET_LoadDetectTimeout_DEFAULT() BQ769X2_APPLY_NOCONV(LoadDetectTimeout, LoadDetectTimeout_DEFAULTRAW)
#define SET_PTOChargeThreshold_DEFAULT() BQ769X2_APPLY_NOCONV(PTOChargeThreshold, PTOChargeThreshold_DEFAULTRAW)
#define SET_PTODelay_DEFAULT() BQ769X2_APPLY_NOCONV(PTODelay, PTODelay_DEFAULTRAW)
#define SET_PTOReset_DEFAULT() BQ769X2_APPLY_NOCONV(PTOReset, PTOReset_DEFAULTRAW)
#define SET_CUDEPThreshold_DEFAULT() BQ769X2_APPLY_NOCONV(CUDEPThreshold, CUDEPThreshold_DEFAULTRAW)
#define SET_CUDEPDelay_DEFAULT() BQ769X2_APPLY_NOCONV(CUDEPDelay, CUDEPDelay_DEFAULTRAW)
#define SET_SUVThreshold_DEFAULT() BQ769X2_APPLY_NOCONV(SUVThreshold, SUVThreshold_DEFAULTRAW)
#define SET_SUVDelay_DEFAULT() BQ769X2_APPLY_NOCONV(SUVDelay, SUVDelay_DEFAULTRAW)
#define SET_SOVThreshold_DEFAULT() BQ769X2_APPLY_NOCONV(SOVThreshold, SOVThreshold_DEFAULTRAW)
#define SET_SOVDelay_DEFAULT() BQ769X2_APPLY_NOCONV(SOVDelay, SOVDelay_DEFAULTRAW)
#define SET_TOSSThreshold_DEFAULT() BQ769X2_APPLY_NOCONV(TOSSThreshold, TOSSThreshold_DEFAULTRAW)
#define SET_TOSSDelay_DEFAULT() BQ769X2_APPLY_NOCONV(TOSSDelay, TOSSDelay_DEFAULTRAW)
#define SET_SOCCThreshold_DEFAULT() BQ769X2_APPLY_NOCONV(SOCCThreshold, SOCCThreshold_DEFAULTRAW)
#define SET_SOCCDelay_DEFAULT() BQ769X2_APPLY_NOCONV(SOCCDelay, SOCCDelay_DEFAULTRAW)
#define SET_SOCDThreshold_DEFAULT() BQ769X2_APPLY_NOCONV(SOCDThreshold, SOCDThreshold_DEFAULTRAW)
#define SET_SOCDDelay_DEFAULT() BQ769X2_APPLY_NOCONV(SOCDDelay, SOCDDelay_DEFAULTRAW)
#define SET_SOTThreshold_DEFAULT() BQ769X2_APPLY_NOCONV(SOTThreshold, SOTThreshold_DEFAULTRAW)
#define SET_SOTDelay_DEFAULT() BQ769X2_APPLY_NOCONV(SOTDelay, SOTDelay_DEFAULTRAW)
#define SET_SOTFThreshold_DEFAULT() BQ769X2_APPLY_NOCONV(SOTFThreshold, SOTFThreshold_DEFAULTRAW)
#define SET_SOTFDelay_DEFAULT() BQ769X2_APPLY_NOCONV(SOTFDelay, SOTFDelay_DEFAULTRAW)
#define SET_VIMRCheckVoltage_DEFAULT() BQ769X2_APPLY_NOCONV(VIMRCheckVoltage, VIMRCheckVoltage_DEFAULTRAW)
#define SET_VIMRMaxRelaxCurrent_DEFAULT() BQ769X2_APPLY_NOCONV(VIMRMaxRelaxCurrent, VIMRMaxRelaxCurrent_DEFAULTRAW)
#define SET_VIMRThreshold_DEFAULT() BQ769X2_APPLY_NOCONV(VIMRThreshold, VIMRThreshold_DEFAULTRAW)
#define SET_VIMRDelay_DEFAULT() BQ769X2_APPLY_NOCONV(VIMRDelay, VIMRDelay_DEFAULTRAW)
#define SET_VIMRRelaxMinDuration_DEFAULT() BQ769X2_APPLY_NOCONV(VIMRRelaxMinDuration, VIMRRelaxMinDuration_DEFAULTRAW)
#define SET_VIMACheckVoltage_DEFAULT() BQ769X2_APPLY_NOCONV(VIMACheckVoltage, VIMACheckVoltage_DEFAULTRAW)
#define SET_VIMAMinActiveCurrent_DEFAULT() BQ769X2_APPLY_NOCONV(VIMAMinActiveCurrent, VIMAMinActiveCurrent_DEFAULTRAW)
#define SET_VIMAThreshold_DEFAULT() BQ769X2_APPLY_NOCONV(VIMAThreshold, VIMAThreshold_DEFAULTRAW)
#define SET_VIMADelay_DEFAULT() BQ769X2_APPLY_NOCONV(VIMADelay, VIMADelay_DEFAULTRAW)
#define SET_CFETFOFFThreshold_DEFAULT() BQ769X2_APPLY_NOCONV(CFETFOFFThreshold, CFETFOFFThreshold_DEFAULTRAW)
#define SET_CFETFOFFDelay_DEFAULT() BQ769X2_APPLY_NOCONV(CFETFOFFDelay, CFETFOFFDelay_DEFAULTRAW)
#define SET_DFETFOFFThreshold_DEFAULT() BQ769X2_APPLY_NOCONV(DFETFOFFThreshold, DFETFOFFThreshold_DEFAULTRAW)
#define SET_DFETFOFFDelay_DEFAULT() BQ769X2_APPLY_NOCONV(DFETFOFFDelay, DFETFOFFDelay_DEFAULTRAW)
#define SET_VSSFFailThreshold_DEFAULT() BQ769X2_APPLY_NOCONV(VSSFFailThreshold, VSSFFailThreshold_DEFAULTRAW)
#define SET_VSSFDelay_DEFAULT() BQ769X2_APPLY_NOCONV(VSSFDelay, VSSFDelay_DEFAULTRAW)
#define SET_PF2LVLDelay_DEFAULT() BQ769X2_APPLY_NOCONV(PF2LVLDelay, PF2LVLDelay_DEFAULTRAW)
#define SET_LFOFDelay_DEFAULT() BQ769X2_APPLY_NOCONV(LFOFDelay, LFOFDelay_DEFAULTRAW)
#define SET_HWMXDelay_DEFAULT() BQ769X2_APPLY_NOCONV(HWMXDelay, HWMXDelay_DEFAULTRAW)
#define SET_SecuritySettings_DEFAULT() BQ769X2_APPLY_NOCONV(SecuritySettings, SecuritySettings_DEFAULTRAW)
#define SET_UnsealKeyStep1_DEFAULT() BQ769X2_APPLY_NOCONV(UnsealKeyStep1, UnsealKeyStep1_DEFAULTRAW)
#define SET_UnsealKeyStep2_DEFAULT() BQ769X2_APPLY_NOCONV(UnsealKeyStep2, UnsealKeyStep2_DEFAULTRAW)
#define SET_FullAccessKeyStep1_DEFAULT() BQ769X2_APPLY_NOCONV(FullAccessKeyStep1, FullAccessKeyStep1_DEFAULTRAW)
#define SET_FullAccessKeyStep2_DEFAULT() BQ769X2_APPLY_NOCONV(FullAccessKeyStep2, FullAccessKeyStep2_DEFAULTRAW)

/* Table 13-37. Data Memory Table
Class,Subclass,Address,Name,LibName,Type,Min Value,Max Value,Default,Units
Calibration,Voltage,0x9180,Cell 1 Gain,Cell1Gain,I2,-32767,32767,0,—
Calibration,Voltage,0x9182,Cell 2 Gain,Cell2Gain,I2,-32767,32767,0,—
Calibration,Voltage,0x9184,Cell 3 Gain,Cell3Gain,I2,-32767,32767,0,—
Calibration,Voltage,0x9186,Cell 4 Gain,Cell4Gain,I2,-32767,32767,0,—
Calibration,Voltage,0x9188,Cell 5 Gain,Cell5Gain,I2,-32767,32767,0,—
Calibration,Voltage,0x918A,Cell 6 Gain,Cell6Gain,I2,-32767,32767,0,—
Calibration,Voltage,0x918C,Cell 7 Gain,Cell7Gain,I2,-32767,32767,0,—
Calibration,Voltage,0x918E,Cell 8 Gain,Cell8Gain,I2,-32767,32767,0,—
Calibration,Voltage,0x9190,Cell 9 Gain,Cell9Gain,I2,-32767,32767,0,—
Calibration,Voltage,0x9192,Cell 10 Gain,Cell10Gain,I2,-32767,32767,0,—
Calibration,Voltage,0x9194,Cell 11 Gain,Cell11Gain,I2,-32767,32767,0,—
Calibration,Voltage,0x9196,Cell 12 Gain,Cell12Gain,I2,-32767,32767,0,—
Calibration,Voltage,0x9198,Cell 13 Gain,Cell13Gain,I2,-32767,32767,0,—
Calibration,Voltage,0x919A,Cell 14 Gain,Cell14Gain,I2,-32767,32767,0,—
Calibration,Voltage,0x919C,Cell 15 Gain,Cell15Gain,I2,-32767,32767,0,—
Calibration,Voltage,0x919E,Cell 16 Gain,Cell16Gain,I2,-32767,32767,0,—
Calibration,Voltage,0x91A0,Pack Gain,PackGain,U2,0,65535,0,—
Calibration,Voltage,0x91A2,TOS Gain,TOSGain,U2,0,65535,0,—
Calibration,Voltage,0x91A4,LD Gain,LDGain,U2,0,65535,0,—
Calibration,Voltage,0x91A6,ADC Gain,ADCGain,I2,-32767,32767,0,—
Calibration,Current,0x91A8,CC Gain,CCGain,F4,1.00E-02,10.00E+02,7.4768,—
Calibration,Current,0x91AC,Capacity Gain,CapacityGain,F4,2.98262E+03,4.193046E+08,2230042.463,—
Calibration,Vcell Offset,0x91B0,Vcell Offset,VcellOffset,I2,-32767,32767,0,mV
Calibration,V Divider Offset,0x91B2,Vdiv Offset,VdivOffset,I2,-32767,32767,0,userV
Calibration,Current Offset,0x91C6,Coulomb Counter Offset Samples,CoulombCounterOffsetSamples,U2,0,65535,64,—
Calibration,Current Offset,0x91C8,Board Offset,BoardOffset,I2,-32768,32767,0,—
Calibration,Temperature,0x91CA,Internal Temp Offset,InternalTempOffset,I1,-128,127,0,0.1°C
Calibration,Temperature,0x91CB,CFETOFF Temp Offset,CFETOFFTempOffset,I1,-128,127,0,0.1°C
Calibration,Temperature,0x91CC,DFETOFF Temp Offset,DFETOFFTempOffset,I1,-128,127,0,0.1°C
Calibration,Temperature,0x91CD,ALERT Temp Offset,ALERTTempOffset,I1,-128,127,0,0.1°C
Calibration,Temperature,0x91CE,TS1 Temp Offset,TS1TempOffset,I1,-128,127,0,0.1°C
Calibration,Temperature,0x91CF,TS2 Temp Offset,TS2TempOffset,I1,-128,127,0,0.1°C
Calibration,Temperature,0x91D0,TS3 Temp Offset,TS3TempOffset,I1,-128,127,0,0.1°C
Calibration,Temperature,0x91D1,HDQ Temp Offset,HDQTempOffset,I1,-128,127,0,0.1°C
Calibration,Temperature,0x91D2,DCHG Temp Offset,DCHGTempOffset,I1,-128,127,0,0.1°C
Calibration,Temperature,0x91D3,DDSG Temp Offset,DDSGTempOffset,I1,-128,127,0,0.1°C
Calibration,Internal Temp Model,0x91E2,Int Gain,IntGain,I2,-32768,32767,25390,—
Calibration,Internal Temp Model,0x91E4,Int base offset,Intbaseoffset,I2,-32768,32767,3032,—
Calibration,Internal Temp Model,0x91E6,Int Maximum AD,IntMaximumAD,I2,-32768,32767,16383,—
Calibration,Internal Temp Model,0x91E8,Int Maximum Temp,IntMaximumTemp,I2,0,32767,6379,0.1K
Calibration,18K Temperature Model,0x91EA,Coeff a1,T18kCoeffa1,I2,-32768,32767,-15524,—
Calibration,18K Temperature Model,0x91EC,Coeff a2,T18kCoeffa2,I2,-32768,32767,26423,—
Calibration,18K Temperature Model,0x91EE,Coeff a3,T18kCoeffa3,I2,-32768,32767,-22664,—
Calibration,18K Temperature Model,0x91F0,Coeff a4,T18kCoeffa4,I2,-32768,32767,28834,—
Calibration,18K Temperature Model,0x91F2,Coeff a5,T18kCoeffa5,I2,-32768,32767,672,—
Calibration,18K Temperature Model,0x91F4,Coeff b1,T18kCoeffb1,I2,-32768,32767,-371,—
Calibration,18K Temperature Model,0x91F6,Coeff b2,T18kCoeffb2,I2,-32768,32767,708,—
Calibration,18K Temperature Model,0x91F8,Coeff b3,T18kCoeffb3,I2,-32768,32767,-3498,—
Calibration,18K Temperature Model,0x91FA,Coeff b4,T18kCoeffb4,I2,-32768,32767,5051,—
Calibration,18K Temperature Model,0x91FE,Adc0,T18kAdc0,I2,-32768,32767,11703,—
Calibration,180K Temperature Model,0x9200,Coeff a1,T180kCoeffa1,I2,-32768,32767,-17513,—
Calibration,180K Temperature Model,0x9202,Coeff a2,T180kCoeffa2,I2,-32768,32767,25759,—
Calibration,180K Temperature Model,0x9204,Coeff a3,T180kCoeffa3,I2,-32768,32767,-23593,—
Calibration,180K Temperature Model,0x9206,Coeff a4,T180kCoeffa4,I2,-32768,32767,32175,—
Calibration,180K Temperature Model,0x9208,Coeff a5,T180kCoeffa5,I2,-32768,32767,2090,—
Calibration,180K Temperature Model,0x920A,Coeff b1,T180kCoeffb1,I2,-32768,32767,-2055,—
Calibration,180K Temperature Model,0x920C,Coeff b2,T180kCoeffb2,I2,-32768,32767,2955,—
Calibration,180K Temperature Model,0x920E,Coeff b3,T180kCoeffb3,I2,-32768,32767,-3427,—
Calibration,180K Temperature Model,0x9210,Coeff b4,T180kCoeffb4,I2,-32768,32767,4385,—
Calibration,180K Temperature Model,0x9214,Adc0,T180kAdc0,I2,-32768,32767,17246,—
Calibration,Custom Temperature Model,0x9216,Coeff a1,CustomCoeffa1,I2,-32768,32767,0,—
Calibration,Custom Temperature Model,0x9218,Coeff a2,CustomCoeffa2,I2,-32768,32767,0,—
Calibration,Custom Temperature Model,0x921A,Coeff a3,CustomCoeffa3,I2,-32768,32767,0,—
Calibration,Custom Temperature Model,0x921C,Coeff a4,CustomCoeffa4,I2,-32768,32767,0,—
Calibration,Custom Temperature Model,0x921E,Coeff a5,CustomCoeffa5,I2,-32768,32767,0,—
Calibration,Custom Temperature Model,0x9220,Coeff b1,CustomCoeffb1,I2,-32768,32767,0,—
Calibration,Custom Temperature Model,0x9222,Coeff b2,CustomCoeffb2,I2,-32768,32767,0,—
Calibration,Custom Temperature Model,0x9224,Coeff b3,CustomCoeffb3,I2,-32768,32767,0,—
Calibration,Custom Temperature Model,0x9226,Coeff b4,CustomCoeffb4,I2,-32768,32767,0,—
Calibration,Custom Temperature Model,0x9228,Rc0,CustomRc0,I2,-32768,32767,0,—
Calibration,Custom Temperature Model,0x922A,Adc0,CustomAdc0,I2,-32768,32767,0,—
Calibration,Current Deadband,0x922D,Coulomb Counter Deadband,CoulombCounterDeadband,U1,0,255,9,234nV
Calibration,CUV,0x91D4,CUV Threshold Override,CUVThresholdOverride,U2,0x0000,0xFFFF,0xFFFF,Raw
Calibration,COV,0x91D6,COV Threshold Override,COVThresholdOverride,U2,0x0000,0xFFFF,0xFFFF,Raw
Settings,Fuse,0x9231,Min Blow Fuse Voltage,MinBlowFuseVoltage,I2,0,32767,500,10mV
Settings,Fuse,0x9233,Fuse Blow Timeout,FuseBlowTimeout,U1,0,255,30,s
Settings,Configuration,0x9234,Power Config,PowerConfig,H2,0x0000,0xFFFF,0x2982,Raw
Settings,Configuration,0x9236,REG12 Config,REG12Config,H1,0x00,0xFF,0x00,Raw
Settings,Configuration,0x9237,REG0 Config,REG0Config,H1,0x00,0x03,0x00,Raw
Settings,Configuration,0x9238,HWD Regulator Options,HWDRegulatorOptions,H1,0x00,0xFF,0x00,Raw
Settings,Configuration,0x9239,Comm Type,CommType,U1,0x00,0x1F,0,—
Settings,Configuration,0x923A,I2C Address,I2CAddress,U1,0x00,0xFF,0,—
Settings,Configuration,0x923C,SPI Configuration,SPIConfiguration,H1,0x00,0x7F,0x20,—
Settings,Configuration,0x923D,Comm Idle Time,CommIdleTime,U1,0,255,0,s
Settings,Configuration,0x92FA,CFETOFF Pin Config,CFETOFFPinConfig,H1,0x00,0xFF,0x00,Raw
Settings,Configuration,0x92FB,DFETOFF Pin Config,DFETOFFPinConfig,H1,0x00,0xFF,0x00,Raw
Settings,Configuration,0x92FC,ALERT Pin Config,ALERTPinConfig,H1,0x00,0xFF,0x00,Raw
Settings,Configuration,0x92FD,TS1 Config,TS1Config,H1,0x00,0xFF,0x07,Raw
Settings,Configuration,0x92FE,TS2 Config,TS2Config,H1,0x00,0xFF,0x00,Raw
Settings,Configuration,0x92FF,TS3 Config,TS3Config,H1,0x00,0xFF,0x00,Raw
Settings,Configuration,0x9300,HDQ Pin Config,HDQPinConfig,H1,0x00,0xFF,0x00,Raw
Settings,Configuration,0x9301,DCHG Pin Config,DCHGPinConfig,H1,0x00,0xFF,0x00,Raw
Settings,Configuration,0x9302,DDSG Pin Config,DDSGPinConfig,H1,0x00,0xFF,0x00,Raw
Settings,Configuration,0x9303,DA Configuration,DAConfiguration,H1,0x00,0xFF,0x05,Raw
Settings,Configuration,0x9304,Vcell Mode,VCellMode,H2,0x0000,0xFFFF,0x0000,Raw
Settings,Configuration,0x9307,CC3 Samples,CC3Samples,U1,2,255,80,Num
Settings,Protection,0x925F,Protection Configuration,ProtectionConfiguration,H2,0x0000,0x07FF,0x0002,Raw
Settings,Protection,0x9261,Enabled Protections A,EnabledProtectionsA,U1,0x00,0xFF,0x88,Raw
Settings,Protection,0x9262,Enabled Protections B,EnabledProtectionsB,U1,0x00,0xFF,0x00,Raw
Settings,Protection,0x9263,Enabled Protections C,EnabledProtectionsC,U1,0x00,0xFF,0x00,Raw
Settings,Protection,0x9265,CHG FET Protections A,CHGFETProtectionsA,U1,0x00,0xFF,0x98,Raw
Settings,Protection,0x9266,CHG FET Protections B,CHGFETProtectionsB,U1,0x00,0xFF,0xD5,Raw
Settings,Protection,0x9267,CHG FET Protections C,CHGFETProtectionsC,U1,0x00,0xFF,0x56,Raw
Settings,Protection,0x9269,DSG FET Protections A,DSGFETProtectionsA,U1,0x00,0xFF,0xE4,Raw
Settings,Protection,0x926A,DSG FET Protections B,DSGFETProtectionsB,U1,0x00,0xFF,0xE6,Raw
Settings,Protection,0x926B,DSG FET Protections C,DSGFETProtectionsC,U1,0x00,0xFF,0xE2,Raw
Settings,Protection,0x9273,Body Diode Threshold,BodyDiodeThreshold,I2,0,32767,50,mA
Settings,Alarm,0x926D,Default Alarm Mask,DefaultAlarmMask,H2,0x0000,0xFFFF,0xF800,Raw
Settings,Alarm,0x926F,SF Alert Mask A,SFAlertMaskA,U1,0x00,0xFF,0xFC,Raw
Settings,Alarm,0x9270,SF Alert Mask B,SFAlertMaskB,U1,0x00,0xFF,0xF7,Raw
Settings,Alarm,0x9271,SF Alert Mask C,SFAlertMaskC,U1,0x00,0xFF,0xF4,Raw
Settings,Alarm,0x92C4,PF Alert Mask A,PFAlertMaskA,U1,0x00,0xFF,0x5F,Raw
Settings,Alarm,0x92C5,PF Alert Mask B,PFAlertMaskB,U1,0x00,0xFF,0x9F,Raw
Settings,Alarm,0x92C6,PF Alert Mask C,PFAlertMaskC,U1,0x00,0xFF,0x00,Raw
Settings,Alarm,0x92C7,PF Alert Mask D,PFAlertMaskD,U1,0x00,0xFF,0x00,Raw
Settings,Permanent Failure,0x92C0,Enabled PF A,EnabledPFA,U1,0x00,0xFF,0x00,Raw
Settings,Permanent Failure,0x92C1,Enabled PF B,EnabledPFB,U1,0x00,0xFF,0x00,Raw
Settings,Permanent Failure,0x92C2,Enabled PF C,EnabledPFC,U1,0x00,0xFF,0x07,Raw
Settings,Permanent Failure,0x92C3,Enabled PF D,EnabledPFD,U1,0x00,0xFF,0x00,Raw
Settings,FET,0x9308,FET Options,FETOptions,H1,0x00,0xFF,0x0D,Raw
Settings,FET,0x9309,Chg Pump Control,ChgPumpControl,U1,0x00,0xFF,0x01,Raw
Settings,FET,0x930A,Precharge Start Voltage,PrechargeStartVoltage,I2,0,32767,0,mV
Settings,FET,0x930C,Precharge Stop Voltage,PrechargeStopVoltage,I2,0,32767,0,mV
Settings,FET,0x930E,Predischarge Timeout,PredischargeTimeout,U1,0,255,5,10ms
Settings,FET,0x930F,Predischarge Stop Delta,PredischargeStopDelta,U1,0,255,50,10mV
Settings,Current Thresholds,0x9310,Dsg Current Threshold,DsgCurrentThreshold,I2,0,32767,100,userA
Settings,Current Thresholds,0x9312,Chg Current Threshold,ChgCurrentThreshold,I2,0,32767,50,userA
Settings,Cell Open-Wire,0x9314,Check Time,CheckTime,U1,0,255,5,s
Settings,Interconnect Resistances,0x9315,Cell 1 Interconnect,Cell1Interconnect,I2,0,32767,0,mΩ
Settings,Interconnect Resistances,0x9317,Cell 2 Interconnect,Cell2Interconnect,I2,0,32767,0,mΩ
Settings,Interconnect Resistances,0x9319,Cell 3 Interconnect,Cell3Interconnect,I2,0,32767,0,mΩ
Settings,Interconnect Resistances,0x931B,Cell 4 Interconnect,Cell4Interconnect,I2,0,32767,0,mΩ
Settings,Interconnect Resistances,0x931D,Cell 5 Interconnect,Cell5Interconnect,I2,0,32767,0,mΩ
Settings,Interconnect Resistances,0x931F,Cell 6 Interconnect,Cell6Interconnect,I2,0,32767,0,mΩ
Settings,Interconnect Resistances,0x9321,Cell 7 Interconnect,Cell7Interconnect,I2,0,32767,0,mΩ
Settings,Interconnect Resistances,0x9323,Cell 8 Interconnect,Cell8Interconnect,I2,0,32767,0,mΩ
Settings,Interconnect Resistances,0x9325,Cell 9 Interconnect,Cell9Interconnect,I2,0,32767,0,mΩ
Settings,Interconnect Resistances,0x9327,Cell 10 Interconnect,Cell10Interconnect,I2,0,32767,0,mΩ
Settings,Interconnect Resistances,0x9329,Cell 11 Interconnect,Cell11Interconnect,I2,0,32767,0,mΩ
Settings,Interconnect Resistances,0x932B,Cell 12 Interconnect,Cell12Interconnect,I2,0,32767,0,mΩ
Settings,Interconnect Resistances,0x932D,Cell 13 Interconnect,Cell13Interconnect,I2,0,32767,0,mΩ
Settings,Interconnect Resistances,0x932F,Cell 14 Interconnect,Cell14Interconnect,I2,0,32767,0,mΩ
Settings,Interconnect Resistances,0x9331,Cell 15 Interconnect,Cell15Interconnect,I2,0,32767,0,mΩ
Settings,Interconnect Resistances,0x9333,Cell 16 Interconnect,Cell16Interconnect,I2,0,32767,0,mΩ
Settings,Manufacturing,0x9343,Mfg Status Init,MfgStatusInit,H2,0x0000,0xFFFF,0x0040,Raw
Settings,Cell Balancing Config,0x9335,Balancing Configuration,BalancingConfiguration,H1,0x00,0xFF,0x00,Raw
Settings,Cell Balancing Config,0x9336,Min Cell Temp,MinCellTemp,I1,-128,127,-20,°C
Settings,Cell Balancing Config,0x9337,Max Cell Temp,MaxCellTemp,I1,-128,127,60,°C
Settings,Cell Balancing Config,0x9338,Max Internal Temp,MaxInternalTemp,I1,-128,127,70,°C
Settings,Cell Balancing Config,0x9339,Cell Balance Interval,CellBalanceInterval,U1,1,255,20,s
Settings,Cell Balancing Config,0x933A,Cell Balance Max Cells,CellBalanceMaxCells,U1,0,16,1,Num
Settings,Cell Balancing Config,0x933B,Cell Balance Min Cell V (Charge),CellBalanceMinCellVCharge,I2,0,5000,3900,mV
Settings,Cell Balancing Config,0x933D,Cell Balance Min Delta (Charge),CellBalanceMinDeltaCharge,U1,0,255,40,mV
Settings,Cell Balancing Config,0x933E,Cell Balance Stop Delta (Charge),CellBalanceStopDeltaCharge,U1,0,255,20,mV
Settings,Cell Balancing Config,0x933F,Cell Balance Min Cell V (Relax),CellBalanceMinCellVRelax,I2,0,5000,3900,mV
Settings,Cell Balancing Config,0x9341,Cell Balance Min Delta (Relax),CellBalanceMinDeltaRelax,U1,0,255,40,mV
Settings,Cell Balancing Config,0x9342,Cell Balance Stop Delta (Relax),CellBalanceStopDeltaRelax,U1,0,255,20,mV
Power,Shutdown,0x923F,Shutdown Cell Voltage,ShutdownCellVoltage,I2,0,32767,0,mV
Power,Shutdown,0x9241,Shutdown Stack Voltage,ShutdownStackVoltage,I2,0,32767,600,10mV
Power,Shutdown,0x9243,Low V Shutdown Delay,LowVShutdownDelay,U1,0,63,1,s
Power,Shutdown,0x9244,Shutdown Temperature,ShutdownTemperature,U1,0,150,85,°C
Power,Shutdown,0x9245,Shutdown Temperature Delay,ShutdownTemperatureDelay,U1,0,254,5,s
Power,Shutdown,0x9252,FET Off Delay,FETOffDelay,U1,0,127,0,0.25s
Power,Shutdown,0x9253,Shutdown Command Delay,ShutdownCommandDelay,U1,0,254,0,0.25s
Power,Shutdown,0x9254,Auto Shutdown Time,AutoShutdownTime,U1,0,250,0,min
Power,Shutdown,0x9255,RAM Fail Shutdown Time,RAMFailShutdownTime,U1,0,255,5,s
Power,Sleep,0x9248,Sleep Current,SleepCurrent,I2,0,32767,20,mA
Power,Sleep,0x924A,Voltage Time,VoltageTime,U1,1,255,5,s
Power,Sleep,0x924B,Wake Comparator Current,WakeComparatorCurrent,I2,500,32767,500,mA
Power,Sleep,0x924D,Sleep Hysteresis Time,SleepHysteresisTime,U1,0,255,10,s
Power,Sleep,0x924E,Sleep Charger Voltage Threshold,SleepChargerVoltageThreshold,I2,0,32767,2000,10mV
Power,Sleep,0x9250,Sleep Charger PACK-TOS Delta,SleepChargerPACKTOSDelta,I2,10,8500,200,10mV
System Data,Integrity,0x91E0,Config RAM Signature,ConfigRAMSignature,U2,0x0000,0x7FFF,0,Raw
Protections,CUV,0x9275,Threshold,CUVThreshold,U1,20,90,50,50.6mV
Protections,CUV,0x9276,Delay,CUVDelay,U2,1,2047,74,3.3,ms
Protections,CUV,0x927B,Recovery Hysteresis,CUVRecoveryHysteresis,U1,2,20,2,50.6mV
Protections,COV,0x9278,Threshold,COVThreshold,U1,20,110,86,50.6mV
Protections,COV,0x9279,Delay,COVDelay,U2,1,2047,74,3.3,ms
Protections,COV,0x927C,Recovery Hysteresis,COVRecoveryHysteresis,U1,2,20,2,50.6mV
Protections,COVL,0x927D,Latch Limit,COVLLatchLimit,U1,0,255,0,—
Protections,COVL,0x927E,Counter Dec Delay,COVLCounterDecDelay,U1,0,255,10,s
Protections,COVL,0x927F,Recovery Time,COVLRecoveryTime,U1,0,255,15,s
Protections,OCC,0x9280,Threshold,OCCThreshold,U1,2,62,2,2mV
Protections,OCC,0x9281,Delay,OCCDelay,U1,1,127,4,3.3,ms
Protections,OCC,0x9288,Recovery Threshold,OCCRecoveryThreshold,I2,-32768,32767,-200,mA
Protections,OCC,0x92B0,PACK-TOS Delta,OCCPACKTOSDelta,I2,10,8500,200,10mV
Protections,OCD1,0x9282,Threshold,OCD1Threshold,U1,2,100,4,2mV
Protections,OCD1,0x9283,Delay,OCD1Delay,U1,1,127,1,3.3,ms
Protections,OCD2,0x9284,Threshold,OCD2Threshold,U1,2,100,3,2mV
Protections,OCD2,0x9285,Delay,OCD2Delay,U1,1,127,7,3.3,ms
Protections,SCD,0x9286,Threshold,SCDThreshold,U1,0,15,0,—
Protections,SCD,0x9287,Delay,SCDDelay,U1,1,31,2,15µs
Protections,SCD,0x9294,Recovery Time,SCDRecoveryTime,U1,0,255,5,s
Protections,OCD3,0x928A,Threshold,OCD3Threshold,I2,-32768,0,-4000,userA
Protections,OCD3,0x928C,Delay,OCD3Delay,U1,0,255,2,s
Protections,OCD,0x928D,Recovery Threshold,OCDRecoveryThreshold,I2,-32768,32767,200,mA
Protections,OCDL,0x928F,Latch Limit,OCDLLatchLimit,U1,0,255,0,—
Protections,OCDL,0x9290,Counter Dec Delay,OCDLCounterDecDelay,U1,0,255,10,s
Protections,OCDL,0x9291,Recovery Time,OCDLRecoveryTime,U1,0,255,15,s
Protections,OCDL,0x9292,Recovery Threshold,OCDLRecoveryThreshold,I2,-32768,32767,200,mA
Protections,SCDL,0x9295,Latch Limit,SCDLLatchLimit,U1,0,255,0,—
Protections,SCDL,0x9296,Counter Dec Delay,SCDLCounterDecDelay,U1,0,255,10,s
Protections,SCDL,0x9297,Recovery Time,SCDLRecoveryTime,U1,0,255,15,s
Protections,SCDL,0x9298,Recovery Threshold,SCDLRecoveryThreshold,I2,-32768,32767,200,mA
Protections,OTC,0x929A,Threshold,OTCThreshold,I1,-40,120,55,°C
Protections,OTC,0x929B,Delay,OTCDelay,U1,0,255,2,s
Protections,OTC,0x929C,Recovery,OTCRecovery,I1,-40,120,50,°C
Protections,OTD,0x929D,Threshold,OTDThreshold,I1,-40,120,60,°C
Protections,OTD,0x929E,Delay,OTDDelay,U1,0,255,2,s
Protections,OTD,0x929F,Recovery,OTDRecovery,I1,-40,120,55,°C
Protections,OTF,0x92A0,Threshold,OTFThreshold,U1,0,150,80,°C
Protections,OTF,0x92A1,Delay,OTFDelay,U1,0,255,2,s
Protections,OTF,0x92A2,Recovery,OTFRecovery,U1,0,150,65,°C
Protections,OTINT,0x92A3,Threshold,OTINTThreshold,I1,-40,120,85,°C
Protections,OTINT,0x92A4,Delay,OTINTDelay,U1,0,255,2,s
Protections,OTINT,0x92A5,Recovery,OTINTRecovery,I1,-40,120,80,°C
Protections,UTC,0x92A6,Threshold,UTCThreshold,I1,-40,120,0,°C
Protections,UTC,0x92A7,Delay,UTCDelay,U1,0,255,2,s
Protections,UTC,0x92A8,Recovery,UTCRecovery,I1,-40,120,5,°C
Protections,UTD,0x92A9,Threshold,UTDThreshold,I1,-40,120,0,°C
Protections,UTD,0x92AA,Delay,UTDDelay,U1,0,255,2,s
Protections,UTD,0x92AB,Recovery,UTDRecovery,I1,-40,120,5,°C
Protections,UTINT,0x92AC,Threshold,UTINTThreshold,I1,-40,120,-20,°C
Protections,UTINT,0x92AD,Delay,UTINTDelay,U1,0,255,2,s
Protections,UTINT,0x92AE,Recovery,UTINTRecovery,I1,-40,120,-15,°C
Protections,Recovery,0x92AF,Time,ProtectionsRecoveryTime,U1,0,255,3,s
Protections,HWD,0x92B2,Delay,HWDDelay,U2,0,65535,60,s
Protections,Load Detect,0x92B4,Active Time,LoadDetectActiveTime,U1,0,255,0,s
Protections,Load Detect,0x92B5,Retry Delay,LoadDetectRetryDelay,U1,0,255,50,s
Protections,Load Detect,0x92B6,Timeout,LoadDetectTimeout,U2,0,65535,1,hrs
Protections,PTO,0x92BA,Charge Threshold,PTOChargeThreshold,I2,-32768,32767,250,mA
Protections,PTO,0x92BC,Delay,PTODelay,U2,0,65535,1800,s
Protections,PTO,0x92BE,Reset,PTOReset,I2,0,10000,2,userAh
Permanent Fail,CUDEP,0x92C8,Threshold,CUDEPThreshold,I2,0,32767,1500,mV
Permanent Fail,CUDEP,0x92CA,Delay,CUDEPDelay,U1,0,255,2,s
Permanent Fail,SUV,0x92CB,Threshold,SUVThreshold,I2,0,32767,2200,mV
Permanent Fail,SUV,0x92CD,Delay,SUVDelay,U1,0,255,5,s
Permanent Fail,SOV,0x92CE,Threshold,SOVThreshold,I2,0,32767,4500,mV
Permanent Fail,SOV,0x92D0,Delay,SOVDelay,U1,0,255,5,s
Permanent Fail,TOS,0x92D1,Threshold,TOSSThreshold,I2,0,32767,500,mV
Permanent Fail,TOS,0x92D3,Delay,TOSSDelay,U1,0,255,5,s
Permanent Fail,SOCC,0x92D4,Threshold,SOCCThreshold,I2,-32768,32767,10000,userA
Permanent Fail,SOCC,0x92D6,Delay,SOCCDelay,U1,0,255,5,s
Permanent Fail,SOCD,0x92D7,Threshold,SOCDThreshold,I2,-32768,32767,-32000,userA
Permanent Fail,SOCD,0x92D9,Delay,SOCDDelay,U1,0,255,5,s
Permanent Fail,SOT,0x92DA,Threshold,SOTThreshold,I1,-40,120,65,°C
Permanent Fail,SOT,0x92DB,Delay,SOTDelay,U1,0,255,5,s
Permanent Fail,SOTF,0x92DC,Threshold,SOTFThreshold,U1,0,150,85,°C
Permanent Fail,SOTF,0x92DD,Delay,SOTFDelay,U1,0,255,5,s
Permanent Fail,VIMR,0x92DE,Check Voltage,VIMRCheckVoltage,I2,0,5500,3500,mV
Permanent Fail,VIMR,0x92E0,Max Relax Current,VIMRMaxRelaxCurrent,I2,10,32767,10,mA
Permanent Fail,VIMR,0x92E2,Threshold,VIMRThreshold,I2,0,5500,500,mV
Permanent Fail,VIMR,0x92E4,Delay,VIMRDelay,U1,0,255,5,s
Permanent Fail,VIMR,0x92E5,Relax Min Duration,VIMRRelaxMinDuration,U2,0,65535,100,s
Permanent Fail,VIMA,0x92E7,Check Voltage,VIMACheckVoltage,I2,0,5500,3700,mV
Permanent Fail,VIMA,0x92E9,Min Active Current,VIMAMinActiveCurrent,I2,10,32767,50,mA
Permanent Fail,VIMA,0x92EB,Threshold,VIMAThreshold,I2,0,5500,200,mV
Permanent Fail,VIMA,0x92ED,Delay,VIMADelay,U1,0,255,5,s
Permanent Fail,CFETF,0x92EE,OFF Threshold,CFETFOFFThreshold,I2,10,5000,20,mA
Permanent Fail,CFETF,0x92F0,OFF Delay,CFETFOFFDelay,U1,0,255,5,s
Permanent Fail,DFETF,0x92F1,OFF Threshold,DFETFOFFThreshold,I2,-5000,-10,-20,mA
Permanent Fail,DFETF,0x92F3,OFF Delay,DFETFOFFDelay,U1,0,255,5,s
Permanent Fail,VSSF,0x92F4,Fail Threshold,VSSFFailThreshold,I2,1,32767,100,—
Permanent Fail,VSSF,0x92F6,Delay,VSSFDelay,U1,0,255,5,s
Permanent Fail,2LVL,0x92F7,Delay,PF2LVLDelay,U1,0,255,5,s
Permanent Fail,LFOF,0x92F8,Delay,LFOFDelay,U1,0,255,5,s
Permanent Fail,HWMX,0x92F9,Delay,HWMXDelay,U1,0,255,5,s
Security,Settings,0x9256,Security Settings,SecuritySettings,H1,0x00,0x07,0x00,Raw
Security,Keys,0x9257,Unseal Key Step 1,UnsealKeyStep1,U2,0x0100,0xFFFF,0x0414,Raw
Security,Keys,0x9259,Unseal Key Step 2,UnsealKeyStep2,U2,0x0100,0xFFFF,0x3672,Raw
Security,Keys,0x925B,Full Access Key Step 1,FullAccessKeyStep1,U2,0x0100,0xFFFF,0xFFFF,Raw
Security,Keys,0x925D,Full Access Key Step 2,FullAccessKeyStep2,U2,0x0100,0xFFFF,0xFFFF,Raw
*/

#endif // __BQ769X2_H