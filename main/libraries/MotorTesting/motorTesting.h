#ifndef MOTOR_TESTING_H
#define MOTOR_TESTING_H

#include <stdint.h>
#include "arduino.h"
#include <DueTimer.h>
#include "wheelEncoder.h"

#define LEFT_MOTOR_FORWARD_PIN_STATE    0
#define RIGHT_MOTOR_FORWARD_PIN_STATE   1
#define LEFT_MOTOR_REVERSE_PIN_STATE    1
#define RIGHT_MOTOR_REVERSE_PIN_STATE   0

#define LEFT_MOTOR_DIR_PIN              22
#define RIGHT_MOTOR_DIR_PIN             23

#define PWM_DUTY_MOVING                 2500
#define TC_DUTY_MOVING                  105000
#define PWM_DUTY_MOVING_SLOW            1250
#define TC_DUTY_MOVING_SLOW             52500
#define PWM_STOP                        1
#define TC_STOP                         1                     

#define DEFAULT_TURN_45_TIME_MS         1500
#define DEFAULT_TURN_90_TIME_MS         3000
#define DEFAULT_TURN_180_TIME_MS        6000
#define TURN_CHECK_TIME_MS              500

#define TURN_ENCODER_TICKS_45           9
#define TURN_ENCODER_TICKS_90           18
#define TURN_ENCODER_TICKS_180          36

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
    int         turnEncoderTicks;

    friend void digitalWrite(uint32_t uPin, uint32_t uVal);
    friend void pinMode(uint32_t ulPin, uint32_t ulMode);

    /**************************************************
     * void motorInit()
     * 
     * Method to handle the startup of the motors. This 
     * method will only be called once before any code 
     * is run.
    **************************************************/
    void motorInit();

    /**************************************************
     * void stop()
     * 
     * Method to stop the vehicle
    **************************************************/
    void stop();

    /**************************************************
     * State_t getState()
     * 
     * Method to return the current state of the 
     * vehicle
    **************************************************/
    State_t getState();

    /**************************************************
     * void moveForwardLeft()
     * 
     * Method to begin forward movement on left motors
    **************************************************/
    void moveForwardLeft();

    /**************************************************
     * void moveForwardRight()
     * 
     * Method to begin forward movement on right motors
    **************************************************/
    void moveForwardRight();

    /**************************************************
     * void moveForward()
     * 
     * Method to move both left and right motors forward
    **************************************************/
    void moveForward();

    /**************************************************
     * void moveReverseLeft()
     * 
     * Method to begin reverse movement on left motors
    **************************************************/
    void moveReverseLeft();

    /**************************************************
     * void moveReverseRight()
     * 
     * Method to begin reverse movement on right motors
    **************************************************/
    void moveReverseRight();

    /**************************************************
     * void moveReverse()
     * 
     * Method to move reverse on both left and right motors
    **************************************************/
    void moveReverse(); 

    /**************************************************
     * void slowDownLeft()
     * 
     * Method to slow down left motor speed by 50%
    **************************************************/
    void slowDownLeft();

    /**************************************************
     * void slowDownRight()
     * 
     * Method to slow down right motor speed by 50%
    **************************************************/
    void slowDownRight();

    /**************************************************
     * void slowDown()
     * 
     * Method to slow down both left and right motors
     * speeds by 50%
    **************************************************/
    void slowDown();

    /**************************************************
     * void turn45(bool left))
     * 
     * Method to begin turning 45 degrees to the left
     * if parameter is true otherwise turn 45 degrees to
     * the right
    **************************************************/
    void turn45(bool left);

    /**************************************************
     * void turn90(bool left))
     * 
     * Method to begin turning 90 degrees to the left
     * if parameter is true otherwise turn 90 degrees to
     * the right
    **************************************************/
    void turn90(bool left);

    /**************************************************
     * void turn180(bool left))
     * 
     * Method to begin turning 180 degrees to the left
     * if parameter is true otherwise turn 180 degrees to
     * the right
    **************************************************/
    void turn180(bool left);

    /**************************************************
     * void CancelTurn()
     * 
     * Method to cancel the turning of the vehicle
    **************************************************/
    void CancelTurn();
};

/**************************************************
 * void TurnISR()
 * 
 * Method to check whether or not the turn is complete
**************************************************/
void TurnISR();

extern Motors Vehicle;

#endif