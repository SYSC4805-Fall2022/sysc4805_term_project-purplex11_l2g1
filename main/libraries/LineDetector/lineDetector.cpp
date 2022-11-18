#include "lineDectector.h"


void lineDetector::lineDetectorInit{
     direction = TRUE;

     attachInterrupt(LINE_DETECTION_ECHO_PIN, lineDetector(), RISING);
}

void lineDetector::lineDetected(){
    if(detected()){
        
        Motors.Turn(DEGREE_180, direction);
        direction = !direction;

    }
}

bool_t lineDetector::detected(){
    if(digitalRead(LINE_DETECTION_ECHO_PIN)){
        return true;
    }
    return false;
    
}


