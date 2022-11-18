/****************************************************************************************
 * Filename: wheelEncoder.h
 * Author: Justin Whalley
 * Description: Header file to handle contain encoder structs and function declarations
****************************************************************************************/

#ifndef WHEEL_ENCODER_H
#define WHEEL_ENCODER_H

#include "common.h"

/****************************************************************************************
 * Definitions
****************************************************************************************/

#define ACCUMULATION_TIME_MS        200
#define DEBOUNCING_TIME             25
#define MAX_SPEED                   25
#define SPEED_FROM_COUNT(X,Y)       ((18.84*(X/10))/Y)

/****************************************************************************************
 * Enums
****************************************************************************************/
typedef enum
{
    LEFT = 0,
    RIGHT = 1,
} WheelEncoderSide_t;

/****************************************************************************************
 * Structs
****************************************************************************************/

class WheelEncoderOneSide
{
public: 
    uint32_t startTime;
    uint16_t accumulatedCount;
    uint16_t count;
    uint32_t time;
    uint16_t cachedCount;
    uint32_t cachedTime;
    uint8_t  speed;
    uint32_t lastInterruptTime;

    void WheelEncoderOneSideInit(WheelEncoderSide_t side);

    void EncoderReset();
};

class WheelEncoders
{   
public:
    WheelEncoderOneSide LeftWheelEncoder;
    WheelEncoderOneSide RightWheelEncoder;

    void WheelEncodersInit();

    void EncoderReset();
    void EncoderResetLeft();
    void EncoderResetRight();

    uint16_t GetEncoderCountLeft();
    uint16_t GetEncoderCountRight();

    uint32_t GetEncoderTimeLeft();
    uint32_t GetEncoderTimeRight();

    uint8_t GetMeasuredSpeedLeft();
    uint8_t GetMeasuredSpeedRight();
};

void EncoderISR();
void TC1_Handler();
void TC6_Handler();

extern WheelEncoders encoders;
/****************************************************************************************
 * Methods
****************************************************************************************/


#endif