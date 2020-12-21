
#include <iostream>
#include <wiringPiI2C.h>
#include <thread>
#include <chrono>

#define DEVICE_ID (0x29)
#define REG_COMMAND (0xA0)
#define REG_C0DATAL (0x14)
#define REG_C0DATAH (0x15)
#define REG_C1DATAL (0x16)
#define REG_C1DATAH (0x17)
#define REG_POWER (0x00)

int main(void) 
{
    int fd = wiringPiI2CSetup(DEVICE_ID);

    if (fd == -1)
    {
        std::cout << "Failed to init I2C communication.\n";
        return -1;
    }
    std::cout << "I2C successfully setup.\n";

    wiringPiI2CWriteReg8(fd, REG_POWER, 0b00000011);

    uint32_t c0d, c1d;
    uint16_t c0, c1;

    while (1)
    {
        c0d = wiringPiI2CReadReg16(fd, REG_COMMAND | REG_C0DATAL);
        c0d <<= 16;
        c1d = wiringPiI2CReadReg16(fd, REG_COMMAND | REG_C1DATAL);

        uint32_t res = c0d | c1d;

        c0 = (res & 0xFFFF);
        c1 = (res >> 16);
        std::cout << "c0: " << c0 << ", c1: " << c1 << "\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }

    return 0;
}