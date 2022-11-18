#include "lineDetector.h"

lineDetector lineD;
void lineDetector::lineDetectorInit(){
     direction = TRUE;
     linedetected_LEFT = FALSE;
     linedetected_RIGHT = FALSE;
     linedetected_MIDDLE = FALSE;
     linedetectionState = FALSE;

     //attachInterrupt(LINE_DETECTION_ECHO_PIN_LEFT, lineDetected_LEFT(), RISING); //they would all call the same function
     //attachInterrupt(LINE_DETECTION_ECHO_PIN_RIGHT, lineDetected_RIGHT(), RISING);
     //attachInterrupt(LINE_DETECTION_ECHO_PIN_FRONT, lineDetected_MIDDLE(), RISING);
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

    if(lineD.linedetected_LEFT && lineD.linedetected_RIGHT && lineD.linedetected_MIDDLE){
        Motors.Turn(DEGREE_180, lineD.direction);
        //lineD.direction = !lineD.direction;
        lineD.linedetected_LEFT = FALSE;
        lineD.linedetected_RIGHT = FALSE;
        lineD.linedetected_MIDDLE = FALSE;
        lineD.linedetectionState = TRUE;
    }else if (lineD.linedetected_RIGHT && lineD.linedetected_MIDDLE){
        Motors.Turn(DEGREE_45, FALSE);
        lineD.linedetected_RIGHT = FALSE;
        lineD.linedetected_MIDDLE = FALSE;
        lineD.linedetectionState = TRUE;
    }else if (lineD.linedetected_LEFT && lineD.linedetected_MIDDLE){
        Motors.Turn(DEGREE_45, TRUE);
        lineD.linedetected_LEFT = FALSE;
        lineD.linedetected_MIDDLE = FALSE;
        lineD.linedetectionState = TRUE;
    }else if (lineD.linedetected_RIGHT){
        Motors.Turn(DEGREE_5, FALSE);
        lineD.linedetected_RIGHT = FALSE;
        lineD.linedetectionState = TRUE;
    }else if(lineD.linedetected_LEFT){
        Motors.Turn(DEGREE_5, TRUE);
        lineD.linedetected_LEFT = FALSE;
        lineD.linedetectionState = TRUE;
    }

    if(lineD.linedetectionState){
        if(Motors.isTurning){
            Motors.MoveForward();
            lineD.linedetectionState = FALSE;
        }
    }
}

