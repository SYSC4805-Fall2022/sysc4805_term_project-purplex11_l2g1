#include "motor.h"

Motors Vehicle;

void Motors::motorInit() {
    state = STOPPED;

    pinMode(LEFT_MOTOR_DIR_PIN, OUTPUT);
    pinMode(RIGHT_MOTOR_DIR_PIN, OUTPUT);

    digitalWrite(LEFT_MOTOR_DIR_PIN, LEFT_MOTOR_FORWARD_PIN_STATE);
    digitalWrite(RIGHT_MOTOR_DIR_PIN, RIGHT_MOTOR_FORWARD_PIN_STATE);

    PMC->PMC_PCER0 |= PMC_PCER0_PID27;
    PIOB->PIO_PDR |= PIO_PDR_P25; 
    PIOB->PIO_ABSR |= PIO_PB25B_TIOA0;
    TC0->TC_CHANNEL[0].TC_CMR = TC_CMR_TCCLKS_TIMER_CLOCK1 
                                | TC_CMR_WAVE 
                                | TC_CMR_WAVSEL_UP_RC 
                                | TC_CMR_ACPA_CLEAR 
                                | TC_CMR_ACPC_SET; 
    TC0->TC_CHANNEL[0].TC_RC = ( 42000000 / 100) -1; 
    TC0->TC_CHANNEL[0].TC_RA = (42000000 / 100) * 0.25; 
    TC0->TC_CHANNEL[0].TC_CCR = TC_CCR_SWTRG 
                                | TC_CCR_CLKEN;
                                
    PMC->PMC_PCER1 |= PMC_PCER1_PID36; // Enable Clock to PWM module
    PIOC->PIO_ABSR |= PIO_PC3B_PWMH0; // Assign C3 to PWM module (Periph_B)
    PIOC->PIO_PDR |= PIO_PDR_P3; // Release C3 from the PIO module
    REG_PWM_CLK = PWM_CLK_PREA(0) | PWM_CLK_DIVA(84);//Set PWM clock 1MHz (Mck/84)
    PWM->PWM_CH_NUM[0].PWM_CMR |= PWM_CMR_CPRE_CLKA // Set the clock source as CLKA
    | PWM_CMR_CPOL; //Set output polarity be high.
    PWM->PWM_CH_NUM[0].PWM_CPRD = ( 1000000 / 100) -1; 
    PWM->PWM_CH_NUM[0].PWM_CDTY = ( 1000000 / 100)* 0.25; // Set PWM duty cycle
    PWM->PWM_ENA = PWM_ENA_CHID0; // Enable the PWM channel

    stop();
}

void Motors::stop() {
    if (rightMotorState != STOPPED) {
        PWM->PWM_CH_NUM[0].PWM_CDTY = PWM_STOP;
    }

    if (leftMotorState != STOPPED) {
        TC0->TC_CHANNEL[0].TC_RA = TC_STOP;
    }

    leftMotorState = MOTOR_STOPPED;
    rightMotorState = MOTOR_STOPPED;
    state = STOPPED;
}


State_t Motors::getState() {
    return state;
}


void Motors::moveForwardLeft() {
    if (leftMotorState != MOTOR_FORWARD) {
        digitalWrite(LEFT_MOTOR_DIR_PIN, LEFT_MOTOR_FORWARD_PIN_STATE);
        TC0->TC_CHANNEL[0].TC_RA = TC_DUTY_MOVING;
        leftMotorState = MOTOR_FORWARD;
        if (rightMotorState == MOTOR_FORWARD) {
            state = FORWARD;
        }
    }

    
}


void Motors::moveForwardRight() {
    if (rightMotorState != MOTOR_FORWARD) {
        digitalWrite(RIGHT_MOTOR_DIR_PIN, RIGHT_MOTOR_FORWARD_PIN_STATE);
        PWM->PWM_CH_NUM[0].PWM_CDTY = PWM_DUTY_MOVING;
        rightMotorState = MOTOR_FORWARD;
        if (leftMotorState == MOTOR_FORWARD) {
            state = FORWARD;
        }
    }
}


void Motors::moveForward() {
    moveForwardLeft();
    moveForwardRight();
}


void Motors::moveReverseLeft() {
    if (leftMotorState != MOTOR_REVERSE) {
        digitalWrite(LEFT_MOTOR_DIR_PIN, LEFT_MOTOR_REVERSE_PIN_STATE);
        TC0->TC_CHANNEL[0].TC_RA = TC_DUTY_MOVING;
        leftMotorState = MOTOR_REVERSE;
        if (rightMotorState == MOTOR_REVERSE) {
            state = REVERSE;
        }
    }
}


void Motors::moveReverseRight() {
    if (rightMotorState != MOTOR_REVERSE) {
        digitalWrite(RIGHT_MOTOR_DIR_PIN, RIGHT_MOTOR_REVERSE_PIN_STATE);
        PWM->PWM_CH_NUM[0].PWM_CDTY = PWM_DUTY_MOVING;
        rightMotorState = MOTOR_REVERSE;
        if (leftMotorState == MOTOR_REVERSE) {
            state = REVERSE;
        }
    }
}


void Motors::moveReverse() {
    moveReverseLeft();
    moveReverseRight();
}
 

void Motors::turn45(bool left) {
    if (left) {
        moveForwardRight();
        moveReverseLeft();
    } else {
        moveReverseRight();
        moveForwardLeft();
    }
    state = TURNING;
    Timer4.attachInterrupt(TurnISR).start(TURN_45_TIME_MS*1000);
}


void Motors::turn90(bool left) {
    if (left) {
        moveForwardRight();
        moveReverseLeft();
    } else {
        moveReverseRight();
        moveForwardLeft();
    }
    state = TURNING;
    Timer4.attachInterrupt(TurnISR).start(TURN_90_TIME_MS*1000);
}


void Motors::turn180(bool left) {
    state = TURNING;
    if (left) {
        moveForwardRight();
        moveReverseLeft();
    } else {
        moveReverseRight();
        moveForwardLeft();
    }
    state = TURNING;
    Timer4.attachInterrupt(TurnISR).start(TURN_180_TIME_MS*1000); 
}

void Motors::CancelTurn() {
    stop();
    Timer4.stop();
}

void TurnISR() {
    Vehicle.CancelTurn();
}