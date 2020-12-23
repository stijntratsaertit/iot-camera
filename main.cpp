
#include "light-sensor.h"
#include "movement-sensor.h"

int main(void)
{

    LightSensor lightSensor(2591);
    MovementSensor movementSensor(4);

    while(true)
    {
        LightData data = lightSensor.getLightData();
        bool isDetecting = movementSensor.isDetecting();
        
        printf("Is detecting: %d\n", isDetecting);
        printf("Lux: %zu\n", data.lux);

        delay(1000);
    }

    return 0;
}