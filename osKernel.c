/* VER01 --*/
//We need a scheduler
//tcb (thread control block)
//stack to hold contents of our threads

#include "osKernel.h"

//thread control block
#define NUM_OF_THREADS	3
#define STACK_SIZE			100 //bytes

void  osSchedulerLaunch(void);

struct tcb{
	uint32_t *stackPt;		//pointer to the address of current thread
	struct tcb *nextPt;	//pointer to the address of next thread
};

//declare tcb WITh our number of threads
typedef struct tcb tcbType;
tcbType tcbs[NUM_OF_THREADS];
tcbType *currentPt;

//Assigning STACK ARRRAY WITH NUMBER OF ARRAY AND THEIR RESPECTIVE SIZES
uint32_t TCB_STACK[NUM_OF_THREADS][STACK_SIZE];


void osKernelInit()
{
 __enable_irq();
}

void osKernelLaunch(uint32_t quanta)
{
	SystemCoreClockUpdate();
	SysTick_Config(quanta * (SystemCoreClock / 1000U));		//1ms * QUANTA
	NVIC_SetPriority (SysTick_IRQn, 7U); /* set Priority for Systick Interrupt */
	
	osSchedulerLaunch();
}

 uint8_t osKernelAddThreads(void(*task0)(void),void(*task1)(void),void(*task2)(void))
{
	//this is where we set the arrangement of threads
	//we decide here which thread should run after a thread.
	
	//this is a critical section, so disable of interrupts is performed
	
	__disable_irq();
	//setting next stack pointer of thread0 to thread1
	tcbs[0].nextPt=&tcbs[1];
	tcbs[1].nextPt=&tcbs[2];
	tcbs[2].nextPt=&tcbs[0];
	 
	//Now we will initialize the stack
	osKernelStackInit(0);
	TCB_STACK[0][STACK_SIZE-2] = (uint32_t)(task0);	//PC of task0

	osKernelStackInit(1);
	TCB_STACK[1][STACK_SIZE-2] = (uint32_t)(task1);	//PC of task1
	
	osKernelStackInit(2);
	TCB_STACK[2][STACK_SIZE-2] = (uint32_t)(task2);	//PC of task2
	
	currentPt=&tcbs[0];	//initially set task0
	__enable_irq();
	return 1;
}

//Now we are going to initialize the stack first.
//What happens in the initialization? we will see now
void osKernelStackInit(uint32_t ThreadNumber)
{
	//Now we have to ensure that the Stackpointer is pointing to the top of the stack
	//make sure we point the stackpointer of each thread to the top of the stack frame
	// Stack will be as below
	//	[																															  xpsr	PC	LR	R12	R3	R2	R1	R0]
	//	[0	3	7	11	15	23	27	31	35	39	43	47	51	55	59	63	67	71	75	79	83	87	91	95	99] this is top of stack
	//	[ 																															R0	R1	R2	R3	R12	LR	PC	xpsr
	tcbs[ThreadNumber].stackPt = &TCB_STACK[ThreadNumber][STACK_SIZE-16];
	
	//Now storing Value of XPSR as thread mode in the stackpointer frame
	TCB_STACK[ThreadNumber][STACK_SIZE-1] = 0x01000000;
}

/* VER2.0 ---*/
//Till now we gave 1000 quanta size to each thread, but it might be possible that some threads might be completed in less time.
//So we can give the remaining time to trigger other threads.
//This can be done only by context switching.
//to do this context switch before quanta 1000 is to raise the systick interrupt again.
//One method is to set the PEND bit in ICSR register of systick
//this will set the PENDING bit to ON for systick and trigger systick interrrupt and hence context switch will happen.
void osThreadYield()
{
 SCB->ICSR |= (1U<<SCB_ICSR_PENDSTSET_Pos);
}


uint32_t periodic_tick;

void Periodic_Task3(void);
void Periodic_Task4(void);
//void osSchedulerRoundRobinSinglePeriodicTask(void)
//{
//	//Below algorithm is OK IF AND ONLY IF 1 periodic task is present.
//	//But that wont be the case in real world.
//	//So for multiple Periodic task, we need to define HYPERPERIOD and accordingly we need to schedule tasks.
//	if (++periodic_tick==PERIOD)
//	{
//		(*Periodic_Task3)();
//		periodic_tick=0;
//	}
//	currentPt=currentPt->nextPt;
//}

/*
Let us say we have two periodic tasks running at 1000ms and 500ms
So to Calculate HyperPeriod H = LCM(500,1000) = 1000
For study purpose, we shall not write algorithm to calculate HyperPeriod.
*/
void osScheduerRoundRobin()
{
	//OUR QUANTA SIZE IS 100, means the variable periodic_tick will be incremented only after 100 msec.
	//So at every 100msec, variable periodic_tick will be incremented
	++periodic_tick;
	periodic_tick %=10;	//mod operation
	if(periodic_tick%5==0)
	{
		(*Periodic_Task4)();
	}
	if(periodic_tick%10==0)
	{
		(*Periodic_Task3)();
	}
	
	if (periodic_tick>10)
	{
		periodic_tick=0;
	}
	currentPt=currentPt->nextPt;
}


