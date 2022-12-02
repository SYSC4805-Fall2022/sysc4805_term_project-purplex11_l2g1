#ifndef WHEEL_ENCODER_H
#define WHEEL_ENCODER_H

#include "arduino.h"
#include <DueTimer.h>

#define DEBOUNCING_TIME            25
#define MIN_ACCUMULATION_TIME_MS   DEBOUNCING_TIME

#ifndef MAX(X, Y)
#define MAX(X, Y)                  (X > Y ? X : Y)
#endif
#define SPEED_FROM_COUNT(X,Y)       ((18.84*(X/10))/Y)

typedef enum {
    DISABLED = 0,
    ENABLED = 1
} WheelEncoderState_t;

class WheelEncoders
{
public:
    uint16_t            lastRecordedSpeedLeft;
    uint16_t            lastRecordedSpeedRight;
    uint16_t            accumulatedCountLeft;
    uint16_t            accumulatedCountRight;
    uint16_t            totalCountLeft;
    uint16_t            totalCountRight;
    long                encoderStartTime;
    long                encoderEndTime;
    long                lastInterruptTimeLeft;
    long                lastInterruptTimeRight;
    int16_t             encoderAccumulationTime;
    int16_t             accumulation_time_ms;

    /**************************************************
     * void WheelEncoderInit()
     * 
     * Method to handle the startup of the encoders. This 
     * method will only be called once before any code 
     * is run.
    **************************************************/
    void WheelEncoderInit();

    /**************************************************
     * void Disable()
     * 
     * Method to disabled both encoders by disabling 
     * all encoder interrupt handlers
    **************************************************/
    void Disable();

    /**************************************************
     * void Enable(int16_t time_ms=-1, int16_t accumulation_time_ms=-1)
     * 
     * Method to enable the encoders by enabling all 
     * encoder interrupt handlers. The time_ms parameter
     * states how long to enable for and will disable the encoders 
     * after the time is reached. If time_ms is negative, it 
     * will enable for 5 minutes. Accumulation time is how long
     * to average the count over before setting the speed.
    **************************************************/
    void Enable(int16_t time_ms, int16_t accumulation_time_ms);

    /**************************************************
     * void Enable()
     * 
     * Method to enable the encoders by enabling all 
     * encoder interrupt handlers. Encoders will be enabled
     * for 5 minutes with an accumultion time of 25.
    **************************************************/
    void Enable();

    /**************************************************
     * uint16_t GetSpeed()
     * 
     * Method to return the maximum speed of the left
     * and right encoders
    **************************************************/
    uint16_t GetSpeed();

    /**************************************************
     * uint16_t GetTotalCount()
     * 
     * Method to return the current sum of encoder triggers
    **************************************************/
    uint16_t GetTotalCount();

    /**************************************************
     * void ResetTotalCounts()
     * 
     * Sets the total count variable for left and right
     * encoders to 0
    **************************************************/
    void ResetTotalCounts();
};

/**************************************************
 * void WheelEncoderISR()
 * 
 * Method called every accumulation_time_ms milliseconds
 * while the encoders are enabled to calculate the 
 * speed of the vehicle
**************************************************/
void WheelEncoderISR();

/**************************************************
 * void TC1_Handler()
 * 
 * Method increment the left counts if the debouncing 
 * time requirement is met
**************************************************/
void TC1_Handler();

/**************************************************
 * void TC6_Handler()
 * 
 * Method increment the right counts if the debouncing 
 * time requirement is met
**************************************************/
void TC6_Handler();

extern WheelEncoders encoders;

#endif