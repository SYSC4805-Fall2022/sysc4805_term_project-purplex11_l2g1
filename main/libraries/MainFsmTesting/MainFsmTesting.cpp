#include "MainFsmTesting.h"

VehicleFsm MainVehicleFsm;


void VehicleFsm::MainFsmInit() {
    /* Set the default states for the main fsm*/
    mainFsmState = INIT_STATE;
    obstacleFsmState = OBSTACLE_STOP_STATE;
    lineFsmState = LINE_STOP_STATE;

    /* Call the init functions for all other modules*/
    LineDetector.LineFollowerInit();
    DistanceSensor.UltrasonicSensorInit();
    encoders.WheelEncoderInit();
    Vehicle.motorInit();

    /* Set the default time between main fsm ticks */
    tickTime = DEFAULT_TICK_TIME_MS;

#ifdef SKIP_FIRST_LINE
    isFirstLine = true;
    lineFound = false;
#endif
}


void VehicleFsm::MainFsmStart() {
    /* Start up in steady state on first timer tick*/
    mainFsmState = STEADY_STATE; 

    /* Cache the time for potential random walk trigger*/
    fsmStartTime = millis();  

    /* Initialize Timer 7 with interupt */
    //Timer7.attachInterrupt(MainFsmTick).start(tickTime * 1000);

    while(true) {
        MainFsmTick();
        delay(tickTime);
    }
}


void MainFsmTick() {
    /* Perform the code associated with each main FSM state.
     * For all states (except STEADY_STATE), call the sub-fsm
     * function where the underlying functionality of the state
     * is located */
    switch (MainVehicleFsm.mainFsmState) {
        case STEADY_STATE:
            if (Vehicle.getState() != FORWARD) {
                Vehicle.moveForward();
#ifdef INCLUDE_WHEEL_ENCODER
                encoders.Enable(1000, 200);
#endif
            }
#ifdef INCLUDE_WHEEL_ENCODER
            /* Slow down if the speed exceeds threshold */
            if (encoders.GetSpeed() > MAX_SPEED) {
                Vehicle.slowDown();
                encoders.Enable(200, 200);
            }
#endif
            MainVehicleFsm.StateCheck();
            break;
        case OBSTACLE_STATE:
            MainVehicleFsm.ObstacleState();
            MainVehicleFsm.StateCheck();
            break;
        case LINE_STATE:
            MainVehicleFsm.LineState();
            MainVehicleFsm.StateCheck();
            break;
        case RANDOM_WALK_STATE:
            MainVehicleFsm.RandomWalkState();
            MainVehicleFsm.StateCheck();
            break;
    }
}

void VehicleFsm::ObstacleState() {
    /* Handle the sub-states when in OBSTACLE_STATE */
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
                fsmStartTime = millis();
                mainFsmState = STEADY_STATE;
            }
    }
}


void VehicleFsm::LineState() {
    /* Handle the sub-states when in LINE_STATE */
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
                fsmStartTime = millis();
                mainFsmState = STEADY_STATE;
            }
            break;
    }
}

void VehicleFsm::RandomWalkState() {
    /* Handle the sub-states when in RANDOM_WALK_STATE */
    switch (randomWalkFsmState) {
        case RANDOM_WALK_STOP_STATE:
            Vehicle.stop();
            randomWalkFsmState = RANDOM_WALK_TURN_STATE;
            break;
        case RANDOM_WALK_TURN_STATE:
            /* Get random booleans to create 4 possible
             * random walk options */
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
                fsmStartTime = millis();
                mainFsmState = STEADY_STATE;
            }
            break;
    }
}

void VehicleFsm::StateCheck() {
    uint16_t startTickTime = tickTime;
    tickTime = DEFAULT_TICK_TIME_MS;

    /* State change only allowed when in steady state */
    if (mainFsmState == STEADY_STATE) {
#ifdef INCLUDE_LINE_DETECT
        /* If line detected begin line reaction state */
        if (LineDetector.pollLine() == LINE) {
#ifdef INCLUDE_SKIP_FIRST_LINE
            /* Skip first line to enter the boundary*/
            if(!isFirstLine) {
                mainFsmState = LINE_STATE;
                lineFsmState = LINE_STOP_STATE;
            } else {
                lineFound = true;
            }
        } else if (lineFound && isFirstLine) {
            isFirstLine = false;    
        
#else
            mainFsmState = LINE_STATE;
            lineFsmState = LINE_STOP_STATE;
#endif
#endif
#ifdef INCLUDE_OBSTACLE_DETECT
#ifdef INCLUDE_LINE_DETECT
        } else{
#else
        if (true) {
#endif
            WarningState_t obstacleState = DistanceSensor.pollSensors();
            /* If distance in in the reaction required range, begin obstacle
             * reaction state */
            if (obstacleState == TRIGGER_HANDLER) {
                mainFsmState = OBSTACLE_STATE;
                obstacleFsmState = OBSTACLE_STOP_STATE;
            } else if (obstacleState == WARNING) {
                /* If distance is greater than the reaction required range but 
                 * is nearing the range, shorten the fsm time to poll sensors 
                 * more often. */
                tickTime = WARNING_TICK_TIME_MS;
            } 
#endif
#ifdef INCLUDE_RANDOM_WALK
#ifdef INCLUDE_OBSTACLE_DETECT
            else if (((millis() - fsmStartTime)/1000) >= RANDOM_WALK_CHANGE_DIR_TIME_S) {
#else
            if (((millis() - fsmStartTime)/1000) >= RANDOM_WALK_CHANGE_DIR_TIME_S) {
#endif
                /* Trigger random walk state since steady state time is greater than the threshold*/
                mainFsmState = RANDOM_WALK_STATE;
                randomWalkFsmState = RANDOM_WALK_STOP_STATE; 
            }
        }
#endif
    }

    /* If the ticktime has changed, restart the timer with the new tick time */
    if (startTickTime != tickTime) {
        Timer7.stop();
        Timer7.start(tickTime*1000);
    }
} 