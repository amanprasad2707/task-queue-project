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
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "FreeRTOS.h"
#include "task.h"
#include <string.h>
#include "semphr.h"
#include "queue.h"
#include "timers.h"
#include "stdio.h"
#include "stdint.h"
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */
extern TaskHandle_t handle_task_menu;
extern TaskHandle_t handle_task_print;
extern TaskHandle_t handle_task_rtc;
extern TaskHandle_t handle_task_led;
extern TaskHandle_t handle_task_cmd;

extern SemaphoreHandle_t uart_mutex;

extern QueueHandle_t q_data;
extern QueueHandle_t q_print;

extern UART_HandleTypeDef huart2;

extern TimerHandle_t timer_led[4];
extern TimerHandle_t timer_rtc;

extern RTC_HandleTypeDef hrtc;
void show_time_date_ITM(void);
void show_time_date(void);
void config_rtc_time(RTC_TimeTypeDef *time);
void config_rtc_date(RTC_DateTypeDef *date);
uint8_t validate_rtc_info(RTC_TimeTypeDef *time, RTC_DateTypeDef *date);


/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */
typedef struct{
  uint8_t payload[10];
  uint32_t len;
}command_t;

typedef enum{
  sMainMenu = 0,
  sLedEffect,
  sRtcMenu,
  sRtcTimeConfig,
  sRtcDateConfig,
  sRtcReport,
}ScreenState_t;
/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */
extern ScreenState_t curr_screen_state;
/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */
void menu_task(void * param);
void print_task(void * param);
void cmd_task(void * param);
void led_task(void * param);
void rtc_task(void * param);

void led_effect_stop(void);
void led_effect1(void);
void led_effect2(void);
void led_effect3(void);
void led_effect4(void);
void led_effect(int n);

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define LED_GREEN_Pin GPIO_PIN_12
#define LED_GREEN_GPIO_Port GPIOD
#define LED_ORANGE_Pin GPIO_PIN_13
#define LED_ORANGE_GPIO_Port GPIOD
#define LED_RED_Pin GPIO_PIN_14
#define LED_RED_GPIO_Port GPIOD
#define LED_BLUE_Pin GPIO_PIN_15
#define LED_BLUE_GPIO_Port GPIOD

/* USER CODE BEGIN Private defines */

/* USER CODE BEGIN Private defines */
#define PRIO_TASK_LED         2
#define PRIO_TASK_PRINT       2
#define PRIO_TASK_RTC         2
#define PRIO_TASK_CMD         2
#define PRIO_TASK_MENU        2
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
