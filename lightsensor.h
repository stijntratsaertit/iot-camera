#pragma once

#include <iostream>
#include <wiringPiI2C.h>
#include <thread>
#include <chrono>

#define VISIBLE_LIGHT (0)
#define INFRARED_LIGHT (1)
#define FULLSPECTRUM_LIGHT (2)

#define DEVICE_ID (0x29)
#define SENSOR_COMMAND (0xA0)
#define SENSOR_ENABLE_POWERON (0x01)
#define SENSOR_ENABLE_POWEROFF (0x00)
#define SENSOR_ENABLE_AEN (0x10)

enum 
{
    SENSOR_REGISTER_ENABLE = 0x00,
    SENSOR_REGISTER_C0DATAL = 0x14,
    SENSOR_REGISTER_C0DATAH = 0x15,
    SENSOR_REGISTER_C1DATAL = 0x16,
    SENSOR_REGISTER_C1DATAH = 0x17
};

class Lightsensor
{
public:
    Lightsensor(int32_t sensorId = -1);
    uint32_t getLuminosity();
    uint32_t calcLux(uint16_t ch0, uint16_t ch1);

private:
    uint16_t read16(uint8_t reg);
    bool init();
    void start();
    void stop();

    int32_t _sensorId;
    bool _initialized;
    int _i2cId;
};
