// Created by: chakibchemso
// Created on: 2023-10-01

#include "MainMotor.hpp"
#include "FloorLevelSensor.hpp"

MainMotor main_motor;
FloorLevelSensor level_sensor;

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(9600);
  main_motor.init();
  level_sensor.init();
}

void loop()
{
  // put your main code here, to run repeatedly:
}
