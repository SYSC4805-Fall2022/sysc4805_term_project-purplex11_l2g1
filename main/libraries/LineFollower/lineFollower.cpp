#include "lineFollower.h"

LineFollower LineDetector;

void LineFollower::LineFollowerInit(){
    /* Set the detection pins as inputs */
    pinMode(LEFT_LINE_DETECT_PIN, INPUT);
    pinMode(MIDDLE_LINE_DETECT_PIN, INPUT);
    pinMode(RIGHT_LINE_DETECT_PIN, INPUT);
}

LineDetectState_t LineFollower::pollLeftLineDetect(){
    /* Read the input pin and convert int to enum */
    int leftDetectRead = analogRead(LEFT_LINE_DETECT_PIN);
    if (leftDetectRead >= 750) {
        leftDetectState = LINE;
    } else {
        leftDetectState = NO_LINE;
    }
    return leftDetectState;
}

LineDetectState_t LineFollower::pollMiddleLineDetect(){
    /* Read the input pin and convert int to enum */
    int middleDetectRead = analogRead(MIDDLE_LINE_DETECT_PIN);
    if (middleDetectRead >= 750) {
        middleDetectState = LINE;
    } else {
        middleDetectState = NO_LINE;
    }
    return middleDetectState;
}

LineDetectState_t LineFollower::pollRightLineDetect(){
    /* Read the input pin and convert int to enum */
    int rightDetectRead = analogRead(RIGHT_LINE_DETECT_PIN);
    if (rightDetectRead >= 750) {
        rightDetectState = LINE;
    } else {
        rightDetectState = NO_LINE;
    }
    return rightDetectState;
}


LineDetectState_t LineFollower::pollLine(){
    /* Update all line detect states */
    pollLeftLineDetect();
    pollMiddleLineDetect();
    pollRightLineDetect();

    /* Return the minimum of all sensors to ensure all 
     * sensors detect a line before triggering */
    return MAX(MAX(leftDetectState, middleDetectState), rightDetectState);
}
