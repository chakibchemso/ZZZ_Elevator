#pragma once
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "FloorLevelSensor.hpp"
#include "MainMotor.hpp"

class Display
{
public:
    Display()
        : lcd(0x27, 20, 4) // 0x27 is a common I2C address for 2004A LCD
    {
    }

    void Setup()
    {
        lcd.init();
        lcd.backlight();
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("PLC Elevator Status ");
    }

    void Update(const FloorLevelSensor &sensor, const MainMotor &motor)
    {
        // Line 1: fixed
        lcd.setCursor(0, 0);
        lcd.print("PLC Elevator Status ");
        // Line 2: Floor and Height
        lcd.setCursor(0, 1);
        lcd.print("Floor: ");
        lcd.print(sensor.GetElevatorFloorInt());
        lcd.print("  Height: ");
        lcd.print(sensor.GetElevatorHeightCM(), 1);
        lcd.print("cm   ");
        // Line 3: Motor state
        lcd.setCursor(0, 2);
        lcd.print("Motor: ");
        lcd.print(motor.isBusy() ? "Running " : "Stopped ");
        lcd.print("Dir: ");
        lcd.print(motor.getDirection() ? "Up  " : "Down");
        // Line 4: Target and Busy
        lcd.setCursor(0, 3);
        lcd.print("Target: ");
        lcd.print(motor.getTargetFloor());
        lcd.print("  ");
        lcd.print(motor.isBusy() ? "Busy " : "Idle ");
        lcd.print("        ");
    }

private:
    LiquidCrystal_I2C lcd;
};
