#include "lineDectector.h"


void lineDetector::lineDetectorInit{
     direction = TRUE;
     linedetected_LEFT = FALSE;
     linedetected_RIGHT = FALSE;
     linedetected_MIDDLE = FALSE;
     linedetectionState = FALSE;

     attachInterrupt(LINE_DETECTION_ECHO_PIN_LEFT, lineDetected, RISING); //they would all call the same function
     attachInterrupt(LINE_DETECTION_ECHO_PIN_RIGHT, lineDetected_RIGHT, RISING);
     attachInterrupt(LINE_DETECTION_ECHO_PIN_FRONT, lineDetected_MIDDLE, RISING);
}

void lineDetector::lineDetected_LEFT(){
    
    linedetected_LEFT = TRUE;

}
void lineDetector::lineDetected_RIGHT(){
    
    linedetected_RIGHT = TRUE;

}
void lineDetector::lineDetected_MIDDLE(){
    
    linedetected_MIDDLE = TRUE;

}


void lineDectectorISR(){

    if(linedetected_LEFT && linedetected_RIGHT && linedetected_MIDDLE){
        Motors.Turn(DEGREE_180, direction);
        direction = !direction;
        lineDetector.linedetected_LEFT = FALSE;
        lineDetector.linedetected_RIGHT = FALSE;
        lineDetector.linedetected_MIDDLE = FALSE;
        lineDetector.linedetectionState = TRUE;
    }else if (linedetected_RIGHT && linedetected_MIDDLE){
        Motors.Turn(DEGREE_45, FALSE);
        lineDetector.linedetected_RIGHT = FALSE;
        lineDetector.linedetected_MIDDLE = FALSE;
        lineDetector.linedetectionState = TRUE;
    }else if (linedetected_LEFT && linedetected_MIDDLE){
        Motors.Turn(DEGREE_45, TRUE);
        lineDetector.linedetected_LEFT = FALSE;
        lineDetector.linedetected_MIDDLE = FALSE;
        lineDetector.linedetectionState = TRUE;
    }else if (linedetected_RIGHT){
        Motors.Turn(DEGREE_5, FALSE);
        lineDetector.linedetected_RIGHT = FALSE;
        lineDetector.linedetectionState = TRUE;
    }else if(linedetected_LEFT){
        Motors.Turn(DEGREE_5, TRUE);
        lineDetector.linedetected_LEFT = FALSE;
        lineDetector.linedetectionState = TRUE;
    }

    if(lineDetector.linedetectionState){
        if(Motors.isTurning){
            Motors.moveForward();
            lineDetector.linedetectionState = FALSE;
        }
    }
}

