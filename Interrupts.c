#include "main.h"

volatile uint32_t tick;
volatile uint32_t _tick;

void SysTick_Handler(void)
{
  	++tick;
}