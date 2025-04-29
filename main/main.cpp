#include <Arduino.h>
#include "../ZZZ_Elevator/ZZZ_Elevator.ino"

extern "C" void app_main()
{
    initArduino();
    setup();
    while (true)
    {
        loop();
    }
}