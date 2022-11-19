/****************************************************************************************
 * Filename: startUp.cpp
 * Author: Justin Whalley
 * Description: File to contain the start up functionality. Start up is intended to 
 *              include all required modules and perform the initialization. Once BootUp
 *              method called, all functionality is expected to be handled by timer or 
 *              other interrupts
****************************************************************************************/
#include "startUp.h"

void BootUp()
{
    /* Initialize all classes 
     * DO NOT CHANGE ORDER UNLESS REQUIRED */
    Scheduler.SchedulerInit();
    encoders.WheelEncodersInit();
    Motors.MotorInit();
}