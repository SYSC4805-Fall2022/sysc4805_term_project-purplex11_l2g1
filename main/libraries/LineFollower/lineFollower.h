#ifndef LINE_FOLLOWER_H
#define LINE_FOLLOWER_H

#include <stdint.h>
#include "arduino.h"
#include <DueTimer.h>

#define LEFT_LINE_DETECT_PIN    28
#define MIDDLE_LINE_DETECT_PIN  29
#define RIGHT_LINE_DETECT_PIN   30

#define MIN(X, Y)                (X < Y ? X : Y)


typedef enum {
    NO_LINE = 0,
    LINE = 1,
} LineDetectState_t;

class LineFollower {

public:
    LineDetectState_t leftDetectState;
    LineDetectState_t middleDetectState;
    LineDetectState_t rightDetectState;

    /**************************************************
     * Methods from arduino.h that are used within the 
     * class
    **************************************************/
    friend void pinMode(uint32_t ulPin, uint32_t ulMode);
    friend int digitalRead(uint32_t ulPin);

    /**************************************************
     * void LineFollowerInit()
     * 
     * Method to handle the startup of the Line follower.
     * This method will only be called once before any code 
     * is run.
    **************************************************/
    void LineFollowerInit();

    /**************************************************
     * LineDetectState_t pollLeftLineDetect()
     * 
     * Method to return whether or not there is a 
     * line under the left sensor
    **************************************************/
    LineDetectState_t pollLeftLineDetect();

    /**************************************************
     * LineDetectState_t pollMiddleLineDetect()
     * 
     * Method to return whether or not there is a 
     * line under the middle sensor
    **************************************************/
    LineDetectState_t pollMiddleLineDetect();

    /**************************************************
     * LineDetectState_t pollRightLineDetect()
     * 
     * Method to return whether or not there is a 
     * line under the right sensor
    **************************************************/
    LineDetectState_t pollRightLineDetect();

    /**************************************************
     * LineDetectState_t pollLine()
     * 
     * Method to poll all 3 sensors and return line 
     * detected if and only if all 3 sensors detected a 
     * line
    **************************************************/
    LineDetectState_t pollLine();
};

extern LineFollower LineDetector;

#endif