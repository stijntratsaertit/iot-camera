#include "main.h"

int main(void)
{
    int movementCount = 0;
    int gasCount = 0;
    int lightCount = 0;

    wiringPiSetup();

    LightSensor lightSensor(2591);
    MovementSensor movementSensor(4);
    Camera camera;
    GasSensor gasSensor(0);

    while (true) 
    {
        bool triggerCondition = movementCount >= 5 || lightCount >= 5 || gasCount >= 5;
        
        if (triggerCondition || camera.requestToRecord() == 0)
        {
            string reason = getReason(movementCount, gasCount, lightCount);
            camera.recordAndSave(reason);
            movementCount = 0;
            lightCount = 0;
            gasCount = 0;
            continue;
        }

        if (movementSensor.isDetecting() == 1) movementCount++;
        else movementCount = 0;

        if (gasSensor.get() == 0) gasCount++;
        else gasCount = 0;

        if (lightSensor.getLightData().visible > 150) lightCount++;
        else lightCount = 0;

        delay(1000);
    }

    return 0;
}

string getReason(int movementCounter, int gasCounter, int lightCounter)
{
    if (movementCounter >= 5) 
        return "movement";
    if (gasCounter >= 5)
        return "gas";
    if (lightCounter >= 5)
        return "light";
    return "request";
}