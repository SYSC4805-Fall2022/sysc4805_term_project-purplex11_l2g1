#include "lineDectector.h"


void lineDetector::lineDetectorInit{
     direction = TRUE;
     linedetected = FALSE;

     attachInterrupt(LINE_DETECTION_ECHO_PIN, lineDetected, RISING);
}

void lineDetector::lineDetected(){
    
        
    Motors.Turn(DEGREE_180, direction);
    direction = !direction;
    linedetected = TRUE;

}

void lineDectectorISR(){
    if(lineDetector.linedetected){
        if(Motors.isTurning){
            Motors.moveForward();
            lineDetector.lineDetector = FALSE;
        }
    }
}


