#pragma once

#include <wiringPi.h>

class MovementSensor
{
public:
    MovementSensor(int pin);
    bool isDetecting();

private:
    void init();

    int _pin;
};
