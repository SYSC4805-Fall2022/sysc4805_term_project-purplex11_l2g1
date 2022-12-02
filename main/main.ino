//#define FULL_INTEGRATION_TEST
#define SKIP_FIRST_LINE_TEST
#define OBSTACLE_DETECTION_TEST
#define LINE_DETECTION_TEST
#define RANDOM_WALK_TEST
#define WHEEL_ENCODER_TEST
#include "MainFsm.h"

void setup() {
  MainVehicleFsm.MainFsmInit();
  MainVehicleFsm.MainFsmStart();
}

void loop() {
}