#include "startUp.h"

int State = 0;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  BootUp();

  Scheduler.AddHandler(IncrementState,10000);
}

void loop() {
  // put your main code here, to run repeatedly:
  switch(State)
  {
    case 0:
      Motors.MoveForward();
      delay(100);
      if (Motors.GetSpeedLeft() != 0 && Motors.GetSpeedRight() != 0)
      {
        Serial.println("Move Forward Test: PASS");
      }
      else
      {
        Serial.println("Move Forward Test: FAIL");
      }
      Motors.Stop();
      break;

    case 1:
      Motors.MoveReverse();
      delay(100);
      if (Motors.GetSpeedLeft() != 0 && Motors.GetSpeedRight() != 0)
      {
        Serial.println("Move Reverse Test: PASS");
      }
      else
      {
        Serial.println("Move Reverse Test: FAIL");
      }
      Motors.Stop();
      break;

    case 2:
      Motors.MoveForward();
      delay(1000);
      Motors.Stop();
      delay(200);
      if (GetSpeed() == 0)
      {
        Serial.println("Stop Test: PASS");
      }
      else
      {
        Serial.println("Stop Test: FAIL");
      }
      break;

    case 3:
      Motors.MoveReverseLeft();
      delay(100);
      if (Motors.GetSpeedLeft() != 0 && Motors.GetSpeedRight() == 0)
      {
        Serial.println("Move Reverse Left Test: PASS");
      }
      else
      {
        Serial.println("Move Reverse Left Test: FAIL");
      }
      Motors.Stop();
      break;

    case 4:
      Motors.MoveForwardLeft();
      delay(100);
      if (Motors.GetSpeedLeft() != 0 && Motors.GetSpeedRight() == 0)
      {
        Serial.println("Move Forward Left Test: PASS");
      }
      else
      {
        Serial.println("Move Forward Left Test: FAIL");
      }
      Motors.Stop();
      break;

    case 5:
      Motors.MoveReverseRight();
      delay(100);
      if (Motors.GetSpeedRight() != 0 && Motors.GetSpeedLeft() == 0)
      {
        Serial.println("Move Reverse Right Test: PASS");
      }
      else
      {
        Serial.println("Move Reverse Right Test: FAIL");
      }
      Motors.Stop();
      break;

    case 6:
      Motors.MoveForwardRight();
      delay(100);
      if (Motors.GetSpeedRight() != 0 && Motors.GetSpeedLeft() == 0)
      {
        Serial.println("Move Forward Right Test: PASS");
      }
      else
      {
        Serial.println("Move Forward Right Test: FAIL");
      }
      Motors.Stop();
      break;

    case 7:
      Turn(DEGREE_180, TRUE);
      delay(1000);
      if (Motors.GetSpeedLeft() != 0 && Motors.GetSpeedRight() == 0)
      {
        Serial.println("Turn Right Test: PASS");
      }
      else
      {
        Serial.println("Turn Right Test: FAIL");
      }
      stop();
      break;
    
    case 8:
      Turn(DEGREE_180, FALSE);
      delay(1000);
      if (Motors.GetSpeedLeft() == 0 && Motors.GetSpeedRight() != 0)
      {
        Serial.println("Turn Left Test: PASS");
      }
      else
      {
        Serial.println("Turn Left Test: FAIL");
      }
      stop();
      break;
  }
}

void IncrementState()
{
  State++;
}