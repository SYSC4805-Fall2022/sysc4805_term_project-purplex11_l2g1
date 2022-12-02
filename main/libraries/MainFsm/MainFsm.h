#ifndef MAIN_FSM_H
#define MAIN_FSM_H

#include "lineFollower.h"
#include "motor.h"
#include "ultrasonicSensor.h"
#include "miniMu.h"
#include <stdint.h>
#include "arduino.h"

#define DEFAULT_TICK_TIME_MS              50
#define WARNING_TICK_TIME_MS              10

#define OBSTACLE_REVERSE_TIME_S           3
#define LINE_REVERSE_TIME_S               1

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

    void MainFsmInit();

    void MainFsmStart();

    void MainFsmTick();

    void MainFsm();

    void ObstacleState();

    void LineState();

    void RandomWalkState();

    void StateCheck();
};

extern VehicleFsm MainVehicleFsm;


#endif