#pragma once

#include <wiringPi.h>

class GasSensor
{
public:
    GasSensor(int pin);
    int get();

private:
    void init();

    int _pin;
};
