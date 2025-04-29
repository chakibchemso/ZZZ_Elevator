#pragma once
#include <Arduino.h>
#include <algorithm>

class SR04Filter
{
public:
    SR04Filter(int trigPin, int echoPin)
        : trigPin(trigPin), echoPin(echoPin), bufIndex(0), bufCount(0), ema(0.0f), alpha(0.3f) {}

    void setup()
    {
        pinMode(trigPin, OUTPUT);
        pinMode(echoPin, INPUT);
        memset(buffer, 0, sizeof(buffer));
        lastMeasure = millis();
    }

    // Call this as often as possible in loop()
    void loop()
    {
        unsigned long now = millis();
        if (now - lastMeasure < 5)
            return;
        lastMeasure = now;
        float dist = measure();
        buffer[bufIndex] = dist;
        bufIndex = (bufIndex + 1) % BUF_SIZE;
        if (bufCount < BUF_SIZE)
            bufCount++;
        applyFilters();
    }

    float getFiltered() const { return ema; }

private:
    static constexpr int BUF_SIZE = 10;
    static constexpr int TRIM = 2;
    int trigPin, echoPin;
    float buffer[BUF_SIZE];
    int bufIndex, bufCount;
    float ema;
    float alpha;
    unsigned long lastMeasure;

    float measure()
    {
        digitalWrite(trigPin, LOW);
        delayMicroseconds(2);
        digitalWrite(trigPin, HIGH);
        delayMicroseconds(10);
        digitalWrite(trigPin, LOW);
        unsigned long duration = pulseIn(echoPin, HIGH, 25000); // 25ms timeout
        float distance = (duration > 0) ? (duration * 0.034f) / 2.0f : 0.0f;
        return distance;
    }

    void applyFilters()
    {
        if (bufCount < BUF_SIZE)
            return; // wait for buffer to fill

        float temp[BUF_SIZE];
        memcpy(temp, buffer, sizeof(temp));
        std::sort(temp, temp + BUF_SIZE);

        float sum = 0.0f;
        int n = 0;
        for (int i = TRIM; i < BUF_SIZE - TRIM; ++i)
        {
            sum += temp[i];
            n++;
        }

        float avg = (n > 0) ? (sum / n) : 0.0f;
        ema = (alpha * avg) + (1.0f - alpha) * ema;
    }
};
