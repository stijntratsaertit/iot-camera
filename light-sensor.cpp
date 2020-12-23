#include "light-sensor.h"

LightSensor::LightSensor(int32_t sensorId)
{
    _initialized = false;
    _sensorId = sensorId;
    _i2cId = 0;
}

bool LightSensor::init()
{
    if (_i2cId == 0)
    {
        std::cout << "Initialising I2C communication.\n";
        _i2cId = wiringPiI2CSetup(DEVICE_ID);
    }

    if (_i2cId == -1)
    {
        std::cout << "Failed to init I2C communication.\n";
        return false;
    }

    wiringPiI2CWriteReg8(_i2cId, SENSOR_REGISTER_ENABLE, SENSOR_ENABLE_POWERON | SENSOR_ENABLE_AEN);

    if (wiringPiI2CReadReg8(_i2cId, 0x80 | 0x20 | 0x12) == 0x50)
    {
        std::cout << "Successfully initialised I2C communication.\n";
    }

    _initialized = true;

    stop();

    return true;
}

void LightSensor::start()
{
    if (!_initialized)
    {
        if (!init())
        {
            return;
        }
    }
    wiringPiI2CWriteReg8(_i2cId, SENSOR_REGISTER_ENABLE, SENSOR_ENABLE_POWERON | SENSOR_ENABLE_AEN);
}

void LightSensor::stop()
{
    if (!_initialized)
    {
        if (!init())
        {
            return;
        }
    }
    wiringPiI2CWriteReg8(_i2cId, SENSOR_REGISTER_ENABLE, SENSOR_ENABLE_POWEROFF);
}

uint32_t LightSensor::getLuminosity()
{
    if (!_initialized)
    {
        if (!init())
        {
            return 0;
        }
    }
    start();

    uint32_t ch0data = wiringPiI2CReadReg16(_i2cId, SENSOR_COMMAND | SENSOR_REGISTER_C0DATAL);
    ch0data <<= 16;
    uint32_t ch1data = wiringPiI2CReadReg16(_i2cId, SENSOR_COMMAND | SENSOR_REGISTER_C1DATAL);
    ch0data |= ch1data;

    stop();

    return ch0data;
}

LightData LightSensor::getLightData()
{
    uint32_t lum = getLuminosity();
    uint16_t infrared = (lum & 0xFFFF);
    uint16_t fullSpectrum = (lum >> 16);
    uint16_t visible = fullSpectrum - infrared;

    return {
        visible,
        infrared,
        fullSpectrum,
        calcLux(fullSpectrum, infrared)};
}

uint32_t LightSensor::calcLux(uint16_t ch0, uint16_t ch1)
{
    uint16_t atime, again;
    float cpl, lux1, lux2, lux;
    uint32_t chan0, chan1;

    if ((ch0 == 0xFFFF) | (ch1 == 0xFFFF))
    {
        return 0;
    }
    cpl = 1000 / SENSOR_LUX_DF;

    lux1 = ((float)ch0 - (SENSOR_LUX_COEFB * (float)ch1)) / cpl;
    lux2 = ((SENSOR_LUX_COEFC * (float)ch0) - (SENSOR_LUX_COEFD * (float)ch1)) / cpl;

    return lux1 > lux2 ? lux1 : lux2;
}