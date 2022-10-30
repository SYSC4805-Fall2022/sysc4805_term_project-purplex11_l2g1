#include <stdio.h>
#include <stdbool.h>

//returns true or false if the sensor detects anything
bool lineDetector(int pin){
    if(digitalRead(pin)){
        return true;
    }else{
        return false;
    }
}