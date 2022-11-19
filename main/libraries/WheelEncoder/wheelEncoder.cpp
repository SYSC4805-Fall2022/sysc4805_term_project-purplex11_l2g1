/****************************************************************************************
 * Filename: wheelEncoder.cpp
 * Author: Justin Whalley
 * Description: File to contain the wheel encoder functionality
****************************************************************************************/
#include "wheelEncoder.h"

WheelEncoders encoders;

void WheelEncoderOneSide::WheelEncoderOneSideInit(WheelEncoderSide_t side)
{
    if (side == LEFT)
    {
        PMC->PMC_PCER0 |= PMC_PCER0_PID28 | PMC_PCER0_PID29; // Timer Counter 0 channel 1 IS TC1, TC1 power ON
        TC0->TC_CHANNEL[1].TC_CMR = TC_CMR_TCCLKS_TIMER_CLOCK1 // capture mode, MCK/2 = 42 MHz
            | TC_CMR_ABETRG // TIOA is used as the external trigger
            | TC_CMR_LDRA_RISING// load RA on rising edge of TIOA
            | TC_CMR_ETRGEDG_RISING; // Trigger on rising edge
        TC0->TC_CHANNEL[1].TC_CCR = TC_CCR_SWTRG | TC_CCR_CLKEN; // Reset TC counter and enable
        TC0->TC_CHANNEL[1].TC_IER |= TC_IER_LDRAS;
        NVIC_EnableIRQ(TC1_IRQn);
    }
    else
    {
        TC2->TC_CHANNEL[0].TC_CMR = TC_CMR_TCCLKS_TIMER_CLOCK1 // capture mode, MCK/2 = 42 MHz
            | TC_CMR_ABETRG // TIOA is used as the external trigger
            | TC_CMR_LDRA_RISING// load RA on rising edge of TIOA
            | TC_CMR_ETRGEDG_RISING; // Trigger on rising edge
        TC2->TC_CHANNEL[0].TC_CCR = TC_CCR_SWTRG | TC_CCR_CLKEN; // Reset TC counter and enable
        TC2->TC_CHANNEL[0].TC_IER |= TC_IER_LDRBS;
        NVIC_EnableIRQ(TC6_IRQn);
    }
}

void WheelEncoderOneSide::EncoderReset()
{
    /* Update start time to current time and clear count and time */
    startTime = millis();
    count = 0;
    accumulatedCount = 0;
    time = 0;
}

void WheelEncoders::WheelEncodersInit()
{
    LeftWheelEncoder.WheelEncoderOneSideInit(LEFT);
    RightWheelEncoder.WheelEncoderOneSideInit(RIGHT);
     

    /* Set the start time for encoder calculations to current time*/
    LeftWheelEncoder.startTime = millis();
    RightWheelEncoder.startTime = millis();   

    Scheduler.AddHandler(EncoderISR, ACCUMULATION_TIME_MS);
}

void WheelEncoders::EncoderReset()
{
    /* Reset both left and right encoders*/
    EncoderResetLeft();
    EncoderResetRight();
}

void WheelEncoders::EncoderResetLeft()
{
    /* Update start time to current time and clear count and time */
    LeftWheelEncoder.EncoderReset();
}

void WheelEncoders::EncoderResetRight()
{
    /* Update start time to current time and clear count and time */
    RightWheelEncoder.EncoderReset();
}

uint16_t WheelEncoders::GetEncoderCountLeft()
{
    return LeftWheelEncoder.accumulatedCount;
}

uint16_t WheelEncoders::GetEncoderCountRight()
{
    return RightWheelEncoder.accumulatedCount;
}

uint32_t WheelEncoders::GetEncoderTimeLeft()
{
    return LeftWheelEncoder.time;
}

uint32_t WheelEncoders::GetEncoderTimeRight()
{
    return RightWheelEncoder.time;
}

uint8_t WheelEncoders::GetMeasuredSpeedLeft()
{
    return LeftWheelEncoder.speed;
}

uint8_t WheelEncoders::GetMeasuredSpeedRight()
{
    return RightWheelEncoder.speed;
}

void EncoderISR()
{
    /* Update time period for count*/
    encoders.LeftWheelEncoder.time = millis() - encoders.LeftWheelEncoder.startTime;
    encoders.RightWheelEncoder.time = millis() - encoders.RightWheelEncoder.startTime;

    /* Cache time and count */
    encoders.LeftWheelEncoder.cachedTime = encoders.LeftWheelEncoder.time;
    encoders.RightWheelEncoder.cachedTime = encoders.RightWheelEncoder.time;

    encoders.RightWheelEncoder.cachedCount = encoders.RightWheelEncoder.cachedCount;
    encoders.LeftWheelEncoder.cachedCount = encoders.LeftWheelEncoder.cachedCount;

    /* Calculate and store speed */
    encoders.RightWheelEncoder.speed = SPEED_FROM_COUNT(encoders.RightWheelEncoder.count, encoders.RightWheelEncoder.time);
    encoders.LeftWheelEncoder.speed = SPEED_FROM_COUNT(encoders.LeftWheelEncoder.count, encoders.LeftWheelEncoder.time);

    /* Reset encoders to begin next iteration*/
    encoders.RightWheelEncoder.EncoderReset();
    encoders.LeftWheelEncoder.EncoderReset();
}


void TC1_Handler() {
    uint32_t status = TC0->TC_CHANNEL[1].TC_SR; //Read status register, Clear status
    if ((status & TC_SR_LDRAS) && (millis() - encoders.LeftWheelEncoder.lastInterruptTime > DEBOUNCING_TIME)) { // If ISR is fired by LDRAS then ....
        encoders.LeftWheelEncoder.accumulatedCount++;
        encoders.LeftWheelEncoder.count++;
        encoders.LeftWheelEncoder.time = millis() - encoders.LeftWheelEncoder.startTime;
        encoders.LeftWheelEncoder.lastInterruptTime = millis();
    }
}

void TC6_Handler() {
    uint32_t status = TC2->TC_CHANNEL[0].TC_SR; //Read status register, Clear status
    if ((status & TC_SR_LDRAS) && (millis() - encoders.RightWheelEncoder.lastInterruptTime > DEBOUNCING_TIME)) { // If ISR is fired by LDRAS then ....
        encoders.RightWheelEncoder.accumulatedCount++;
        encoders.RightWheelEncoder.count++;
        encoders.RightWheelEncoder.time = millis() - encoders.RightWheelEncoder.startTime;
        encoders.RightWheelEncoder.lastInterruptTime = millis();
    }
}