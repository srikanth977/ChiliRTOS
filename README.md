v01:
In this version of ChiliRTOS, we are creating a simple Round Robin Scheduler
Features of this scheduler is:
* Each task is assigned with a time slice of the duration "QUANTA"
* Task will run till QUANTA and then the next task will be taken up for the same QUANTA duration and so on
* Even if the tasks are finished early, next task is not taken up (means, thread is not yielded)
============================================================================================================

This is a basic blinky that runs using simple delays of for loops

* Clock configuration is done using CMSIS library
* GPIO initialization is done using CMSIS library
* Delays are generated using Systick timer configured at 1 millisecond tick.

*******SETUP INFORMATION********************
WE WILL BLINK TWO LEDs

LED DESCRIPTION			CHIP PIN
ONBOARD LED (SINK)  	PC13
DELL_HDD_LED			PA2
DELL_YELLOW_LED			PA1
DELL_SMALLGREEN_LED		PA0
