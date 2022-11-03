/****************************************************************************************
 * Filename: motor.c
 * Author: Justin Whalley
 * Description: Module to control the vehicle motors
****************************************************************************************/

/****************************************************************************************
 * Include Declarations
****************************************************************************************/
#include "motor.h"

/****************************************************************************************
 * Veriable Declarations
****************************************************************************************/
Motor_t Motor;
MotorCtl_t MotorCtl;
MotorEncoder_t MotorEncoder;
InternalMotor_t InternalMotor;

DueTimer_t *Timer0;
DueTimer_t *Timer1;
DueTimer_t *Timer2;
DueTimer_t *Timer3;
DueTimer_t *Timer4;
DueTimer_t *Timer5;
DueTimer_t *Timer6;
DueTimer_t *Timer7;
DueTimer_t *Timer8;

/****************************************************************************************
 * Methods 
****************************************************************************************/

void MotorInit() 
{
    Timer0 = NewTimer(0);
    Timer1 = NewTimer(1);
    Timer2 = NewTimer(2);
    Timer3 = NewTimer(3);
    Timer4 = NewTimer(4);
    Timer5 = NewTimer(5);
    Timer6 = NewTimer(6);
    Timer7 = NewTimer(7);
    Timer8 = NewTimer(8);

    /* Initialize Motor structs to 0*/
    memset(&Motor, 0, sizeof(Motor_t));
    memset(&MotorCtl, 0, sizeof(MotorCtl_t));
    memset(&MotorEncoder, 0, sizeof(MotorEncoder_t));
    memset(&InternalMotor, 0, sizeof(InternalMotor_t));

    /* Set default values */
    SET_MOTOR_STATE(STOPPED);
    SET_MOTOR_ACCELERATION(MAX_ACCELERATION);
    SET_INTERNAL_MOTOR_STATE(STOPPED);

    TimerAttachInterrupt(Timer1, MotorISR());
    TimerStart(Timer1, 75000); // 75ms interrupt

    PMC->PMC_PCER0 |= PMC_PCER0_PID27;//TC0 power ON - Timer Counter 0 channel 0
    PIOB->PIO_PDR |= PIO_PDR_P25; // The pin is no more driven by GPIO
    PIOB->PIO_ABSR |= PIO_PB25B_TIOA0;// B Assign B25 to alternative periph_B (TIOA0):
    TC0->TC_CHANNEL[0].TC_CMR = TC_CMR_TCCLKS_TIMER_CLOCK1 //MCK/2 = 42 MHz,
        | TC_CMR_WAVE //Waveform mode
        | TC_CMR_WAVSEL_UP_RC //Count UP mode till RC
        | TC_CMR_ACPA_CLEAR //Clear TIOA0 on RA compare match
        | TC_CMR_ACPC_SET; // Set TIOA0 on RC compare match
    TC0->TC_CHANNEL[0].TC_RC = ( 1000000 / MOTOR_FREQUENCY) -1; //Set the frequency to 100Hz (Period 10 ms)
    TC0->TC_CHANNEL[0].TC_RA = 0; //Set the duty cycle to 0
    TC0->TC_CHANNEL[0].TC_CCR = TC_CCR_SWTRG //Software trigger TC0 channel 0 counter
        | TC_CCR_CLKEN;//and enable
    TC0->TC_CHANNEL[1].TC_CMR = TC_CMR_TCCLKS_TIMER_CLOCK1 //MCK/2 = 42 MHz,
        | TC_CMR_WAVE //Waveform mode
        | TC_CMR_WAVSEL_UP_RC //Count UP mode till RC
        | TC_CMR_ACPA_CLEAR //Clear TIOA0 on RA compare match
        | TC_CMR_ACPC_SET; // Set TIOA0 on RC compare match
    TC0->TC_CHANNEL[1].TC_RC = ( 1000000 / MOTOR_FREQUENCY) -1; //Set the frequency to 100Hz (Period 10 ms)
    TC0->TC_CHANNEL[1].TC_RA = 0; //Set the duty cycle to 0
    TC0->TC_CHANNEL[1].TC_CCR = TC_CCR_SWTRG //Software trigger TC0 channel 0 counter
        | TC_CCR_CLKEN;//and enable
}


void SetMotorSpeed(uint8_t velocity) 
{
    /* If set velocity is greater than limit, set the target speed
     * to the limit */
    velocity = MIN(velocity, GET_MOTOR_VELOCITY_LIMIT());
    
    /* Set the target velocity */
    SET_MOTOR_VELOCITY(velocity);

    /* If current speed is not in range, accelerate or decelerate to 
     * target motor speed */
    if (!IN_DELTA(GetMotorSpeed(), velocity, ACCEPTABLE_SPEED_RANGE)) 
    {
        if (GetMotorSpeed() > velocity)
        {
            SetMotorAcceleration(-ABS(GET_MOTOR_ACCELERATION()));
        }
        else
        {
            SetMotorAcceleration(ABS(GET_MOTOR_ACCELERATION()));
        }
    }
}


uint8_t GetMotorSpeed()
{
    /* Return the largest motor speed of left and right side */
    return  MAX(GET_LEFT_MOTOR_SPEED(), GET_RIGHT_MOTOR_SPEED());
}

void SetMotorState(MotorState_t state)
{
    /* If state is being changed, decelerate to 0 and then accelerate 
     * to target */
    if (state != GetMotorState())
    {
        SET_TARGET_MOTOR_STATE(state);
        SET_MOTOR_TARGET_VELOCITY(GET_MOTOR_VELOCITY());
        SET_MOTOR_VELOCITY(0);
        SetMotorAcceleration(-ABS(GET_MOTOR_ACCELERATION()));
    }
}

void _SetMotorState(MotorState_t state)
{
    /* Set motor driver pins match state */
    if (state == STOPPED)
    {
        digitalWrite(MOTOR_1_2_CTRL_PIN, LOW);
        digitalWrite(MOTOR_3_4_CTRL_PIN, LOW);
        SET_MOTOR_STATE(STOPPED);
    } 
    else if (state == FORWARD)
    {
        if (GetMotorState() != FORWARD)
        {
            digitalWrite(MOTOR_1_2_DIR_PIN, FORWARD_MOTOR_PIN_STATE);
            digitalWrite(MOTOR_3_4_DIR_PIN, FORWARD_MOTOR_PIN_STATE);
        }
        SET_MOTOR_STATE(FORWARD);
    } 
    else if (state == REVERSE)
    {
        if (GetMotorState() != REVERSE)
        {
            digitalWrite(MOTOR_1_2_DIR_PIN, REVERSE_MOTOR_PIN_STATE);
            digitalWrite(MOTOR_3_4_DIR_PIN, REVERSE_MOTOR_PIN_STATE);
        }
        SET_MOTOR_STATE(REVERSE);
    }
}


MotorState_t GetMotorState()
{
    return GET_MOTOR_STATE();
}


void SetMotorAcceleration(int8_t acceleration)
{
    /* Set new acceleration and if the current velocity is not acceptable, accelerate or 
     * decelerate to target velocity */
    SET_MOTOR_ACCELERATION(acceleration);
    if (!IN_DELTA(GetMotorSpeed(), GET_MOTOR_VELOCITY(), ACCEPTABLE_SPEED_RANGE))
    {
        Accelerate();    
    }
}


int8_t GetMotorAcceleration()
{
    return GET_MOTOR_ACCELERATION();
}

void Turn(TurnAngle_t turnAngle, bool_t turnRight)
{
    /* Set turn variables and stop vehicle before turning */
    MotorCtl.turnTicks = turnAngle;
    MotorCtl.isTurning = TRUE;
    MotorCtl.turnRight = turnRight;
    SetMotorState(STOPPED);
}

void Accelerate()
{
    /* Get the duty cycle increment per 50ms step and start interrupt */
    MotorCtl.dutyCycleIncrement = GET_INCREMENT_FROM_ACCELERATION(ABS(GET_MOTOR_ACCELERATION()));
    TimerAttachIntterupt(Timer2, AccelerateISR());
    TimerStart(Timer2, 50000); // 50ms interrupt 
}

void AccelerateISR()
{
    if (IN_DELTA(GetMotorSpeed(), GET_MOTOR_VELOCITY(), ACCEPTABLE_SPEED_RANGE))
    {
        /* if target is 0, set the current state to stopped */
        if (IN_DELTA(GET_MOTOR_VELOCITY(), 0 , 1))
        {
            _SetMotorState(STOPPED);
            TC0->TC_CHANNEL[0].TC_RA = 0;
            TC0->TC_CHANNEL[1].TC_RA = 0;
        }

        /* If range skipped over the target, backoff step by 1 */
        if (GetMotorSpeed() > GET_MOTOR_VELOCITY())
        {
            TC0->TC_CHANNEL[0].TC_RA = (1000000 / MOTOR_FREQUENCY) * ((TC0->TC_CHANNEL[0].TC_RA / (1000000 / MOTOR_FREQUENCY)) - (MotorCtl.dutyCycleIncrement / 1000));
            TC0->TC_CHANNEL[1].TC_RA = (1000000 / MOTOR_FREQUENCY) * ((TC0->TC_CHANNEL[1].TC_RA / (1000000 / MOTOR_FREQUENCY)) - (MotorCtl.dutyCycleIncrement / 1000));
        }

        /* if changing directions, set the new target velocity and change directions*/
        if (GET_TARGET_MOTOR_STATE() != GET_MOTOR_STATE())
        {
            SET_MOTOR_VELOCITY(GET_MOTOR_TARGET_VELOCITY());
            _SetMotorState(GET_TARGET_MOTOR_STATE());
        }
        else
        {
            /* Done accelerating or decelerating, stop interrupt*/
            TimerStop(Timer2);
        }
    }
    else if (GetMotorSpeed() > GET_MOTOR_VELOCITY())
    {
        /* Decelerate by 1 step */
        TC0->TC_CHANNEL[0].TC_RA = (1000000 / MOTOR_FREQUENCY) * ((TC0->TC_CHANNEL[0].TC_RA / (1000000 / MOTOR_FREQUENCY)) - (MotorCtl.dutyCycleIncrement / 1000));
        TC0->TC_CHANNEL[1].TC_RA = (1000000 / MOTOR_FREQUENCY) * ((TC0->TC_CHANNEL[1].TC_RA / (1000000 / MOTOR_FREQUENCY)) - (MotorCtl.dutyCycleIncrement / 1000));
    }
    else
    {
        /* Accelerate by 1 step */
        TC0->TC_CHANNEL[0].TC_RA = (1000000 / MOTOR_FREQUENCY) * ((TC0->TC_CHANNEL[0].TC_RA / (1000000 / MOTOR_FREQUENCY)) + (MotorCtl.dutyCycleIncrement / 1000));
        TC0->TC_CHANNEL[1].TC_RA = (1000000 / MOTOR_FREQUENCY) * ((TC0->TC_CHANNEL[1].TC_RA / (1000000 / MOTOR_FREQUENCY)) + (MotorCtl.dutyCycleIncrement / 1000));
    }
    
}

void MotorTimerISR() {
    if (GetMotorState() != STOPPED || MotorCtl.isTurning)
    {
        if (GetMotorSpeed() == 0 && !MotorCtl.isTurning)
        {
            SetMotorState(STOPPED);
        } 
        else
        {
            if (MotorCtl.isTurning)
            {
                if(MotorCtl.turnTicks > 0)
                {
                    /* Start motor to turn */
                    if (GetMotorSpeed() == 0)
                    {
                        MotorCtl.turnTickCounter = 0;
                        _SetMotorState(FORWARD);
                        if (MotorCtl.turnRight)
                        {
                            TC0->TC_CHANNEL[0].TC_RA = (1000000 / MOTOR_FREQUENCY) * 0.2;
                        }
                        else
                        {
                            TC0->TC_CHANNEL[1].TC_RA = (1000000 / MOTOR_FREQUENCY) * 0.2;
                        }
                    }
                    /* Turn until number of ticks met for a turn */
                    if (MotorCtl.turnTickCounter >= MotorCtl.turnTicks)
                    {
                        /* When turn complete, stop vehicle */
                        MotorCtl.turnTicks = 0;
                        MotorCtl.turnTickCounter = 0;
                        MotorCtl.isTurning = FALSE;
                        SetMotorState(STOPPED);
                    }
                }
                else
                {
                    SetMotorState(STOPPED);
                }
            }
            else
            {
                /* Confirm vehicle is moving straight */
                if (!IN_DELTA(MotorCtl.leftTickCounter, MotorCtl.rightTickCounter, 2))
                {
                    if (GetMotorSpeed() < MAX_VELOCITY)
                    {
                        if (MotorCtl.leftTickCounter > MotorCtl.rightTickCounter)
                        {
                            /* Increment right speed */
                            TC0->TC_CHANNEL[1].TC_RA = (1000000 / MOTOR_FREQUENCY) * ((TC0->TC_CHANNEL[1].TC_RA / (1000000 / MOTOR_FREQUENCY)) + 
                                                            (GET_INCREMENT_FROM_ACCELERATION(CORRECTION_ACCELERATION) / 1000));
                        }
                        else
                        {
                            /* Increment left speed */
                            TC0->TC_CHANNEL[0].TC_RA = (1000000 / MOTOR_FREQUENCY) * ((TC0->TC_CHANNEL[0].TC_RA / (1000000 / MOTOR_FREQUENCY)) + 
                                                            (GET_INCREMENT_FROM_ACCELERATION(CORRECTION_ACCELERATION) / 1000));
                        }
                    }
                    else
                    {
                        if (MotorCtl.leftTickCounter > MotorCtl.rightTickCounter)
                        {
                            /* Decrement left speed */
                            TC0->TC_CHANNEL[0].TC_RA = (1000000 / MOTOR_FREQUENCY) * ((TC0->TC_CHANNEL[0].TC_RA / (1000000 / MOTOR_FREQUENCY)) - 
                                                            (GET_INCREMENT_FROM_ACCELERATION(CORRECTION_ACCELERATION) / 1000));
                        }
                        else
                        {
                            /* Decrement right speed */
                            TC0->TC_CHANNEL[1].TC_RA = (1000000 / MOTOR_FREQUENCY) * ((TC0->TC_CHANNEL[1].TC_RA / (1000000 / MOTOR_FREQUENCY)) - 
                                                            (GET_INCREMENT_FROM_ACCELERATION(CORRECTION_ACCELERATION) / 1000));
                        }
                    }
                }
            }
            
            /* If current speed is not acceptable, accelerate or decelerate to acceptable speed*/
            if (!IN_DELTA(GetMotorSpeed(), GET_MOTOR_VELOCITY(), ACCEPTABLE_SPEED_RANGE))
            {
                Accelerate();
            }
        }
    }  
}