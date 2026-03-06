
#include "bq769x2_st.h"
#include "main.h"

void bq769x2_delay_us(uint16_t us)
{                                      // Sets the delay in microseconds.
    __HAL_TIM_SET_COUNTER(&htim15, 0); // set the counter value a 0
    while (__HAL_TIM_GET_COUNTER(&htim15) < us)
        ; // wait for the counter to reach the us input in the parameter
}

uint8_t bq769x2_iic_write(uint16_t DevAddress, uint16_t MemAddress,
                          uint16_t MemAddSize, uint8_t *pData, uint16_t Size, uint32_t Timeout)
{
    return HAL_I2C_Mem_Write(&hi2c1, DevAddress, MemAddress, MemAddSize, pData, Size, Timeout);
}
uint8_t bq769x2_iic_read(uint16_t DevAddress, uint16_t MemAddress,
                         uint16_t MemAddSize, uint8_t *pData, uint16_t Size, uint32_t Timeout)
{
    return HAL_I2C_Mem_Read(&hi2c1, DevAddress, MemAddress, MemAddSize, pData, Size, Timeout);
}