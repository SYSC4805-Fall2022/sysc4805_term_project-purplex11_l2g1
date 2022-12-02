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

#ifndef MAX(X, Y)
#define MAX(X, Y)                (X > Y ? X : Y)
#endif

typedef enum {
    NO_WARNING      = 0,
    WARNING         = 1,
    TRIGGER_HANDLER = 2
} WarningState_t;

class UltrasonicSensor {
public:
    WarningState_t leftSensorState;
    WarningState_t rightSensorState;

    friend void digitalWrite(uint32_t uPin, uint32_t uVal);
    friend void pinMode(uint32_t ulPin, uint32_t ulMode);

    void UltrasonicSensorInit();

    WarningState_t pollLeftSensor();
    WarningState_t pollRightSensor();
    WarningState_t pollSensors();
};

extern UltrasonicSensor DistanceSensor;

#endif