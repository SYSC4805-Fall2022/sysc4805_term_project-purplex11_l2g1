#include "lineDectector.h"


void lineDetector::lineDetectorInit{
     direction = TRUE;

     attachInterrupt(LINE_DETECTION_ECHO_PIN, lineDetector(), RISING);
}

void lineDetector::lineDetected(){
    
        
    Motors.Turn(DEGREE_180, direction);
    direction = !direction;

}


