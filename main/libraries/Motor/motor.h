#ifndef MOTOR_H
#define MOTOR_H

#include <stdint.h>
#include "arduino.h"
#include <DueTimer.h>

#define LEFT_MOTOR_FORWARD_PIN_STATE    0
#define RIGHT_MOTOR_FORWARD_PIN_STATE   1
#define LEFT_MOTOR_REVERSE_PIN_STATE    1
#define RIGHT_MOTOR_REVERSE_PIN_STATE   0

#define LEFT_MOTOR_DIR_PIN              22
#define RIGHT_MOTOR_DIR_PIN             23

#define PWM_DUTY_MOVING                 2500
#define TC_DUTY_MOVING                  105000
#define PWM_STOP                        1
#define TC_STOP                         1                     

#define TURN_45_TIME_MS                 3000
#define TURN_90_TIME_MS                 6000
#define TURN_180_TIME_MS                12000

/*
LEFT_MOTOR_EN       2
RIGHT_MOTOR_EN      35
*/

typedef enum {
    STOPPED = 0,
    FORWARD = 1,
    REVERSE = 2,
    TURNING = 3
} State_t;

typedef enum {
    MOTOR_STOPPED = 0,
    MOTOR_FORWARD = 1,
    MOTOR_REVERSE = 2
} MotorState_t;

class Motors {
public:
    State_t state;
    MotorState_t leftMotorState;
    MotorState_t rightMotorState;

    friend void digitalWrite(uint32_t uPin, uint32_t uVal);
    friend void pinMode(uint32_t ulPin, uint32_t ulMode);

    void motorInit();

    void stop();

    State_t getState();

    void moveForwardLeft();

    void moveForwardRight();

    void moveForward();

    void moveReverseLeft();

    void moveReverseRight();

    void moveReverse(); 

    void turn45(bool left);

    void turn90(bool left);

    void turn180(bool left);

    void CancelTurn();
};

void TurnISR();

extern Motors Vehicle;

#endif