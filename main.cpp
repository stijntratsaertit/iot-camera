
#include "light-sensor.h"
#include "movement-sensor.h"
#include "camera.h"

int main(void)
{
    int requestToRecordTimerCount = 1;

    LightSensor lightSensor(2591);
    MovementSensor movementSensor(4);
    Camera camera;

    while (true) 
    {
        // LightData data = lightSensor.getLightData();
        // bool isDetecting = movementSensor.isDetecting();

        // if (isDetecting || data.visible > 1000) 
        // {
        //     camera.recordAndSave();
        // }

        if (camera.requestToRecord() && requestToRecordTimerCount == 10)
        {
            camera.recordAndSave();
            requestToRecordTimerCount = 1;
        }

        delay(500);
        requestToRecordTimerCount++;
    }

    return 0;
}