#include "MainFsm.h"

VehicleFsm MainVehicleFsm;


void VehicleFsm::MainFsmInit() {
    mainFsmState = INIT_STATE;
    obstacleFsmState = OBSTACLE_STOP_STATE;
    lineFsmState = LINE_STOP_STATE;
    LineDetector.LineFollowerInit();
    DistanceSensor.UltrasonicSensorInit();
    Vehicle.motorInit();
    tickTime = DEFAULT_TICK_TIME_MS;
    Serial.begin(9600);
}


void VehicleFsm::MainFsmStart() {
    mainFsmState = STEADY_STATE;   
    MainFsm();     
}


void VehicleFsm::MainFsmTick() {
    switch (mainFsmState) {
        case STEADY_STATE:
            if (Vehicle.getState() != FORWARD) {
                Vehicle.moveForward();
            }
            StateCheck();
            break;
        case OBSTACLE_STATE:
            ObstacleState();
            StateCheck();
            break;
        case LINE_STATE:
            LineState();
            StateCheck();
            break;
        case RANDOM_WALK_STATE:
            RandomWalkState();
            StateCheck();
            break;
    }
}


void VehicleFsm::MainFsm() {
    while (true) {
        MainFsmTick();
        delay(tickTime);
    }
}


void VehicleFsm::ObstacleState() {
    switch (obstacleFsmState) {
        case OBSTACLE_STOP_STATE:
            Vehicle.stop();
            obstacleFsmState = OBSTACLE_REVERSE_STATE;
            break;
        case OBSTACLE_REVERSE_STATE:
            Vehicle.moveReverse();
            reverseStartTime = millis();
            obstacleFsmState = OBSTACLE_REVERSE_WAIT_STATE;
            break;
        case OBSTACLE_REVERSE_WAIT_STATE:
            if (((millis() - reverseStartTime)/1000) >= OBSTACLE_REVERSE_TIME_S) {
                Vehicle.stop();
                obstacleFsmState = OBSTACLE_TURN_STATE;
            }
            break;
        case OBSTACLE_TURN_STATE:
            Vehicle.turn90(true);
            obstacleFsmState = OBSTACLE_TURN_WAIT_STATE;
            break;
        case OBSTACLE_TURN_WAIT_STATE:
            if (Vehicle.getState() != TURNING) {
                Vehicle.moveForward();
                mainFsmState = STEADY_STATE;
            }
    }
}


void VehicleFsm::LineState() {
    switch (lineFsmState) {
        case LINE_STOP_STATE:
            Vehicle.stop();
            lineFsmState = LINE_REVERSE_STATE;
            break;
        case LINE_REVERSE_STATE:
            Vehicle.moveReverse();
            reverseStartTime = millis();
            lineFsmState = LINE_REVERSE_WAIT_STATE;
            break;
        case LINE_REVERSE_WAIT_STATE:
            if (((millis() - reverseStartTime)/1000) >= LINE_REVERSE_TIME_S) {
                Vehicle.stop();
                lineFsmState = LINE_TURN_STATE;
            }
            break;
        case LINE_TURN_STATE:
            Vehicle.turn90(true);
            lineFsmState = LINE_TURN_WAIT_STATE;
            break;
        case LINE_TURN_WAIT_STATE:
            if (Vehicle.getState() != TURNING) {
                Vehicle.moveForward();
                mainFsmState = STEADY_STATE;
            }
            break;
    }
}

void VehicleFsm::RandomWalkState() {
    switch (randomWalkFsmState) {
        case RANDOM_WALK_STOP_STATE:
            Vehicle.stop();
            randomWalkFsmState = RANDOM_WALK_TURN_STATE;
            break;
        case RANDOM_WALK_TURN_STATE:
            if (GET_RANDOM_BOOL()) {
                Vehicle.turn45(GET_RANDOM_BOOL());
            } else {
                Vehicle.turn90(GET_RANDOM_BOOL());
            }
            randomWalkFsmState = RANDOM_WALK_TURN_WAIT_STATE;
            break;
        case RANDOM_WALK_TURN_WAIT_STATE:
            if (Vehicle.getState() != TURNING) {
                Vehicle.moveForward();
                mainFsmState = STEADY_STATE;
                fsmStartTime = millis();
            }
            break;
    }
}

void VehicleFsm::StateCheck() {
    tickTime = DEFAULT_TICK_TIME_MS;
    if (mainFsmState == STEADY_STATE) {
        if (LineDetector.pollLine() == LINE && mainFsmState != LINE_STATE) {
            mainFsmState = LINE_STATE;
            lineFsmState = LINE_STOP_STATE;
        } else if (mainFsmState != OBSTACLE_STATE) {
            WarningState_t obstacleState = DistanceSensor.pollSensors();
            if (obstacleState == TRIGGER_HANDLER) {
                mainFsmState = OBSTACLE_STATE;
                obstacleFsmState = OBSTACLE_STOP_STATE;
            } else if (obstacleState == WARNING) {
                tickTime = WARNING_TICK_TIME_MS;
            } else if (((millis() - fsmStartTime)/1000) >= RANDOM_WALK_CHANGE_DIR_TIME_S*5) {
            mainFsmState = RANDOM_WALK_STATE;
            randomWalkFsmState = RANDOM_WALK_STOP_STATE;
        }
        }
    }
}