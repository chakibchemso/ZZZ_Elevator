#pragma once
#include <Arduino.h>

// Buzzer class for non-blocking 1s beeps
class Buzzer
{
public:
    // durationMs: beep length in milliseconds
    Buzzer(uint8_t pin)
        : _pin(pin), _active(false), _startTime(0), _duration(1000) {}

    void setup()
    {
        pinMode(_pin, OUTPUT);
        digitalWrite(_pin, LOW);
    }

    void beep(unsigned long durationMs = 1000)
    {
        digitalWrite(_pin, HIGH);
        _active = true;
        _startTime = millis();
        _duration = durationMs;
    }

    void loop()
    {
        if (_active && (millis() - _startTime >= _duration))
        {
            digitalWrite(_pin, LOW);
            _active = false;
        }
    }

private:
    uint8_t _pin;
    bool _active;
    unsigned long _startTime;
    unsigned long _duration;
};