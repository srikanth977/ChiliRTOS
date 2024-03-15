#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "stm32f401xe.h"
#include "Interrupts.h"



extern volatile uint32_t tick;
extern volatile uint32_t _tick;


#define GPIO_PIN_0                 ((uint16_t)0x0001)  /* Pin 0 selected    */
#define GPIO_PIN_1                 ((uint16_t)0x0002)  /* Pin 1 selected    */
#define GPIO_PIN_2                 ((uint16_t)0x0004)  /* Pin 2 selected    */
#define GPIO_PIN_3                 ((uint16_t)0x0008)  /* Pin 3 selected    */
#define GPIO_PIN_4                 ((uint16_t)0x0010)  /* Pin 4 selected    */
#define GPIO_PIN_5                 ((uint16_t)0x0020)  /* Pin 5 selected    */
#define GPIO_PIN_6                 ((uint16_t)0x0040)  /* Pin 6 selected    */
#define GPIO_PIN_7                 ((uint16_t)0x0080)  /* Pin 7 selected    */
#define GPIO_PIN_8                 ((uint16_t)0x0100)  /* Pin 8 selected    */
#define GPIO_PIN_9                 ((uint16_t)0x0200)  /* Pin 9 selected    */
#define GPIO_PIN_10                ((uint16_t)0x0400)  /* Pin 10 selected   */
#define GPIO_PIN_11                ((uint16_t)0x0800)  /* Pin 11 selected   */
#define GPIO_PIN_12                ((uint16_t)0x1000)  /* Pin 12 selected   */
#define GPIO_PIN_13                ((uint16_t)0x2000)  /* Pin 13 selected   */
#define GPIO_PIN_14                ((uint16_t)0x4000)  /* Pin 14 selected   */
#define GPIO_PIN_15                ((uint16_t)0x8000)  /* Pin 15 selected   */
#define GPIO_PIN_All               ((uint16_t)0xFFFF)  /* All pins selected */


#define MY_LED_Pin GPIO_PIN_13
#define MY_LED_GPIO_Port GPIOC
#define DELL_SMALLGREEN_Pin GPIO_PIN_0
#define DELL_SMALLGREEN_GPIO_Port GPIOA
#define DELL_YELLOW_Pin GPIO_PIN_1
#define DELL_YELLOW_GPIO_Port GPIOA
#define DELL_HDD_Pin GPIO_PIN_2
#define DELL_HDD_GPIO_Port GPIOA

void DELL_SMALLGREEN_OFF(void);
void DELL_SMALLGREEN_ON(void);

void MY_LED_OFF(void);
void MY_LED_ON(void);

void DELL_YELLOW_OFF(void);
void DELL_YELLOW_ON(void);

void DELL_HDD_OFF(void);
void DELL_HDD_ON(void);

void Task0(void);
void Task1(void);
void Task2(void);

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
