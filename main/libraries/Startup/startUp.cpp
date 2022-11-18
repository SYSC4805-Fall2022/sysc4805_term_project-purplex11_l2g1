#include "startUp.h"

void BootUp()
{
    /* Initialize all classes 
     * DO NOT CHANGE ORDER UNLESS REQUIRED */
    Scheduler.SchedulerInit();
    encoders.WheelEncodersInit();
    Motors.MotorInit();
}