#pragma once
#include "SR04Filter.hpp"

class FloorLevelSensor
{
public:
    FloorLevelSensor(SR04Filter &sr04, float levelZeroThreshold, float levelOneThreshold, float levelTwoThreshold)
        : sr04Filter(sr04), zeroThresh(levelZeroThreshold), oneThresh(levelOneThreshold), twoThresh(levelTwoThreshold) {}

    bool isLevelZero() const
    {
        float height = getHeight();
        return height >= zeroThresh && height < oneThresh;
    }

    bool isLevelOne() const
    {
        float height = getHeight();
        return height >= oneThresh && height < twoThresh;
    }

    bool isLevelTwo() const
    {
        float height = getHeight();
        return height >= twoThresh;
    }

private:
    SR04Filter &sr04Filter;
    float zeroThresh;
    float oneThresh;
    float twoThresh;

    float getHeight() const
    {
        return sr04Filter.getFiltered();
    }
};