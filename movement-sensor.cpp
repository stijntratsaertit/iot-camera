#include "movement-sensor.h"

MovementSensor::MovementSensor(int pin)
{
    _pin = pin;

    init();
}

void MovementSensor::init()
{
    wiringPiSetup();

    pinMode(_pin, INPUT);
}

bool MovementSensor::isDetecting()
{
    int val = digitalRead(_pin);

    return val == 1;
}
