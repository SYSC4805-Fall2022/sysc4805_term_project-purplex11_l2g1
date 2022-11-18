#pragma once
#include <stdint.h>
#include <motor.h>
#include <common.h>



class lineDetector
{
public:
    bool_t direction; //right = true, left = false

    void lineDetector();

    bool_t detected();
}