#pragma once

class FloorLevelSensor
{
public:
    // Constructor now accepts floor heights for ground, one, two, ultrasonic sensor pins, and initial cabinetHeight.
    FloorLevelSensor(float floorGroundHeight, float floorOneHeight, float floorTwoHeight, int triggerPin, int echoPin, float cabinetHeight)
        : floorGroundHeight(floorGroundHeight), floorOneHeight(floorOneHeight), floorTwoHeight(floorTwoHeight),
          cabinetHeightCM(cabinetHeight), triggerPin(triggerPin), echoPin(echoPin)
    {
    }
    ~FloorLevelSensor() = default;

    // Hardware setup: configure ultrasonic sensor pins
    void Setup()
    {
        pinMode(triggerPin, OUTPUT);
        pinMode(echoPin, INPUT);
        // ...any additional setup code...
    }

    // Loop: trigger ultrasound sensor, capture echo, compute distance.
    void Loop()
    {
        // Trigger the ultrasound pulse
        digitalWrite(triggerPin, LOW);
        delayMicroseconds(2);
        digitalWrite(triggerPin, HIGH);
        delayMicroseconds(10);
        digitalWrite(triggerPin, LOW);
        // Measure duration of echo
        unsigned long duration = pulseIn(echoPin, HIGH);
        // Calculate distance in cm (sound speed = 0.034 cm/us) and update cabinetHeight
        cabinetHeightCM = (duration * 0.034) / 2;
    }

    // Returns the cabinet height in centimeters
    float GetElevatorHeightCM()
    {
        return cabinetHeightCM;
    }

    // Returns the current floor as a float (2.x: top, 1.x: middle, 0.x: ground, x: %)
    float GetElevatorFloorFloat()
    {
        return cabinetHeightCM / floorTwoHeight;
    }

    // Returns the current floor as an integer (2: top, 1: middle, 0: ground)
    int GetElevatorFloorInt()
    {
        if (cabinetHeightCM < floorTwoHeight)
            return 2;
        else if (cabinetHeightCM < floorOneHeight)
            return 1;
        else
            return 0;
    }

private:
    float cabinetHeightCM;   // measured cabinet height (ultrasound sensor result)
    float floorGroundHeight; // height for ground floor
    float floorOneHeight;    // height for first floor
    float floorTwoHeight;    // height for second (top) floor
    int triggerPin;
    int echoPin;
};
