/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
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
#include "stm32f1xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define AX2_HOME_Pin GPIO_PIN_14
#define AX2_HOME_GPIO_Port GPIOC
#define AX2_MODE2_Pin GPIO_PIN_15
#define AX2_MODE2_GPIO_Port GPIOC
#define AX2_MODE1_Pin GPIO_PIN_0
#define AX2_MODE1_GPIO_Port GPIOC
#define AX2_MODE0_Pin GPIO_PIN_1
#define AX2_MODE0_GPIO_Port GPIOC
#define AX2_STEP_Pin GPIO_PIN_2
#define AX2_STEP_GPIO_Port GPIOC
#define AX2_ENBL_Pin GPIO_PIN_3
#define AX2_ENBL_GPIO_Port GPIOC
#define AX2_DIR_Pin GPIO_PIN_1
#define AX2_DIR_GPIO_Port GPIOA
#define AX2_FAULT_Pin GPIO_PIN_2
#define AX2_FAULT_GPIO_Port GPIOA
#define AX2_SLEEP_Pin GPIO_PIN_3
#define AX2_SLEEP_GPIO_Port GPIOA
#define AX2_RESET_Pin GPIO_PIN_4
#define AX2_RESET_GPIO_Port GPIOA
#define LED_INDICATOR_G_Pin GPIO_PIN_5
#define LED_INDICATOR_G_GPIO_Port GPIOA
#define TAKE_PHOTO_Pin GPIO_PIN_6
#define TAKE_PHOTO_GPIO_Port GPIOA
#define GET_FOCUS_Pin GPIO_PIN_7
#define GET_FOCUS_GPIO_Port GPIOA
#define AX1_HOME_Pin GPIO_PIN_4
#define AX1_HOME_GPIO_Port GPIOC
#define AX1_MODE2_Pin GPIO_PIN_5
#define AX1_MODE2_GPIO_Port GPIOC
#define AX1_MODE1_Pin GPIO_PIN_0
#define AX1_MODE1_GPIO_Port GPIOB
#define AX1_MODE0_Pin GPIO_PIN_1
#define AX1_MODE0_GPIO_Port GPIOB
#define LED_INDICATOR_R_Pin GPIO_PIN_2
#define LED_INDICATOR_R_GPIO_Port GPIOB
#define AX1_STEP_Pin GPIO_PIN_12
#define AX1_STEP_GPIO_Port GPIOB
#define AX1_ENBL_Pin GPIO_PIN_13
#define AX1_ENBL_GPIO_Port GPIOB
#define AX1_DIR_Pin GPIO_PIN_14
#define AX1_DIR_GPIO_Port GPIOB
#define AX1_FAULT_Pin GPIO_PIN_15
#define AX1_FAULT_GPIO_Port GPIOB
#define AX1_SLEEP_Pin GPIO_PIN_6
#define AX1_SLEEP_GPIO_Port GPIOC
#define AX1_RESET_Pin GPIO_PIN_7
#define AX1_RESET_GPIO_Port GPIOC
#define AX0_HOME_Pin GPIO_PIN_8
#define AX0_HOME_GPIO_Port GPIOC
#define AX0_MODE2_Pin GPIO_PIN_9
#define AX0_MODE2_GPIO_Port GPIOC
#define AX0_MODE1_Pin GPIO_PIN_8
#define AX0_MODE1_GPIO_Port GPIOA
#define AX0_MODE0_Pin GPIO_PIN_15
#define AX0_MODE0_GPIO_Port GPIOA
#define AX0_STEP_Pin GPIO_PIN_10
#define AX0_STEP_GPIO_Port GPIOC
#define AX0_ENBL_Pin GPIO_PIN_11
#define AX0_ENBL_GPIO_Port GPIOC
#define AX0_RESET_Pin GPIO_PIN_12
#define AX0_RESET_GPIO_Port GPIOC
#define AX0_SLEEP_Pin GPIO_PIN_2
#define AX0_SLEEP_GPIO_Port GPIOD
#define AX0_FAULT_Pin GPIO_PIN_4
#define AX0_FAULT_GPIO_Port GPIOB
#define AX0_DIR_Pin GPIO_PIN_5
#define AX0_DIR_GPIO_Port GPIOB
#define LED_INDICATOR_B_Pin GPIO_PIN_6
#define LED_INDICATOR_B_GPIO_Port GPIOB
#define RPI_ENABLE_Pin GPIO_PIN_7
#define RPI_ENABLE_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
