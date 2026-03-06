#ifndef __BQ769X2_CONFIG_H
#define __BQ769X2_CONFIG_H

#include "bq769x2.h"
#include "stm32g0xx_hal.h"

extern I2C_HandleTypeDef hi2c1;

// CONSTANTS

// INR21700-50XG
// Charge : 0 to 60℃
// (Recommended recharge release < 45℃)
// Discharge:-20 to 80℃
// (Recommended re-discharge release < 60℃)
#define CELL_CHARGE_MAX_degC 45
#define CELL_CHARGE_MIN_degC 0
#define CELL_DISCHARGE_MAX_degC 60
#define CELL_DISCHARGE_MIN_degC -20
#define CELL_MIN_degC (CELL_CHARGE_MIN_degC > CELL_DISCHARGE_MIN_degC ? CELL_CHARGE_MIN_degC : CELL_DISCHARGE_MIN_degC)
#define CELL_MAX_degC (CELL_CHARGE_MAX_degC < CELL_DISCHARGE_MAX_degC ? CELL_CHARGE_MAX_degC : CELL_DISCHARGE_MAX_degC)

void bq769x2_init(void);

#endif // __BQ769X2_CONFIG_H
