#ifndef __STM32F4_RTOS_BSP_H
#define __STM32F4_RTOS_BSP_H


#include <stdint.h>
#include "stm32f4xx_hal.h"
#include "string.h"
#include "LCD_20x4.h"
#include <stdio.h>


/* Peripheral declarations for HAL */
extern ADC_HandleTypeDef hadc1;
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim4;
extern TIM_HandleTypeDef htim5;


/* Probe LED GPIO definitions */
#define P3_MY_LED_Pin GPIO_PIN_13
#define P3_MY_LED_GPIO_Port GPIOC
#define P0_DELL_SMALLGREEN_Pin GPIO_PIN_0
#define P0_DELL_SMALLGREEN_GPIO_Port GPIOA
#define P1_DELL_YELLOW_Pin GPIO_PIN_1
#define P1_DELL_YELLOW_GPIO_Port GPIOA
#define P2_DELL_HDD_Pin GPIO_PIN_2
#define P2_DELL_HDD_GPIO_Port GPIOA
#define LCD_D0_Pin GPIO_PIN_0
#define LCD_D0_GPIO_Port GPIOB
#define LCD_D1_Pin GPIO_PIN_1
#define LCD_D1_GPIO_Port GPIOB
#define LCD_D2_Pin GPIO_PIN_2
#define LCD_D2_GPIO_Port GPIOB
#define LCD_RS_Pin GPIO_PIN_8
#define LCD_RS_GPIO_Port GPIOA
#define LCD_RW_Pin GPIO_PIN_9
#define LCD_RW_GPIO_Port GPIOA
#define LCD_EN_Pin GPIO_PIN_10
#define LCD_EN_GPIO_Port GPIOA
#define LCD_D3_Pin GPIO_PIN_3
#define LCD_D3_GPIO_Port GPIOB
#define LCD_D4_Pin GPIO_PIN_4
#define LCD_D4_GPIO_Port GPIOB
#define LCD_D5_Pin GPIO_PIN_5
#define LCD_D5_GPIO_Port GPIOB
#define LCD_D6_Pin GPIO_PIN_6
#define LCD_D6_GPIO_Port GPIOB
#define LCD_D7_Pin GPIO_PIN_7
#define LCD_D7_GPIO_Port GPIOB

extern uint16_t mysensordata;
extern char strsensordata[30];
extern uint32_t count2;
extern uint32_t count3;
extern uint32_t count4;
extern uint32_t count5;

void Oscillator_IO_Init(void);
void SystemClock_Config(void);
void MX_Probe_IO_Init(void);
void MX_ADC1_Init(void);
void MX_TIM2_Init(void);
void MX_TIM3_Init(void);
void MX_TIM4_Init(void);
void MX_TIM5_Init(void);



#endif