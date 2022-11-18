#pragma once
#include <stdint.h>
#include <motor.h>
#include <common.h>



class lineDetector
{
public:
    bool_t direction; //right = true, left = false
    bool_t linedetected_LEFT;
    bool_t linedetected_RIGHT;
    bool_t linedetected_MIDDLE;
    bool_t linedetectionState;

    void lineDetected_LEFT();
    void lineDetected_RIGHT();
    void lineDetected_MIDDLE();

}

void lineDectectorISR();