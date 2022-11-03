/****************************************************************************************
 * Filename: common.h
 * Author: Justin Whalley
 * Description: header file that all modules include. The module contains math macros,
 *              arduino pin outs, arduino functions and other common functionality
****************************************************************************************/

#ifndef COMMON_H
#define COMMON_H

#include <stdint.h>
#include <DueTimer.h>
#include <string.h>
#include "arduino.h"

/****************************************************************************************
 * Definitions
****************************************************************************************/

#define ABS(X)                      (X < 0 ? -X : X)
#define MAX(X, Y)                   (X > Y ? X : Y)
#define MIN(X, Y)                   (X < Y ? X : Y)
#define IN_DELTA(X, Y, Z)           (((X > (Y - Z)) && (X < (Y + Z))) ? TRUE : FALSE)

#define MOTOR_1_2_CTRL_PIN          0
#define MOTOR_1_2_DIR_PIN           0
#define MOTOR_3_4_CTRL_PIN          0
#define MOTOR_3_4_DIR_PIN           0
#define MOTOR_ENCODER_RIGHT         0
#define MOTOR_ENCODER_LEFT          0
#define LINE_DETECTION_ECHO_PIN     0
#define SONAR_ECHO_PIN              0
#define SONAR_CTRL_PIN              0
#define VL53L1X_1_ECHO_PIN          0
#define VL53L1X_2_ECHO_PIN          0
#define MINIMU_ECHO_PIN             0

#define MOTOR_FREQUENCY             100 // Hz

/****************************************************************************************
 * Enums
****************************************************************************************/

typedef enum
{
    FALSE = 0,
    TRUE = 1,
} bool_t;


/****************************************************************************************
 * Structs
****************************************************************************************/


/****************************************************************************************
 * Methods
****************************************************************************************/

#endif