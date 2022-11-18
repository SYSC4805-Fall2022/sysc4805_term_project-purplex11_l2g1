


#include "motor.h"

Motor Motors;

void Motor::_SetDir(bool_t forward){
    /* Set motor direction pins to match parameter*/
    if (forward)
    {
        digitalWrite(MOTOR_1_2_DIR_PIN, FORWARD_MOTOR_PIN_STATE);
        digitalWrite(MOTOR_3_4_DIR_PIN, FORWARD_MOTOR_PIN_STATE);
    }
    else
    {
        digitalWrite(MOTOR_1_2_DIR_PIN, REVERSE_MOTOR_PIN_STATE);
        digitalWrite(MOTOR_3_4_DIR_PIN, REVERSE_MOTOR_PIN_STATE);
    }
}

void Motor::Stop(){
    /* Stop all motors immediately */
    TC0->TC_CHANNEL[0].TC_RA = (42000000 / MOTOR_FREQUENCY) * 0.01; 
    PWM->PWM_CH_NUM[0].PWM_CDTY = ( 1000000 / MOTOR_FREQUENCY)* 0.01; 
    Motors.isTurning = FALSE;
}

void Motor::MotorInit()
{
    Motors.isTurning = FALSE;
    Motors.turnDegrees = DEGREE_0;
    Motors.turnRight = FALSE;

    Scheduler.AddHandler(MotorISR, MOTOR_ISR_TIME_MS);
}

void Motor::MoveForwardLeft(){
    _SetDir(TRUE);
     TC0->TC_CHANNEL[0].TC_RA = (42000000 / MOTOR_FREQUENCY) * 0.25; 
}

void Motor::MoveForwardRight(){
    _SetDir(TRUE);
    PWM->PWM_CH_NUM[0].PWM_CDTY = ( 1000000 / MOTOR_FREQUENCY)*0.25;
}

void Motor::MoveReverseLeft(){
    _SetDir(FALSE);
     TC0->TC_CHANNEL[0].TC_RA = (42000000 / MOTOR_FREQUENCY) * 0.25; 
}

void Motor::MoveReverseRight(){
    _SetDir(FALSE);
    PWM->PWM_CH_NUM[0].PWM_CDTY = ( 1000000 / MOTOR_FREQUENCY)*0.25;
}

void Motor::MoveForward()
{
    MoveForwardLeft();
    MoveForwardRight();
}

void Motor::MoveReverse()
{
    MoveReverseLeft();
    MoveReverseRight();
}

int8_t Motor::GetSpeedRight()
{
    /* Return the greatest speed from left or right motors */
    if (ABS(GetSpeedLeft()) > ABS(GetSpeedRight()))
    {
        return GetSpeedLeft();
    }
    else
    {
        return GetSpeedRight();
    }
}

int8_t Motor::GetSpeedLeft()
{
    /* Return the encoder measured speed for left wheels */
    return encoders.GetMeasuredSpeedLeft();
}

int8_t Motor::GetSpeed()
{
    /* Return the encoder measured speed for right wheels */
    return encoders.GetMeasuredSpeedRight();
}

void Motor::Turn(TurnAngle_t degrees, bool_t right){
    Stop();

    if (degrees != DEGREE_0)
    {
        /* Initialize variables to begin turning */
        turnRight = right;
        turnDegrees = degrees;
        isTurning = TRUE;
    }
    else
    {
        /* Stop turning if DEGREE_0 is inputted */
        isTurning = FALSE;
    }
}

void MotorISR()
{
    /* Check if the motor is turning */
    if (Motors.isTurning)
    {
        /* If stopped, begin the turning procedure */
        if (Motors.GetSpeed() == 0)
        {
            /* Set the left or right motors to move depending on turn direction */
            if (Motors.turnRight)
            {
                Motors.MoveForwardLeft();
            }
            else
            {
                Motors.MoveForwardRight();
            }
            encoders.EncoderReset();
        }
        else
        {
            /* Check if turn complete. If complete, stop vehicle and clear isTurning flag */
            if (Motors.turnRight)
            {
                if (encoders.GetEncoderCountLeft() > Motors.turnDegrees)
                {
                    Motors.isTurning = FALSE;
                    Motors.Stop();
                }
            }
            else
            {
                if (encoders.GetEncoderCountRight() > Motors.turnDegrees)
                {
                    Motors.isTurning = FALSE;
                    Motors.Stop();
                }
            }
        }
    }
}