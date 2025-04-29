#pragma once

#include <stdint.h>
#include <Arduino.h>

// Button macros (bit positions)
#define BTN_IN0 0
#define BTN_IN1 1
#define BTN_IN2 2
#define BTN_EX0 3
#define BTN_EX1 4
#define BTN_EX2 5
#define BTN_EMS 6

class InputProvider
{
public:
    // buttonPins: array of 7 pin numbers for each button
    // debounceMs: debounce interval in ms
    // scanIntervalMs: minimum interval between scans (ms)
    InputProvider(const uint8_t buttonPins[7], uint16_t debounceMs = 30, uint16_t scanIntervalMs = 15)
        : _debounceMs(debounceMs), _scanIntervalMs(scanIntervalMs), _buttonStates(0), _lastRawStates(0), _lastScanTime(0), _lastDebounceTime{0}
    {
        for (int i = 0; i < 7; ++i)
        {
            _buttonPins[i] = buttonPins[i];
            pinMode(_buttonPins[i], INPUT); // Use INPUT_PULLUP for internal pull-up
            _lastDebounceTime[i] = 0;
        }
    }

    // Call periodically to update button states
    void loop()
    {
        unsigned long now = millis();
        if (now - _lastScanTime < _scanIntervalMs)
            return;
        _lastScanTime = now;

        uint8_t newRawStates = 0;
        for (uint8_t i = 0; i < 7; ++i)
        {
            if (digitalRead(_buttonPins[i]) == LOW) // pressed = LOW (INPUT_PULLUP)
            {
                newRawStates |= (1 << i);
            }
        }

        for (uint8_t i = 0; i < 7; ++i)
        {
            bool rawPressed = (newRawStates >> i) & 0x01;
            bool lastPressed = (_lastRawStates >> i) & 0x01;
            if (rawPressed != lastPressed)
            {
                _lastDebounceTime[i] = now;
            }
            if ((now - _lastDebounceTime[i]) > _debounceMs)
            {
                if (rawPressed)
                {
                    _buttonStates |= (1 << i);
                }
                else
                {
                    _buttonStates &= ~(1 << i);
                }
            }
        }
        _lastRawStates = newRawStates;
    }

    // Returns true if the button (macro) is pressed
    bool isPressed(uint8_t buttonMacro) const
    {
        return (_buttonStates >> buttonMacro) & 0x01;
    }

private:
    uint8_t _buttonPins[7];
    uint16_t _debounceMs;
    uint16_t _scanIntervalMs;
    uint8_t _buttonStates;  // bitmask for 7 buttons
    uint8_t _lastRawStates; // last raw read
    unsigned long _lastScanTime;
    unsigned long _lastDebounceTime[7];
};