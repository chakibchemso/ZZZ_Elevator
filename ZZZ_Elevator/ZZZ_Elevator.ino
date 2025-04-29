// Created by: chakibchemso
// Created on: 2025-04-18

#include <Arduino.h>
#include <Wire.h>
#include "Pins.hpp"
#include "InputProvider.hpp"
#include "FloorLevelSensor.hpp"
#include "MainMotor.hpp"
#include "Display.hpp"

// Simple Dependency Injection
uint8_t buttonPins[7] = {BUTTONINT0_PIN, BUTTONINT1_PIN, BUTTONINT2_PIN,
                         BUTTONEXT0_PIN, BUTTONEXT1_PIN, BUTTONEXT2_PIN,
                         BUTTONEMS_PIN};
InputProvider input_provider(buttonPins);

SR04Filter sr04_filter(TRIG_PIN, ECHO_PIN);

float levelZeroThreshold = 5.0f;
float levelOneThreshold = 10.0f;
float levelTwoThreshold = 15.0f;
FloorLevelSensor level_sensor(sr04_filter, levelZeroThreshold, levelOneThreshold, levelTwoThreshold);

MainMotor main_motor(MOTOR_PIN);

Display display;

// Main Program

void f1()
{
  enum State
  {
    IDLE,
    TO_CALLER,
    WAIT_INTERNAL,
    TO_TARGET,
    PAUSE_AT_TARGET,
    RETURNING
  };
  static State state = IDLE;
  static int callerFloor = -1, targetFloor = -1;
  static unsigned long pauseTimer = 0;
  static State lastState = IDLE;
  const int MOTOR_SPEED = 10;

  int currentFloor = -1;
  if (level_sensor.isLevelZero())
    currentFloor = 0;
  else if (level_sensor.isLevelOne())
    currentFloor = 1;
  else if (level_sensor.isLevelTwo())
    currentFloor = 2;

  switch (state)
  {
  case IDLE:
    if (input_provider.isPressed(BTN_EX0))
    {
      callerFloor = 0;
      state = TO_CALLER;
    }
    else if (input_provider.isPressed(BTN_EX1))
    {
      callerFloor = 1;
      state = TO_CALLER;
    }
    else if (input_provider.isPressed(BTN_EX2))
    {
      callerFloor = 2;
      state = TO_CALLER;
    }
    if (state == TO_CALLER)
    {
      main_motor.setSpeed(MOTOR_SPEED);
      if (callerFloor > currentFloor)
        main_motor.goUp();
      else if (callerFloor < currentFloor)
        main_motor.goDown();
      else
      {
        main_motor.stop();
        state = WAIT_INTERNAL;
      }
    }
    break;
  case TO_CALLER:
    if (currentFloor == callerFloor)
    {
      main_motor.stop();
      state = WAIT_INTERNAL;
    }
    break;
  case WAIT_INTERNAL:
    if (input_provider.isPressed(BTN_IN0))
    {
      targetFloor = 0;
      state = TO_TARGET;
    }
    else if (input_provider.isPressed(BTN_IN1))
    {
      targetFloor = 1;
      state = TO_TARGET;
    }
    else if (input_provider.isPressed(BTN_IN2))
    {
      targetFloor = 2;
      state = TO_TARGET;
    }
    break;
  case TO_TARGET:
    if (currentFloor < targetFloor)
    {
      main_motor.setSpeed(MOTOR_SPEED);
      main_motor.goUp();
    }
    else if (currentFloor > targetFloor)
    {
      main_motor.setSpeed(MOTOR_SPEED);
      main_motor.goDown();
    }
    else
    {
      main_motor.stop();
      state = PAUSE_AT_TARGET;
      pauseTimer = millis();
    }
    break;
  case PAUSE_AT_TARGET:
    if (millis() - pauseTimer >= 5000UL)
    {
      state = RETURNING;
    }
    break;
  case RETURNING:
    if (currentFloor > 0)
    {
      main_motor.setSpeed(MOTOR_SPEED);
      main_motor.goDown();
    }
    else
    {
      main_motor.stop();
      state = IDLE;
    }
    break;
  }

  // Debug: log state transitions
  if (state != lastState)
  {
    switch (state)
    {
    case TO_CALLER:
      Serial.print("External call: moving to floor ");
      Serial.println(callerFloor);
      break;
    case WAIT_INTERNAL:
      Serial.println("Arrived at caller, waiting for internal selection");
      break;
    case TO_TARGET:
      Serial.print("Internal call: moving to floor ");
      Serial.println(targetFloor);
      break;
    case PAUSE_AT_TARGET:
      Serial.println("Arrived at target, pausing 5s");
      break;
    case RETURNING:
      Serial.println("Returning to floor 0");
      break;
    case IDLE:
      Serial.println("Idle at floor 0");
      break;
    }
    lastState = state;
  }

  // drive status LEDs
  if (state == IDLE)
  {
    digitalWrite(LEDBUSY_PIN, LOW);
    digitalWrite(LEDOPEN_PIN, HIGH);
  }
  else
  {
    digitalWrite(LEDBUSY_PIN, HIGH);
    digitalWrite(LEDOPEN_PIN, LOW);
  }

  display.showFloor(currentFloor >= 0 ? currentFloor : 0, 0, false);
  display.showStatus(state == IDLE ? "Idle" : "Busy");
  {
    char heightBuf[20];
    sprintf(heightBuf, "Ht:%.1f cm", sr04_filter.getFiltered());
    display.showWarning(heightBuf);
  }
}

void setup()
{
  Wire.begin(SDA, SCL); // SDA, SCL
  Serial.begin(115200);
  sr04_filter.setup();
  main_motor.setup();
  display.begin();
  pinMode(LEDBUSY_PIN, OUTPUT);
  pinMode(LEDOPEN_PIN, OUTPUT);
  digitalWrite(LEDBUSY_PIN, LOW);
  digitalWrite(LEDOPEN_PIN, HIGH);
  Serial.println("ZZZ Elevator Control System Initialized");
}

void loop()
{
  input_provider.loop();
  sr04_filter.loop();
  f1();
}
