#include lineDectector.h

Motor Motors;


void lineDetectorInit(){
     direction = TRUE;

     attachInterrupt(LINE_DETECTION_ECHO_PIN, lineDetector(), RISING);
}

void lineDetector(){
    if(detected()){
        
        Motors.Turn(DEGREE_180, direction);
        direction = !direction;

    }
}

bool_t detected(){
    if(digitalRead(LINE_DETECTION_ECHO_PIN)){
        return true;
    }
    return false;
    
}


