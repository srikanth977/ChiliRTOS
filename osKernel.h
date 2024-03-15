/* VER01 --*/
#ifndef _OS_KERNEL_H
#define _OS_KERNEL_H
#include <stdint.h>
#include "stm32f401xe.h"
 
 //extern void osSchedulerLaunch();	
 //We need a function to Launch the threads
 void osKernelLaunch(uint32_t quanta);
 
 //function to initialize our kernel
 void osKernelInit(void);
 
 //function to add threads
 uint8_t osKernelAddThreads(void(*task0)(void),
														void(*task1)(void),
														void(*task2)(void));
														
void osKernelStackInit(uint32_t ThreadNumber);		

void osThreadYield(void);
											
 #endif