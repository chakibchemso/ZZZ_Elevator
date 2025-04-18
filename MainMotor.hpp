#pragma once
#include "FloorLevelSensor.hpp"
#include <Arduino.h>

class MainMotor
{
public:
    // Constructor accepts the floor sensor by reference and the stepper control pins (IN1-IN4)
    MainMotor(FloorLevelSensor &sensor, int in1, int in2, int in3, int in4)
        : floorSensor(sensor), busy(false), targetFloor(-1),
          IN1(in1), IN2(in2), IN3(in3), IN4(in4), direction(true), stepIndex(0)
    {
    }
    ~MainMotor() = default;

    void Setup()
    {
        pinMode(IN1, OUTPUT);
        pinMode(IN2, OUTPUT);
        pinMode(IN3, OUTPUT);
        pinMode(IN4, OUTPUT);
        stopMotor();
        RequestFloor(0);
    }

    // Request the elevator to go to a specific floor (0: ground, 1: middle, 2: top)
    bool RequestFloor(int floor)
    {
        if (busy)
            return false;
        targetFloor = floor;
        busy = true;
        int currentFloor = floorSensor.GetElevatorFloorInt();
        if (targetFloor > currentFloor)
            direction = true; // upward
        else if (targetFloor < currentFloor)
            direction = false; // downward
        else
        {
            busy = false;
            return true; // already at floor
        }
        return true;
    }

    // Loop: step the motor towards the target floor
    void Loop()
    {
        if (!busy)
            return;
        int currentFloor = floorSensor.GetElevatorFloorInt();
        if (currentFloor == targetFloor)
        {
            stopMotor();
            busy = false;
            targetFloor = -1;
        }
        else
        {
            stepMotor(direction);
            delayMicroseconds(1200); // ~500-1200us for 28BYJ-48
        }
    }

    void EmergencyStop()
    {
        stopMotor();
        busy = false;
        targetFloor = -1;
    }

    // Accessors for display
    bool isBusy() const { return busy; }
    int getTargetFloor() const { return targetFloor; }
    bool getDirection() const { return direction; }

private:
    // 8-step sequence for 28BYJ-48
    const uint8_t stepSequence[8][4] = {
        {1, 0, 0, 0},
        {1, 1, 0, 0},
        {0, 1, 0, 0},
        {0, 1, 1, 0},
        {0, 0, 1, 0},
        {0, 0, 1, 1},
        {0, 0, 0, 1},
        {1, 0, 0, 1}};

    void stepMotor(bool dir)
    {
        if (dir)
            stepIndex = (stepIndex + 1) % 8;
        else
            stepIndex = (stepIndex + 7) % 8;
        digitalWrite(IN1, stepSequence[stepIndex][0]);
        digitalWrite(IN2, stepSequence[stepIndex][1]);
        digitalWrite(IN3, stepSequence[stepIndex][2]);
        digitalWrite(IN4, stepSequence[stepIndex][3]);
    }

    void stopMotor()
    {
        digitalWrite(IN1, LOW);
        digitalWrite(IN2, LOW);
        digitalWrite(IN3, LOW);
        digitalWrite(IN4, LOW);
    }

    FloorLevelSensor &floorSensor;
    bool busy;
    int targetFloor;
    bool direction;
    int stepIndex;
    const int IN1, IN2, IN3, IN4;
};
