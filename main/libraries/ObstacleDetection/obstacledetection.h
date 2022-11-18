/****************************************************************************************
 * Filename: motor.h
 * Author: Marwan Zeyada
 * Description: Header file to contain structs and function declarations for obstacle detection module
****************************************************************************************/

#ifndef OBSTACLE_H
#define OBSTACLE_H

#include "common.h"
#include "motor.h"
#include <VL53L1X.h>

#define D_THRESHOLD_MM 100

typedef enum {
    CHECKING = 0,
    TURNING_LEFT = 1,
    MOVING_STRAIGHT = 2,
    TURNING_RIGHT = 3
} ObstacleState_t;

class ObstacleDetectors{
public:
    ObstacleState_t obstacleDetectedState;
    uint8_t timerticks;

    void ObstacleDetectorInit();

};

void ObstacleDetectionISR();

extern ObstacleDetectors ObstacleDetector;
extern VL53L1X sensor;
#endif