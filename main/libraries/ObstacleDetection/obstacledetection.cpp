#include <Wire.h>
#include "obstacledetection.h"
VL53L1X sensor;

ObstacleDetectors ObstacleDetector;

void ObstacleDetectors::ObstacleDetectorInit()
{
  while (!Serial) {}
  Serial.begin(115200);
  Wire.begin();
  Wire.setClock(400000); // use 400 kHz I2C

  sensor.setTimeout(500);
  if (!sensor.init())
  {
    Serial.println("Failed to detect and initialize sensor!");
    while (1);
  }

  // setting the modes for the sensor
  sensor.setDistanceMode(VL53L1X::Short);
  sensor.setMeasurementTimingBudget(50000);

  // Start continuous readings at a rate of one measurement every 50 ms
  sensor.startContinuous(50);

  Scheduler.AddHandler(ObstacleDetectionISR, 50);
}

void ObstacleDetectionISR(){
    sensor.read();
    if (sensor.ranging_data.range_mm < D_THRESHOLD_MM){
        Motors.Turn(DEGREE_90, FALSE);
        ObstacleDetector.obstacleDetectedState = CHECKING;

    }
    if (ObstacleDetector.obstacleDetectedState == TURNING_LEFT){
        if(!Motors.isTurning){
            Motors.MoveForward();
            ObstacleDetector.obstacleDetectedState = MOVING_STRAIGHT;
            ObstacleDetector.timerticks = 0;
        }

    }else if(ObstacleDetector.obstacleDetectedState == MOVING_STRAIGHT){
        if(ObstacleDetector.timerticks >= 5){
            Motors.Turn(DEGREE_90, TRUE);
            ObstacleDetector.obstacleDetectedState = TURNING_RIGHT;
            ObstacleDetector.timerticks = 0;
        }else{
            ObstacleDetector.timerticks++;
        }
    }else if(ObstacleDetector.obstacleDetectedState == TURNING_RIGHT){
        if(!Motors.isTurning){
            Motors.MoveForward();
            ObstacleDetector.obstacleDetectedState = CHECKING;
            ObstacleDetector.timerticks = 0;
        }
    }
}

