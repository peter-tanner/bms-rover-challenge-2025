#ifndef __BQ769X2_ST_H
#define __BQ769X2_ST_H

#include "stm32g0xx_hal.h"

extern TIM_HandleTypeDef htim15;
extern I2C_HandleTypeDef hi2c1;

#define DELAY_US_TIMER &htim15

void bq769x2_delay_us(uint16_t us);
uint8_t bq769x2_iic_write(uint16_t DevAddress, uint16_t MemAddress,
                          uint16_t MemAddSize, uint8_t *pData, uint16_t Size, uint32_t Timeout);
uint8_t bq769x2_iic_read(uint16_t DevAddress, uint16_t MemAddress,
                         uint16_t MemAddSize, uint8_t *pData, uint16_t Size, uint32_t Timeout);

#define bq769x2_error_handler() Error_Handler();

#endif