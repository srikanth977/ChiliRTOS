/* VER01 --*/
//#include "main.h"
#include "osKernel.h"

#include <stdint.h>
#include "STM32F4_RTOS_BSP.h"


uint32_t volatile Taskcount0=0;
uint32_t volatile Taskcount1=0;
uint32_t volatile Taskcount2=0;


void ClockConfiguration(void);
void GPIO_Init(void);
void P0_DELL_SMALLGREEN_OFF(void);
void P0_DELL_SMALLGREEN_ON(void);

void P3_MY_LED_OFF(void);
void P3_MY_LED_ON(void);

void P1_DELL_YELLOW_OFF(void);
void P1_DELL_YELLOW_ON(void);

void P2_DELL_HDD_OFF(void);
void P2_DELL_HDD_ON(void);



void ClockConfiguration()
{
	/* First enable Power clock to set the Voltage regulator */
	RCC->APB1ENR |= (1U <<RCC_APB1ENR_PWREN_Pos);
	PWR->CR |= (3U <<PWR_CR_VOS_Pos);

	/* STEP 1 : TURN OFF PLL AND CONFIGURE IT */
	RCC->CR &= ~(1U<<RCC_CR_PLLON_Pos);
	while (RCC->CR & RCC_CR_PLLRDY){}		//WAIT TILL PLLONREADY IS ZERO
	/* PLL CFGR OUTPUT IS AS PER BELOW FORMULA
	 * HSE (25 MHZ) / PLL M (25) = 1 MHZ   	-->A
	 * A * PLL N (384) = 384 MHZ								--> B
	 * B / PLL P (4)  = 96 MHZ								--> SYSCLK
	 *
	 * This will satisfy the USB CLOCK OF 48MHZ
	 */
	RCC->PLLCFGR = (RCC->PLLCFGR & ~(RCC_PLLCFGR_PLLQ_Msk |
										RCC_PLLCFGR_PLLP_Msk |
										RCC_PLLCFGR_PLLN_Msk |
										RCC_PLLCFGR_PLLM_Msk )) |
					(25U << RCC_PLLCFGR_PLLM_Pos) |							// PLLM = /12
					(384U << RCC_PLLCFGR_PLLN_Pos) |							// PLLN = 96
					RCC_PLLCFGR_PLLQ_3 |							// PLLQ = /8
					RCC_PLLCFGR_PLLSRC |						// PLL SOURCE = HSE
					RCC_PLLCFGR_PLLP_0;							// PLL P = /4

	FLASH->ACR |= FLASH_ACR_LATENCY_3WS;							// 3 wait states
	RCC->CFGR = (RCC->CFGR & ~RCC_CFGR_PPRE1_Msk) |					//DIVIDING APB1 CLOCK /2
					RCC_CFGR_PPRE1_2;

	RCC->CR |= (1U <<RCC_CR_HSEON_Pos);										// SELECT HSE

	RCC->CR |= (1U<<RCC_CR_PLLON_Pos);										//PLL ON
	RCC->CFGR |= (2U);														// SWITCH TO PLL CLOCK


}


void GPIO_Init()
{
	/* PINS ARE LOCATED IN
	 * PC13 - ONBOARD
	 * PA0 - DELL_SMALLGREEN
	 * PA1 - DELL_YELLOW
	 * PA2 - DELL_HDD
	 */
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN | RCC_AHB1ENR_GPIOAEN;
	GPIOA->MODER = (GPIOA->MODER & ~(GPIO_MODER_MODER0 |
									GPIO_MODER_MODER1 |
									GPIO_MODER_MODER2)) |
					(GPIO_MODER_MODER0_0 |
					GPIO_MODER_MODER1_0 |
					GPIO_MODER_MODER2_0);

	GPIOC->MODER = (GPIOC->MODER & ~(GPIO_MODER_MODER13)) |
					(GPIO_MODER_MODE13_0);

	/* Setting Mode of Pins to Output Low speed */

}

void Task0(void)
{
	
	while(1)
	{
		Taskcount0++;
		P0_DELL_SMALLGREEN_ON();
		P1_DELL_YELLOW_OFF();
		P2_DELL_HDD_OFF();
		//osThreadYield();
		
	}
	
}


void Task1(void)
{
	
	while(1)
	{
		Taskcount1++;
		P0_DELL_SMALLGREEN_OFF();
		P1_DELL_YELLOW_ON();
		P2_DELL_HDD_OFF();
		//osThreadYield();
	}
	
}


void Task2(void)
{
	
	
	while(1)
	{
		Taskcount2++;
		P0_DELL_SMALLGREEN_OFF();
		P1_DELL_YELLOW_OFF();
		P2_DELL_HDD_ON();
		//osThreadYield();
	}
	
}

///* Timer interrupt equivalent to Task3 */
//void TIM4_IRQHandler(void)
//{
//  /* USER CODE BEGIN TIM4_IRQn 0 */
//	//++count4;
//	HAL_GPIO_TogglePin(P3_MY_LED_GPIO_Port,P3_MY_LED_Pin);
//  /* USER CODE END TIM4_IRQn 0 */
//  HAL_TIM_IRQHandler(&htim4);
//  /* USER CODE BEGIN TIM4_IRQn 1 */

//  /* USER CODE END TIM4_IRQn 1 */
//}

void Periodic_Task3()
{
	/* These threads will not execute indefinitely, they execute once */
	HAL_GPIO_TogglePin(P3_MY_LED_GPIO_Port,P3_MY_LED_Pin);
}

void Periodic_Task4()
{
	/* These threads will not execute indefinitely, they execute once */
	HAL_GPIO_TogglePin(P4_LED_GPIO_Port,P4_LED_Pin);
}

void P1_DELL_YELLOW_ON(void)
{
	P1_DELL_YELLOW_GPIO_Port->ODR |=P1_DELL_YELLOW_Pin;
}

void P1_DELL_YELLOW_OFF(void)
{
	P1_DELL_YELLOW_GPIO_Port->ODR &= ~P1_DELL_YELLOW_Pin;
}

void P3_MY_LED_ON(void)
{
	P3_MY_LED_GPIO_Port->ODR |=P3_MY_LED_Pin;
}

void P3_MY_LED_OFF(void)
{
	P3_MY_LED_GPIO_Port->ODR &=~P3_MY_LED_Pin;
}


void P0_DELL_SMALLGREEN_ON(void)
{
	P0_DELL_SMALLGREEN_GPIO_Port->ODR |=P0_DELL_SMALLGREEN_Pin;
}

void P0_DELL_SMALLGREEN_OFF(void)
{
	P0_DELL_SMALLGREEN_GPIO_Port->ODR &=~P0_DELL_SMALLGREEN_Pin;
}


void P2_DELL_HDD_ON(void)
{
	P2_DELL_HDD_GPIO_Port->ODR |=P2_DELL_HDD_Pin;
}
void P2_DELL_HDD_OFF(void)
{
	P2_DELL_HDD_GPIO_Port->ODR &= ~P2_DELL_HDD_Pin;
}


int main(void)
{
	HAL_Init();
	Oscillator_IO_Init();
	//SystemClock_Config();
	ClockConfiguration();
	MX_Probe_IO_Init();
	//MX_TIM4_Init();
	//HAL_TIM_Base_Start_IT(&htim4);
	osKernelInit();
	osKernelAddThreads(&Task0,&Task1,&Task2);
	osKernelLaunch(QUANTA);
	while(1)
	{
	}
}