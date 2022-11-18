#pragma once
#include <stdint.h>
#include <motor.h>
#include <common.h>



class lineDetector
{
public:
    bool_t direction; //right = true, left = false
    bool_t linedetected;

    void lineDetector();

    bool_t detected();
}

void lineDectectorISR();