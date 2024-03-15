/*
VERSION HISTORY

00 - INITIAL
* Added Blinky with bare metal

*/

#include "main.h"


void ClockConfiguration(void);
void GPIO_Init(void);
void DelayS(uint32_t milliseconds);
uint32_t getTick(void);
void main_DELL_HDD(void);
void main_DELL_SMALLGREEN(void);
uint32_t volatile run;
int main(void)
{
	__disable_irq();
  ClockConfiguration();
  GPIO_Init();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  __enable_irq();
  run = 0u;
  if (run)
  {
	  main_DELL_SMALLGREEN();
  }
  else
  {
	  main_DELL_HDD();
  }
	  while (1)
  {
  }
}


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

	SystemCoreClockUpdate();
	SysTick_Config(SystemCoreClock / (1000U / 1));
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

void DELL_YELLOW_ON(void)
{
	DELL_YELLOW_GPIO_Port->ODR |=DELL_YELLOW_Pin;
}

void DELL_YELLOW_OFF(void)
{
	DELL_YELLOW_GPIO_Port->ODR &= ~DELL_YELLOW_Pin;
}

void MY_LED_ON(void)
{
	MY_LED_GPIO_Port->ODR |=MY_LED_Pin;
}

void MY_LED_OFF(void)
{
	MY_LED_GPIO_Port->ODR &=~MY_LED_Pin;
}

void main_DELL_SMALLGREEN(void)
{
	while(1)
	{
		DELL_SMALLGREEN_ON();
		DelayS(1000U);
		DELL_SMALLGREEN_OFF();
		DelayS(1000U);
	}
}
void DELL_SMALLGREEN_ON()
{
	DELL_SMALLGREEN_GPIO_Port->ODR |=DELL_SMALLGREEN_Pin;
}

void DELL_SMALLGREEN_OFF()
{
	DELL_SMALLGREEN_GPIO_Port->ODR &=~DELL_SMALLGREEN_Pin;
}

void main_DELL_HDD()
{
	while(1)
	{
		DELL_HDD_ON();
		DelayS(1000U);
		DELL_HDD_OFF();
		DelayS(1000U);
	}
}
void DELL_HDD_ON()
{
	DELL_HDD_GPIO_Port->ODR |=DELL_HDD_Pin;
}
void DELL_HDD_OFF()
{
	DELL_HDD_GPIO_Port->ODR &= ~DELL_HDD_Pin;
}


uint32_t getTick()
{
	__disable_irq();
	_tick=tick;
	__enable_irq();
	return _tick;
}

void DelayS(uint32_t milliseconds)
{
	uint32_t tempnum = getTick();
	while((getTick()-tempnum)<milliseconds){}
}
