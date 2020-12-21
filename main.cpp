
#include "lightsensor.h"
#include "wiringPi.h"

int main(void)
{

    LightSensor lightSensor(2591);

    while(true)
    {
        LightData data = lightSensor.getLightData();
                
        printf("Visible: %zu\n", data.visible);
        printf("Infrared: %zu\n", data.infrared);
        printf("Full Spectrum: %zu\n", data.fullSpectrum);
        printf("Lux: %zu\n", data.lux);

        delay(1000);
    }

    return 0;
}