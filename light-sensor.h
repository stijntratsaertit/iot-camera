#pragma once

#include <iostream>
#include <wiringPiI2C.h>
#include <thread>
#include <chrono>

#define DEVICE_ID (0x29)
#define SENSOR_COMMAND (0xA0)
#define SENSOR_ENABLE_POWERON (0x01)
#define SENSOR_ENABLE_POWEROFF (0x00)
#define SENSOR_ENABLE_AEN (0x02)

#define SENSOR_LUX_DF (408.0F)
#define SENSOR_LUX_COEFB (1.64F)
#define SENSOR_LUX_COEFC (0.59F)
#define SENSOR_LUX_COEFD (0.86F)

enum
{
    SENSOR_REGISTER_ENABLE = 0x00,
    SENSOR_REGISTER_C0DATAL = 0x14,
    SENSOR_REGISTER_C0DATAH = 0x15,
    SENSOR_REGISTER_C1DATAL = 0x16,
    SENSOR_REGISTER_C1DATAH = 0x17
};

struct LightData
{
    uint16_t visible;
    uint16_t infrared;
    uint16_t fullSpectrum;
    uint32_t lux;
};

class LightSensor
{
public:
    LightSensor(int32_t sensorId = -1);
    LightData getLightData();

private:
    uint32_t getLuminosity();
    uint32_t calcLux(uint16_t ch0, uint16_t ch1);
    bool init();
    void start();
    void stop();

    int32_t _sensorId;
    bool _initialized;
    int _i2cId;
};
