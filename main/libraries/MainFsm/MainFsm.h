#ifndef MAIN_FSM_H
#define MAIN_FSM_H

#include "lineFollower.h"
#include "motor.h"
#include "ultrasonicSensor.h"
#include <stdint.h>
#include "arduino.h"
#include <DueTimer.h>
#include "wheelEncoder.h"

//#define SKIP_FIRST_LINE

#define DEFAULT_TICK_TIME_MS              50
#define WARNING_TICK_TIME_MS              30

#define OBSTACLE_REVERSE_TIME_S           2
#define LINE_REVERSE_TIME_S               2

#define MAX_SPEED                         30

#define GET_RANDOM_BOOL()                 (rand()%2)

#define RANDOM_WALK_CHANGE_DIR_TIME_S     5

typedef enum {
    INIT_STATE          = 0,
    STEADY_STATE        = 1,
    OBSTACLE_STATE      = 2,
    LINE_STATE          = 3,
    RANDOM_WALK_STATE   = 4,
} FsmState_t;

typedef enum {
    OBSTACLE_STOP_STATE         = 0,
    OBSTACLE_REVERSE_STATE      = 1,
    OBSTACLE_REVERSE_WAIT_STATE = 2,
    OBSTACLE_TURN_STATE         = 3,
    OBSTACLE_TURN_WAIT_STATE    = 4,
} ObstacleFsmState_t;

typedef enum {
    LINE_STOP_STATE         = 0,
    LINE_REVERSE_STATE      = 1,
    LINE_REVERSE_WAIT_STATE = 2,
    LINE_TURN_STATE         = 3,
    LINE_TURN_WAIT_STATE    = 4,
} LineFsmState_t;

typedef enum {
    RANDOM_WALK_STOP_STATE       = 0,
    RANDOM_WALK_TURN_STATE       = 1,
    RANDOM_WALK_TURN_WAIT_STATE  = 2,
} RandomWalkFsmState_t;

class VehicleFsm {
public:
    FsmState_t              mainFsmState;
    ObstacleFsmState_t      obstacleFsmState;
    LineFsmState_t          lineFsmState;
    RandomWalkFsmState_t    randomWalkFsmState;
    uint16_t                tickTime;
    long                    reverseStartTime;
    long                    fsmStartTime;
#ifdef SKIP_FIRST_LINE
    bool                    isFirstLine;
    bool                    lineFound;
#endif

    /**************************************************
     * void MainFsmInit()
     * 
     * Method to handle the startup of the fsm. This 
     * method will only be called once before any code 
     * is run.
    **************************************************/
    void MainFsmInit();

    /**************************************************
     * void MainFsmStart()
     * 
     * Method to start the timer to begin the fsm.
    **************************************************/
    void MainFsmStart();

    /**************************************************
     * void ObstacleState()
     * 
     * Method to handle the obstacle detect state and 
     * is called on every MainFsmTick where the 
     * mainFsmState is equal to OBSTACLE_STATE.
    **************************************************/
    void ObstacleState();

    /**************************************************
     * void LineState()
     * 
     * Method to handle the line detect state and 
     * is called on every MainFsmTick where the 
     * mainFsmState is equal to LINE_STATE.
    **************************************************/
    void LineState();

    /**************************************************
     * void RandomWalkState()
     * 
     * Method to handle the random walk state and 
     * is called on every MainFsmTick where the 
     * mainFsmState is equal to RANDOM_WALK_STATE. 
     * In random walk state, the vehicle will randomly
     * choose between 45 degrees left, 45 degrees right,
     * 90 degrees left, and 90 degrees right.
    **************************************************/
    void RandomWalkState();

    /**************************************************
     * void StateCheck()
     * 
     * Method to handle to poll the line detector, the
     * distance sensors and check the time since last 
     * turn to potentially trigger random walk. The 
     * handling priority is line, obstacle, random walk.
    **************************************************/
    void StateCheck();
};

/**************************************************
 * void MainFsmTick()
 * 
 * Method to handle the a main fsm timer tick where
 * it will call the respective state handlers.
**************************************************/
void MainFsmTick();

extern VehicleFsm MainVehicleFsm;



#endif