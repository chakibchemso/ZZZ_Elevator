#pragma once
#include <Arduino.h>
#include <LiquidCrystal_I2C.h>

// Display class for 20x4 LCD in elevator project
class Display
{
public:
    // LCD I2C address and dimensions are usually 0x27, 20 cols, 4 rows
    Display(uint8_t lcdAddr = 0x27, uint8_t cols = 20, uint8_t rows = 4)
        : lcd(lcdAddr, cols, rows) {}

    void begin()
    {
        lcd.init();
        lcd.backlight();
        lcd.clear();
    }

    // Show current floor, direction (▲/▼), and door status on row 0
    void showFloor(uint8_t floor, int8_t direction, bool doorOpen)
    {
        lcd.setCursor(0, 0);
        lcd.print("Floor: ");
        lcd.print(floor);
        lcd.print("   ");
        if (direction > 0)
            lcd.print("▲");
        else if (direction < 0)
            lcd.print("▼");
        else
            lcd.print(" ");
        lcd.print("  Door: ");
        lcd.print(doorOpen ? "Open " : "Closed");
        lcd.print("   "); // clear trailing chars
    }

    // Show status message on row 1
    void showStatus(const char *status)
    {
        lcd.setCursor(0, 1);
        lcd.print("Status: ");
        lcd.print(status);
        int len = strlen(status);
        for (int i = 0; i < 20 - 8 - len; ++i)
            lcd.print(' ');
    }

    // Show pending calls/requests on row 2
    void showCalls(const char *calls)
    {
        lcd.setCursor(0, 2);
        lcd.print("Calls: ");
        lcd.print(calls);
        int len = strlen(calls);
        for (int i = 0; i < 20 - 7 - len; ++i)
            lcd.print(' ');
    }

    // Show warning or emergency message on row 3
    void showWarning(const char *msg)
    {
        lcd.setCursor(0, 3);
        lcd.print(msg);
        int len = strlen(msg);
        for (int i = 0; i < 20 - len; ++i)
            lcd.print(' ');
    }

    // Optionally, clear a specific row
    void clearRow(uint8_t row)
    {
        lcd.setCursor(0, row);
        for (int i = 0; i < 20; ++i)
            lcd.print(' ');
    }

private:
    LiquidCrystal_I2C lcd;
};
