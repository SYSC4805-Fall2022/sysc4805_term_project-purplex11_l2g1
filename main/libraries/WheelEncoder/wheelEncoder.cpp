#include "wheelEncoder.h"

WheelEncoders encoders;

void WheelEncoders::WheelEncoderInit(){
    /* Attach encoder ISR to the Timer5 handler */
    Timer5.attachInterrupt(WheelEncoderISR);

    /* Initialize the timer counters for the encoders */
    PMC->PMC_PCER0 |= PMC_PCER0_PID28 | PMC_PCER0_PID29; 
    TC0->TC_CHANNEL[1].TC_CMR = TC_CMR_TCCLKS_TIMER_CLOCK1 
        | TC_CMR_ABETRG 
        | TC_CMR_LDRA_RISING
        | TC_CMR_ETRGEDG_RISING; 
    TC0->TC_CHANNEL[1].TC_CCR = TC_CCR_SWTRG | TC_CCR_CLKEN; 
    TC0->TC_CHANNEL[1].TC_IER |= TC_IER_LDRAS;

    TC2->TC_CHANNEL[0].TC_CMR = TC_CMR_TCCLKS_TIMER_CLOCK1 
        | TC_CMR_ABETRG
        | TC_CMR_LDRA_RISING
        | TC_CMR_ETRGEDG_RISING; 
    TC2->TC_CHANNEL[0].TC_CCR = TC_CCR_SWTRG | TC_CCR_CLKEN; 
    TC2->TC_CHANNEL[0].TC_IER |= TC_IER_LDRBS;

    /* Initialize all variables to 0 */
    lastRecordedSpeedLeft = 0;
    lastRecordedSpeedRight = 0;
    accumulatedCountLeft = 0;
    accumulatedCountRight = 0;
    totalCountLeft = 0;
    totalCountRight = 0;
    encoderStartTime = 0;
    encoderEndTime = 0;
    lastInterruptTimeLeft = 0;
    lastInterruptTimeRight = 0;
    encoderAccumulationTime = 0;

    /* Disable encoders on default */
    Disable();
}

void WheelEncoders::Disable(){
    /* Disable the encoders by disabling NVIC for all interrupts */
    NVIC_DisableIRQ(TC1_IRQn);
    NVIC_DisableIRQ(TC6_IRQn);
    Timer5.stop();
}

void WheelEncoders::Enable(int16_t time_ms, int16_t accumulation_time_ms){
    /* Limit time to be always greater than 0 */
    if (time_ms <= 0) {
        encoderEndTime = millis() + 300000; // set end time to 5 minutes
    } else {
        encoderEndTime = millis() + time_ms;
    }

    /* Limit accumulation time to be greater than minimum accumulation time */
    if (accumulation_time_ms < MIN_ACCUMULATION_TIME_MS) {
        accumulation_time_ms = MIN_ACCUMULATION_TIME_MS;
    }

    /* Enable required NVIC controllers */
    Timer5.start(accumulation_time_ms);
    NVIC_EnableIRQ(TC1_IRQn);
    NVIC_EnableIRQ(TC6_IRQn);
}

void WheelEncoders::Enable(){
    /* Enable encoders for 5 minutes and min accumulation time  */
    Enable(-1,0);
}

uint16_t WheelEncoders::GetSpeed(){
    /* Return the max speed of both encoders */
    return MAX(lastRecordedSpeedLeft, lastRecordedSpeedRight);
}

void WheelEncoders::ResetTotalCounts(){
    /* Reset count sum to 0 */
    totalCountLeft = 0;
    totalCountRight = 0;
}

uint16_t WheelEncoders::GetTotalCount() {
    /* Returns the sum of both encoders counts */
    return (totalCountLeft + totalCountRight);
}


void WheelEncoderISR() {
    /* Retrieve the speed from the accumulated count */
    encoders.lastRecordedSpeedLeft = SPEED_FROM_COUNT(encoders.accumulatedCountLeft*1000, encoders.accumulation_time_ms); //Multiply count by 1000 rather than divide time 
    encoders.lastRecordedSpeedRight = SPEED_FROM_COUNT(encoders.accumulatedCountRight*1000, encoders.accumulation_time_ms); // as whole numbers are easier to work with

    /* Reset accumulated counts to 0 */
    encoders.accumulatedCountLeft = 0;
    encoders.accumulatedCountRight = 0;

    /* If time passed time limit set in enable, disable the encoders */
    if (millis() >= encoders.encoderEndTime) {
        encoders.Disable();
    }
}

void TC1_Handler() {
    /* Increment count on every interrupt if time is greater than debouncing time */
    uint32_t status = TC0->TC_CHANNEL[1].TC_SR;
    if ((status & TC_SR_LDRAS) && (millis() - encoders.lastInterruptTimeLeft > DEBOUNCING_TIME)) { 
        encoders.accumulatedCountLeft++;
        encoders.totalCountLeft++;
        encoders.lastInterruptTimeLeft = millis();
    }
}

void TC6_Handler() {
    /* Increment count on every interrupt if time is greater than debouncing time */
    uint32_t status = TC2->TC_CHANNEL[0].TC_SR;
    if ((status & TC_SR_LDRAS) && (millis() - encoders.lastInterruptTimeRight > DEBOUNCING_TIME)) { 
        encoders.accumulatedCountRight++;
        encoders.totalCountRight++;
        encoders.lastInterruptTimeRight = millis();
    }
}