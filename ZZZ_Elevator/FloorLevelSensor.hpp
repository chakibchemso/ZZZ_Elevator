#pragma once
#include <Arduino.h>

class FloorLevelSensor
{
public:
    FloorLevelSensor(uint8_t pinZero, uint8_t pinOne, uint8_t pinTwo)
        : pinZero(pinZero), pinOne(pinOne), pinTwo(pinTwo)
    {
        pinMode(pinZero, INPUT);
        pinMode(pinOne, INPUT);
        pinMode(pinTwo, INPUT);
    }

    bool isLevelZero() const
    {
        return digitalRead(pinZero) == LOW;
    }

    bool isLevelOne() const
    {
        return digitalRead(pinOne) == LOW;
    }

    bool isLevelTwo() const
    {
        return digitalRead(pinTwo) == LOW;
    }

private:
    uint8_t pinZero;
    uint8_t pinOne;
    uint8_t pinTwo;
};