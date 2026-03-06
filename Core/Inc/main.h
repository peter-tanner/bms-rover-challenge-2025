/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.h
 * @brief          : Header for main.c file.
 *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32g0xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "bq769x2.h"
#include "bq769x2_config.h"
#include "bq769x2_st.h"
#include "can_types.h"
#include "can_types_uart.h"
#include "f18.h"
#include "helper.h"
#include "lwrb.h"
#include <stdbool.h>
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */
/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */
// #define ENABLE_SLEEPING
/* USER CODE END EM */

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define HCLK_MHz 16
#define HCLK_kHz HCLK_MHz*1000
#define ALARM_CALLBACK_WATCHDOG_ms 6000
#define ALERT_Pin GPIO_PIN_13
#define ALERT_GPIO_Port GPIOC
#define ALERT_EXTI_IRQn EXTI4_15_IRQn
#define TIM2_CH1__BUZZ_Pin GPIO_PIN_0
#define TIM2_CH1__BUZZ_GPIO_Port GPIOA
#define CLEAR_FAULT_Pin GPIO_PIN_1
#define CLEAR_FAULT_GPIO_Port GPIOA
#define LED_GREEN_Pin GPIO_PIN_4
#define LED_GREEN_GPIO_Port GPIOA
#define TO_PA7__HIZ_Pin GPIO_PIN_5
#define TO_PA7__HIZ_GPIO_Port GPIOA
#define FAULT_LAMP_Pin GPIO_PIN_0
#define FAULT_LAMP_GPIO_Port GPIOB
#define TO_PA14_BOOT0__HIZ_Pin GPIO_PIN_2
#define TO_PA14_BOOT0__HIZ_GPIO_Port GPIOB
#define RJ8P8C_LED_G_Pin GPIO_PIN_10
#define RJ8P8C_LED_G_GPIO_Port GPIOB
#define RJ8P8C_LED_Y_Pin GPIO_PIN_11
#define RJ8P8C_LED_Y_GPIO_Port GPIOB
#define FDCAN2_SILENCE_Pin GPIO_PIN_15
#define FDCAN2_SILENCE_GPIO_Port GPIOB
#define BUCK_EN_Pin GPIO_PIN_8
#define BUCK_EN_GPIO_Port GPIOA
#define FDCAN12_SHUTDOWN_Pin GPIO_PIN_9
#define FDCAN12_SHUTDOWN_GPIO_Port GPIOA
#define TO_PD1__HIZ_Pin GPIO_PIN_6
#define TO_PD1__HIZ_GPIO_Port GPIOC
#define TO_PD0__HIZ_Pin GPIO_PIN_7
#define TO_PD0__HIZ_GPIO_Port GPIOC
#define FDCAN1_SILENCE_Pin GPIO_PIN_10
#define FDCAN1_SILENCE_GPIO_Port GPIOA
#define TO_PA14_BOOT0__HIZA15_Pin GPIO_PIN_15
#define TO_PA14_BOOT0__HIZA15_GPIO_Port GPIOA
#define TO_PA14_BOOT0__HIZD2_Pin GPIO_PIN_2
#define TO_PA14_BOOT0__HIZD2_GPIO_Port GPIOD
#define RST_SHUT_Pin GPIO_PIN_3
#define RST_SHUT_GPIO_Port GPIOD
#define GND__HIZ_Pin GPIO_PIN_3
#define GND__HIZ_GPIO_Port GPIOB
#define CFETOFF_Pin GPIO_PIN_4
#define CFETOFF_GPIO_Port GPIOB
#define DFETOFF_Pin GPIO_PIN_5
#define DFETOFF_GPIO_Port GPIOB
#define GND__HIZB6_Pin GPIO_PIN_6
#define GND__HIZB6_GPIO_Port GPIOB
#define GND__HIZB7_Pin GPIO_PIN_7
#define GND__HIZB7_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
