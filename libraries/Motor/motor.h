/****************************************************************************************
 * Filename: motor.h
 * Author: Justin Whalley
 * Description: Header file to contain structs and function declarations for motor module
****************************************************************************************/

#ifndef MOTOR_H
#define MOTOR_H

#include "common.h"
#include "wheelEncoder.h"


/****************************************************************************************
 * Definitions
****************************************************************************************/

#define MOTOR_ISR_TIME_MS       50
#define FORWARD_MOTOR_PIN_STATE 1
#define REVERSE_MOTOR_PIN_STATE 0

/****************************************************************************************
 * Enums
****************************************************************************************/

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

class Motor
{
public:
    bool_t          isTurning;
    TurnAngle_t     turnDegrees;
    bool_t          turnRight;

    friend void digitalWrite(uint32_t ulPin, uint32_t ulVal);

    /****************************************************************************************
     * void _SetDir()
     * 
     * Function to change the direction pins to match parameter.
    ****************************************************************************************/
    void _SetDir(bool_t forward);

    /****************************************************************************************
     * void HardStop()
     * 
     * Function to immediately stop the vehicle.
    ****************************************************************************************/
    void Stop();

    /****************************************************************************************
     * void MoveForwardLeft()
     * 
     * Sets the left wheels to move forward.
    ****************************************************************************************/
    void MoveForwardLeft();

    /****************************************************************************************
     * void MoveForwardRight()
     * 
     * Sets the Right wheels to move forward.
    ****************************************************************************************/
    void MoveForwardRight();

    /****************************************************************************************
     * void MoveReverseLeft()
     * 
     * Sets the left wheels to reverse.
    ****************************************************************************************/
    void MoveReverseLeft();

    /****************************************************************************************
     * void MoveReverseRight()
     * 
     * Sets the Right wheels to reverse.
    ****************************************************************************************/
    void MoveReverseRight();

    /****************************************************************************************
     * void MoveForward()
     * 
     * Sets both left and right to move forward.
    ****************************************************************************************/
    void MoveForward();

    /****************************************************************************************
     * void MoveReverse()
     * 
     * Sets both left and right to reverse.
    ****************************************************************************************/
    void MoveReverse();

    /****************************************************************************************
     * void GetSpeedRight()
     * 
     * Returns the current speed of the right wheels. This function wraps wheel encoders 
     * GetMeasuredSpeedRight function.
    ****************************************************************************************/
    int8_t GetSpeedRight();

    /****************************************************************************************
     * void GetSpeedLeft()
     * 
     * Returns the current speed of the left wheels. This function wraps wheel encoders 
     * GetMeasuredSpeedLeft function.
    ****************************************************************************************/
    int8_t GetSpeedLeft();

    /****************************************************************************************
     * void GetSpeed()
     * 
     * Returns the highest speed of GetSpeedRight and GetSpeedLeft.
    ****************************************************************************************/
    int8_t GetSpeed();

    /****************************************************************************************
     * void GetSpeed()
     * 
     * Stops vehicle and turns according to parameters. The degrees states how many degrees
     * to turn and the boolean right decides whether to turn right or left
    ****************************************************************************************/
    void Turn(TurnAngle_t degrees, bool_t right);

    /****************************************************************************************
     * void MotorInit()
     * 
     * Function to encapsulate startup sequence for Motor library
    ****************************************************************************************/
    void MotorInit();
};

void MotorISR();
extern Motor Motors;

#endif