#include "gas-sensor.h"

using namespace std;

GasSensor::GasSensor(int pin)
{
    _pin = pin;

    init();
}

void GasSensor::init()
{
    pinMode(_pin, INPUT);
}

int GasSensor::get()
{
    return digitalRead(_pin);
}
