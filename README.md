# v3.2.1
## This is a minor update where all tasks are made periodic. This is usefull to understand HYPERPERIOD.

### Theory:
The list of tasks in our board are and their periods (periods are taken for )
    
|Task Name|Period in milliseconds|LED|
|---------|----------------------|---|
|```Task0```| 250|P0_DELL_SMALLGREEN|
|```Task1```|500|P1_DELL_YELLOW|
|```Task2```|500|P2_DELL_HDD|
|```Periodic_Task3```|750|P3_MY_LED|
|```Periodic_Task4```|1000|P4_LED|

### Calculations
Hyper period (H)=LCM(250,500,500,750,1000)
                
                H = 3000 milliseconds

### Implementation
We will set ```QUANTA``` to 1 millisecond, so ```SysTick_Handler``` will be executed every millisecond. Now during this time, our funtion ```osSchedulerRoundRobin``` will decide which task to be executed.

LED animations from the tasks ```Task0,Task1,Task2``` are deleted. I still dont know if the task addition function is relevant or not. I did not modify it yet.

---

# v3.2
Here in second method of thread scheduling, we are going to modify the ```SysTick_Handler```. Till Now we moved to another thread using this ```SysTick_Handler```, but now we shall do it using C function which will give us more control on the context switching portion.

Capability to switch to next thread is implemented in C function instead of Assembly.
Implementation is again considered for following conditions

## Only 1 periodic thread to be executed
For this, the function ```osSchedulerRoundRobinSinglePeriodicTask``` need to be uncommented in osKernel.c file and also in ```SysTick_Handler``` present in osKernel.s file.

## Multiple periodic threads to be executed
Function ``` osSchedulerRoundRobin``` is already uncommented and this is the default function enabled for execution

---

# v3.1
Here in this version and lesson, we are going to talk abount periodic schedulers.
Now there might be many ways to achieve this.

First type of periodic scheduler is described in this version.
Recall from version v2 we have a basic round robin scheduler which consists of ```Task0```, ```Task1``` and ```Task2``` that are called every time by the ```SysTick_Handler```.

Now let us say we need another task which is periodic (say should execute every 100milli seconds). Method explained in  this version is by creating a Timer (```TIM4``` is used in the example) of period 100 milliseconds and executing our task in this interrupt.

Again, HAL Library is used as we are not learning the bare metal programming. Only exception to the application of HAL library is the Clock configuration. Its observed that by using HAL library to configure clock, Systick timer was getting started by HAL, but in our application ```osKernelLaunch``` function.

We will not use this method of periodic scheduling. So lets see for version v3.2

---
# v3:
## This is a pre-release of the actual RTOS

Now that we have understood context switching (using ```SysTick Timer```), we shall now try to move towards more practical example.
So consider a scenario where a reading from Potentiometer is displayed on the LCD screen. and our existing 3 LEDs shall also blink as per the QUANTA time. The 3 LEDs are now called as Probe LEDs.

Since our main intention is to study the RTOS and integration of peripherals around it, we are not going to do bare-metal programming here. So I have used HAL library provided by ST microelectronics in KEIL IDE.

Below are the peripheral details and their pin assignment
### Board : STM32F411CEUTx

### ADC :
Using ADC, Value is displayed on LCD.
	
| ADC CHANNEL | BOARD PIN|
|-------------|----------|
|ADC1_5|PA5|

### LCD 20X4 DISPLAY

| LCD PIN | BOARD PIN |
|---------|-----------|
|DB0|PB0|
|DB1|PB1|
|DB2|PB2|
|DB3|PB3|
|DB4|PB4|
|DB5|PB5|
|DB6|PB6|
|DB7|PB7|
|RS|PA8|
|RW|PA9|
|EN|PA10|

### PROBE LEDs
Probe LEDs are used for animation as well as Oscilloscope verification of our task switching.
|Probe Name| LED PIN DEFINITION|PIN|
|---|---|---|
|Probe 0|P0_DELL_SMALLGREEN_Pin | PA0|
|Probe 1|P1_DELL_YELLOW_Pin |PC13|
|Probe 2|P2_DELL_HDD_Pin |PC14|
|Probe 3|P3_MY_LED_Pin |PC15|

---
# v2:
Till now we gave 1000 quanta size to each thread, but it might be possible that some threads might be completed in less time.
So we can give the remaining time to trigger other threads.
This can be done only by context switching.

To do this context switch before quanta 1000 is to raise the systick interrupt again.
One method is to set the ``PEND`` bit in ``ICSR`` register of ``SysTick``
this will set the PENDING bit to ON for ``SysTick`` and trigger ``SysTick_Handler`` interrrupt and hence context switch will happen.

Now in the context switch, next thread will be fetched immediately.
In this code example, Task0 is yielding, means, it will execute only once in the while loop and raise a PEND bit of SYSTICK timer
That causes the context switch to ``Task1``.

Notice that LED ON for ``Task0`` WILL NOT BE visible as its almost done im microseconds, Oscilloscope only shall do that.

---
# v1:

In this version of ChiliRTOS, we are creating a simple Round Robin Scheduler.

Features of this scheduler is:
* Each task is assigned with a time slice of the duration "QUANTA"
* Task will run till QUANTA and then the next task will be taken up for the same QUANTA duration and so on
* Even if the tasks are finished early, next task is not taken up (means, thread is not yielded)

---
# v0:

This is a basic blinky that runs using simple delays of for loops

* Clock configuration is done using CMSIS library
* GPIO initialization is done using CMSIS library
* Delays are generated using Systick timer configured at 1 millisecond tick.

# Setup information
WE WILL BLINK TWO LEDs as part of tasks

List of LEDs configured in the code are

| LED DESCRIPTION | CHIP PIN |
|-----------------|----------|
|ONBOARD LED (SINK)|PC13|
|DELL_HDD_LED|PA2|
|DELL_YELLOW_LED|PA1|
|DELL_SMALLGREEN_LED|PA0|

---
---
---