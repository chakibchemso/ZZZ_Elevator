#pragma once
#include <Arduino.h>
#include <ESP32Servo.h>

class MainMotor
{
public:
    MainMotor(int controlPin)
        : pin(controlPin), speed(0)
    {
    }
    ~MainMotor() = default;

    void setup()
    {
        servo.attach(pin);
        stop();
    }

    // s: -90 (full speed one way) to +90 (full speed other way), 0 = stop
    void setSpeed(int s)
    {
        speed = constrain(abs(s), 0, 90);
        speed = map(speed, -90, 90, 0, 180);
    }

    void goUp()
    {
        servo.write(speed);
    }

    void goDown()
    {
        servo.write(-speed);
    }

    void stop()
    {
        servo.write(90);
    }

private:
    Servo servo;
    const int pin;
    int speed = 0; // default to 0
};
