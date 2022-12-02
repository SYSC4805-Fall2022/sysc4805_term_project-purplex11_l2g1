#include "lineFollower.h"

LineFollower LineDetector;

void LineFollower::LineFollowerInit(){
    pinMode(LEFT_LINE_DETECT_PIN, INPUT);
    pinMode(MIDDLE_LINE_DETECT_PIN, INPUT);
    pinMode(RIGHT_LINE_DETECT_PIN, INPUT);
}

LineDetectState_t LineFollower::pollLeftLineDetect(){
    int leftDetectRead = digitalRead(LEFT_LINE_DETECT_PIN);
    if (leftDetectRead == 1) {
        leftDetectState = LINE;
    } else {
        leftDetectState = NO_LINE;
    }
    return leftDetectState;
}

LineDetectState_t LineFollower::pollMiddleLineDetect(){
    int middleDetectRead = digitalRead(MIDDLE_LINE_DETECT_PIN);
    if (middleDetectRead == 1) {
        middleDetectState = LINE;
    } else {
        middleDetectState = NO_LINE;
    }
    return middleDetectState;
}

LineDetectState_t LineFollower::pollRightLineDetect(){
    int rightDetectRead = digitalRead(RIGHT_LINE_DETECT_PIN);
    if (rightDetectRead == 1) {
        rightDetectState = LINE;
    } else {
        rightDetectState = NO_LINE;
    }
    return rightDetectState;
}


LineDetectState_t LineFollower::pollLine(){
    pollLeftLineDetect();
    pollMiddleLineDetect();
    pollRightLineDetect();

    return MIN(MIN(leftDetectState, middleDetectState), rightDetectState);
}
