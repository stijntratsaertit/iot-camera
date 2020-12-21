

#include "lightsensor.h"

int main(void)
{
    Lightsensor lightsensor = Lightsensor(2591);

    int32_t lum = lightsensor.getLuminosity();

    uint16_t ch0 = (lum & 0xFFFF);
    uint16_t ch1 = (lum >> 16);

    // uint32_t lux = lightsensor.calcLux(ch0, ch1);

    printf("Full Spectrum: %zu\n", ch0);
    printf("Infrared: %zu\n", ch1);
    if ( ch0 >= ch1 )  printf("Visible: %zu\n", ch0-ch1);
    // printf("Lux: %zu\n", lux);

    return 0;
}

Lightsensor::Lightsensor(int32_t sensorId)
{
    _initialized = false;
    _sensorId = sensorId;
    _i2cId = 0;

    init();
}

bool Lightsensor::init()
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

    wiringPiI2CWriteReg8(_i2cId, SENSOR_COMMAND | SENSOR_REGISTER_ENABLE, SENSOR_ENABLE_POWERON | SENSOR_ENABLE_AEN);

    if (wiringPiI2CReadReg8(_i2cId, 0x80 | 0x20 | 0x12) == 0x50) 
    {
        std::cout << "Successfully initialised I2C communication.\n";
    }

    _initialized = true;

    stop();

    return true;
}

void Lightsensor::start()
{
    if (!_initialized)
    {
        if (!init())
        {
            return;
        }
    }
    wiringPiI2CWriteReg8(_i2cId, SENSOR_COMMAND | SENSOR_REGISTER_ENABLE, SENSOR_ENABLE_POWERON | SENSOR_ENABLE_AEN);
}

void Lightsensor::stop()
{
    if(!_initialized) 
    {
        if(!init())
        {
            return;
        }
    }
    wiringPiI2CWriteReg8(_i2cId, SENSOR_COMMAND | SENSOR_REGISTER_ENABLE, SENSOR_ENABLE_POWEROFF);
}

uint32_t Lightsensor::getLuminosity()
{
    if (!_initialized)
    {
        if (!init())
        {
            return 0;
        }
    }
    start();
    
    uint16_t ch0data = wiringPiI2CReadReg16(_i2cId, SENSOR_COMMAND | SENSOR_REGISTER_C0DATAL);
    ch0data <<= 16;

    uint16_t ch1data = wiringPiI2CReadReg16(_i2cId, SENSOR_COMMAND | SENSOR_REGISTER_C1DATAL);
    ch0data |= ch1data;

    stop();
    return ch0data;
}

uint32_t Lightsensor::calcLux(uint16_t ch0, uint16_t ch1)
{
    if ((ch0 == 0xFFFF) | (ch1 == 0xFFFF))
    {
        return 0;
    }

    return 0;

}