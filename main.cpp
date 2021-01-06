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
            printTrigger(movementCount, gasCount, lightCount);
            camera.recordAndSave();
            movementCount = 0;
            lightCount = 0;
            gasCount = 0;
            continue;
        }

        if (movementSensor.isDetecting()) movementCount++;
        else movementCount = 0;

        if (!gasSensor.get()) gasCount++;
        else gasCount = 0;

        if (lightSensor.getLightData().visible > 150) lightCount++;
        else lightCount = 0;

        delay(1000);
    }

    return 0;
}

void printTrigger(int movementCounter, int gasCounter, int lightCounter)
{
    if (movementCounter >= 5) 
        std::cout << "Camera triggered by movement.\n";
        return;
    if (gasCounter >= 5)
        std::cout << "Camera triggered by gas.\n";
        return;
    if (lightCounter >= 5)
        std::cout << "Camera triggered by light.\n";
        return;
    std::cout << "Camera triggered by request.\n";
}