/****************************************************************************************
 * Filename: motor.h
 * Author: Justin Whalley
 * Description: Module containing all motor commands and settings
****************************************************************************************/

#ifndef MOTOR_H
#define MOTOR_H

#include "common.h"

/****************************************************************************************
 * Definitions
****************************************************************************************/

#define MIN_ACCELERATION                    -5
#define MAX_ACCELERATION                    5
#define MIN_VELOCITY                        0
#define MAX_VELOCITY                        25
#define MIN_TURN_ANGLE                      -180
#define MAX_TURN_ANGLE                      180
#define ACCEPTABLE_SPEED_RANGE              3
#define ACCELERATION_TIMER_MS               50
#define CORRECTION_ACCELERATION             1

#define FORWARD_MOTOR_PIN_STATE             1
#define REVERSE_MOTOR_PIN_STATE             0

#define GET_MOTOR_STATE()                   (Motor.state)
#define SET_MOTOR_STATE(X)                  (Motor.state = X)
#define GET_TARGET_MOTOR_STATE()            (Motor.targetState)
#define SET_TARGET_MOTOR_STATE(X)           (Motor.targetState = X)

#define SET_MOTOR_VELOCITY(X)               (Motor.velocity = X)
#define GET_MOTOR_VELOCITY()                (Motor.velocity)
#define SET_MOTOR_TARGET_VELOCITY(X)        (Motor.targetVelocity = X)
#define GET_MOTOR_TARGET_VELOCITY()         (Motor.targetVelocity)

#define GET_MOTOR_ACCELERATION()            (Motor.acceleration)
#define SET_MOTOR_ACCELERATION(X)           (Motor.acceleration = X)

#define GET_LEFT_MOTOR_SPEED()              (MotorEncoder.leftMotorSpeed)
#define GET_RIGHT_MOTOR_SPEED()             (MotorEncoder.rightMotorSpeed)

#define GET_INTERNAL_MOTOR_STATE()          (InternalMotor.MotorState)
#define SET_INTERNAL_MOTOR_STATE(X)         (InternalMotor.MotorState = X)

#define GET_INCREMENT_FROM_ACCELERATION(X)  (((X - 0.9462) / 0.2632) / (1 / (ACCELERATION_TIMER_MS / 1000)) * 1000) // keep 3 decimal points
/*
    Duty cycle (%) -> speed (cm/s)

    0   -> 0
    20  -> 4.6
    40  -> 14.08
    60  -> 18.65
    80  -> 21.55
    100 -> 25.77

    speed(cm/s) = 0.2632*(duty_cycle(%)) + 0.9462    <- R^2 = 0.9673

    Increment = ((acceleration (cm/s^2) - 0.9462) / 0.2632) / (1 / (ACCELERATION_TIMER_MS*e-3))
*/

/****************************************************************************************
 * Enums
****************************************************************************************/

typedef enum {
    STOPPED      = 0,
    FORWARD      = 1,
    REVERSE      = 2,
    ACCELERATING = 3,
    DECELERATING = 4
} MotorState_t;

typedef enum {
    DEGREE_0   = 0,
    DEGREE_5   = 1,
    DEGREE_25  = 5,
    DEGREE_45  = 9,
    DEGREE_90  = 18,
    DEGREE_180 = 36,
} TurnAngle_t;

/****************************************************************************************
 * Structs
****************************************************************************************/

typedef struct 
{
    MotorState_t MotorState;
} InternalMotor_t;


typedef struct 
{
    MotorState_t state;
    MotorState_t targetState;
    uint8_t      velocity;
    uint8_t      targetVelocity;
    int8_t       acceleration;
} Motor_t;

typedef struct 
{
    uint8_t  targetVelocity;
    bool_t   isTurning;
    int8_t   acceleration;
    uint16_t dutyCycleIncrement;
    uint8_t  turnTicks;
    uint8_t  turnTickCounter;
    bool_t   turnRight;
    uint8_t  leftTickCounter;
    uint8_t  rightTickCounter;
} MotorCtl_t;

typedef struct
{
    uint8_t     leftMotorSpeed;
    uint8_t     rightMotorSpeed;
} MotorEncoder_t;

extern Motor_t Motor;
extern MotorCtl_t MotorCtl;
extern MotorEncoder_t MotorEncoder;
extern InternalMotor_t InternalMotor;

/****************************************************************************************
 * Methods
****************************************************************************************/

/****************************************************************************************
 * MotorInit()
 * 
 * Function to run during the boot up process which will set all default variables.
****************************************************************************************/
void MotorInit();

/****************************************************************************************
 * SetMotorSpeed(uint8_t velocity)
 * 
 * Function to set the current motor speed, if velocity parameter is greater than the 
 * maximum limit, speed will be set to the maximum. If the velocity parameter is less than
 * the minimum limit, the velocity will be set to the minimum.If current speed is greater 
 * than set motor speed, the vehicle will begin to deccelerate until the set motor speed 
 * with a decceleration of abs(Motor.Acceleration)
****************************************************************************************/
void SetMotorSpeed(uint8_t velocity);

/****************************************************************************************
 * GetMotorSpeed()
 * 
 * Function to get the current motor speed. 
****************************************************************************************/
uint8_t GetMotorSpeed();

/****************************************************************************************
 * SetMotorState(MotorState_t state)
 * 
 * Function to set the current motor state. If state parameter is not of type MotorState_t
 * the state will remain unchanged. If motor state is set to STOPPED, the vehicle will 
 * begin decclerating with an acceleration of MIN_ACCELERATION. If the motor is in reverse
 * and state is FORWARD, the vehicle will deccelerate with an acceleration of 
 * -abs(Motor.acceleration) to a speed of 0 and then accelerate with an acceleration of 
 * abs(Motor.acceleration) until Motor.velocity is reached.
****************************************************************************************/
void SetMotorState(MotorState_t state);

/****************************************************************************************
 * GetMotorState()
 * 
 * Function to get the current motor state.
****************************************************************************************/
MotorState_t GetMotorState();

/****************************************************************************************
 * SetMotorAcceleration(int8_t acceleration)
 * 
 * Function to set the current motor acceleration, if acceleration parameter is greater 
 * than the maximum limit, acceleration will be set to the maximum. If the acceleration 
 * parameter is less than the minimum limit, the acceleration will be set to the minimum.
 * Once set, the vehicle will begin accelerating up to the maximum velocity if acceleration
 * is positive or decelerating to minimum velocity if negative. 
****************************************************************************************/
void SetMotorAcceleration(int8_t acceleration);

/****************************************************************************************
 * GetMotorAcceleration()
 * 
 * Function to get the current motor acceleration.
****************************************************************************************/
int8_t GetMotorAcceleration();

/****************************************************************************************
 * Turn(TurnAngle_t turnAngle)
 * 
 * Function to turn the vehicle turnAngle degrees.
****************************************************************************************/
void Turn(TurnAngle_t turnAngle, bool_t turnRight);

/****************************************************************************************
 * MotorISR()
 * 
 * Function to handle all timed instructions.
****************************************************************************************/
void MotorISR();
#endif