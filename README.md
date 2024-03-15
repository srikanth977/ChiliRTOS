v02:
Till now we gave 1000 quanta size to each thread, but it might be possible that some threads might be completed in less time.
So we can give the remaining time to trigger other threads.
This can be done only by context switching.
to do this context switch before quanta 1000 is to raise the systick interrupt again.
One method is to set the PEND bit in ICSR register of systick
this will set the PENDING bit to ON for systick and trigger systick interrrupt and hence context switch will happen.
Now in the context switch, next thread will be fetched immediately.
In this code example, Task0 is yielding, means, it will execute only once in the while loop and raise a PEND bit of SYSTICK timer
That causes the context switch to Task1

Notice that LED ON for task0 WILL NOT BE visible as its almost done im microseconds, Oscilloscope only shall do that.
==========================================================================

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
