#include "ultrasonicSensor.h"

UltrasonicSensor DistanceSensor;

void UltrasonicSensor::UltrasonicSensorInit(){
    /* Set trigger and echo pin modes */
    pinMode(TRIGGER_LEFT_PIN, OUTPUT);
    pinMode(TRIGGER_RIGHT_PIN, OUTPUT);
    pinMode(ECHO_LEFT_PIN, INPUT);
    pinMode(ECHO_RIGHT_PIN, INPUT);

    /* Set default warning level to NO_WARNING */
    leftSensorState = NO_WARNING;
    rightSensorState = NO_WARNING;
}

WarningState_t UltrasonicSensor::pollLeftSensor(){
    /* Create a trigger pulse */
    digitalWrite(TRIGGER_LEFT_PIN, HIGH);
    delayMicroseconds(50);
    digitalWrite(TRIGGER_LEFT_PIN, LOW);

    /* Get the pulse width and have a small timeout as 
     * anytime greater would be ignored anyways */
    float pulseWidth = pulseIn(ECHO_LEFT_PIN, HIGH, 5000);
    float distance = 100 * (343 * ((pulseWidth / (1000*1000)) / 2));

    /* Don't perform any functionality with 0 as it indicates a timeout*/
    if (distance == 0) {
        leftSensorState = NO_WARNING;
    } else if (distance < MIN_TRIGGER_DISTANCE_CM) {
        leftSensorState = TRIGGER_HANDLER;
    } else if (distance < WARNING_DISTANCE_CM) {
        leftSensorState = WARNING;
    } else {
        leftSensorState = NO_WARNING;
    }

    return leftSensorState;
}

WarningState_t UltrasonicSensor::pollRightSensor(){
    /* Create a trigger pulse */
    digitalWrite(TRIGGER_RIGHT_PIN, HIGH);
    delayMicroseconds(50);
    digitalWrite(TRIGGER_RIGHT_PIN, LOW);

    /* Get the pulse width and have a small timeout as 
     * anytime greater would be ignored anyways */
    float pulseWidth = pulseIn(ECHO_RIGHT_PIN, HIGH, 5000);
    float distance = 100 * (343 * ((pulseWidth / (1000*1000)) / 2));

    /* Don't perform any functionality with 0 as it indicates a timeout*/
    if (distance == 0) {
        rightSensorState = NO_WARNING;
    } else if (distance < MIN_TRIGGER_DISTANCE_CM) {
        rightSensorState = TRIGGER_HANDLER;
    } else if (distance < WARNING_DISTANCE_CM) {
        rightSensorState = WARNING;
    } else {
        rightSensorState = NO_WARNING;
    }

    return leftSensorState;
}

WarningState_t UltrasonicSensor::pollSensors(){
    /* Poll both left and right sensors*/
    pollLeftSensor();
    pollRightSensor();

    /* Return the maximum WARNING level of the left and right sensors */
    return MAX(leftSensorState, rightSensorState);
}
