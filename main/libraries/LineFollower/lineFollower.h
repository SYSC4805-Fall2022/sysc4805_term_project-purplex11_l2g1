#ifndef LINE_FOLLOWER_H
#define LINE_FOLLOWER_H

#include <stdint.h>
#include "arduino.h"

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

    friend void pinMode(uint32_t ulPin, uint32_t ulMode);
    friend int digitalRead(uint32_t ulPin);

    void LineFollowerInit();
    LineDetectState_t pollLeftLineDetect();
    LineDetectState_t pollMiddleLineDetect();
    LineDetectState_t pollRightLineDetect();

    LineDetectState_t pollLine();
};

extern LineFollower LineDetector;

#endif