// Created by: chakibchemso
// Created on: 2023-10-01

#include "FloorLevelSensor.hpp"
#include "MainMotor.hpp"

// Simple Dependency Injection

FloorLevelSensor level_sensor(0.0, 12.0, 24.0, 9, 10, 0.0);
MainMotor main_motor(level_sensor, 3, 4, 5, 6);

void setup()
{
  Serial.begin(9600);
  level_sensor.Setup();
  main_motor.Setup();
}

void loop()
{
  level_sensor.Loop();
  main_motor.Loop();

  {
    static unsigned long lastPrintTime = 0;
    if (millis() - lastPrintTime >= 1000)
    {
      lastPrintTime = millis();
      Serial.print("Cabinet Height: ");
      Serial.print(level_sensor.GetElevatorHeightCM());
      Serial.print(" cm, Floor: ");
      Serial.println(level_sensor.GetElevatorFloorFloat());
    }
  }
}
