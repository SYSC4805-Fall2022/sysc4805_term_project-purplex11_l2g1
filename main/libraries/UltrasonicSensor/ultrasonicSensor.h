#ifndef ULTRASONIC_SENSOR_H
#define ULTRASONIC_SENSOR_H

#include <stdint.h>
#include "arduino.h"

#define MIN_TRIGGER_DISTANCE_CM  15
#define WARNING_DISTANCE_CM      25

#define TRIGGER_LEFT_PIN         24
#define TRIGGER_RIGHT_PIN        25
#define ECHO_LEFT_PIN            26
#define ECHO_RIGHT_PIN           27

#define MAX(X, Y)                (X > Y ? X : Y)

typedef enum {
    NO_WARNING      = 0,
    WARNING         = 1,
    TRIGGER_HANDLER = 2
} WarningState_t;

class UltrasonicSensor {
public:
    WarningState_t leftSensorState;
    WarningState_t rightSensorState;

    /**************************************************
     * Methods from arduino.h that are used within the 
     * class
    **************************************************/
    friend void digitalWrite(uint32_t uPin, uint32_t uVal);
    friend void pinMode(uint32_t ulPin, uint32_t ulMode);

    /**************************************************
     * void UltrasonicSensorInit()
     * 
     * Method to handle the startup of the ultrasonic sensor.
     * This method will only be called once before any code 
     * is run.
    **************************************************/
    void UltrasonicSensorInit();

    /**************************************************
     * WarningState_t pollLeftSensor()
     * 
     * Method to poll the left ultrasonic sensor. If 
     * obstacle less than MIN_TRIGGER_DISTANCE_CM, return
     * TRIGGER HANDLER. IF obstacle in range MIN_TRIGGER_DISTANCE_CM
     * to WARNING_DISTANCE_CM, return WARNING. Otherwise 
     * return NO WARNING
    **************************************************/
    WarningState_t pollLeftSensor();

    /**************************************************
     * WarningState_t pollRightSensor()
     * 
     * Method to poll the right ultrasonic sensor. If 
     * obstacle less than MIN_TRIGGER_DISTANCE_CM, return
     * TRIGGER HANDLER. IF obstacle in range MIN_TRIGGER_DISTANCE_CM
     * to WARNING_DISTANCE_CM, return WARNING. Otherwise 
     * return NO WARNING
    **************************************************/
    WarningState_t pollRightSensor();

    /**************************************************
     * WarningState_t pollSensors()
     * 
     * Method to poll the both left and right sensors
     * and return the maximum warning/error level of 
     * the two.
    **************************************************/
    WarningState_t pollSensors();
};

extern UltrasonicSensor DistanceSensor;

#endif