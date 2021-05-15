#include <Arduino.h>
#include "Adafruit_MCP23017.h"
#include "Light.h"

Adafruit_MCP23017 mcpp23017;
QueueHandle_t queue;

void setLightStage(int stage)
{
    xQueueSend(queue, &stage, portMAX_DELAY);
}

void setupLight()
{
    mcpp23017.begin();
    queue = xQueueCreate(10, sizeof(int));
    if (queue == NULL)
    {
        Serial.println("Error creating the queue");
    }
    for (int i = 0; i < 16; i++)
    {
        mcpp23017.pinMode(i, OUTPUT);
    }

    xTaskCreatePinnedToCore(
        processLight,         // Function that should be called
        "process light task", // Name of the task (for debugging)
        1000,                 // Stack size (bytes)
        NULL,                 // Parameter to pass
        1,                    // Task priority
        NULL,                 // Task handle
        1                     // Core you want to run the task on (0 or 1)
    );
}

void resetLight()
{

    mcpp23017.digitalWrite(innerLight, HIGH);

    mcpp23017.digitalWrite(frontMainLightLeft, HIGH);
    mcpp23017.digitalWrite(frontMainLightRight, HIGH);

    mcpp23017.digitalWrite(frontLeftRedLight, HIGH);
    mcpp23017.digitalWrite(frontRightRedLight, HIGH);

    mcpp23017.digitalWrite(frontLeftYellowLight, HIGH);
    mcpp23017.digitalWrite(frontRightYellowLight, HIGH);

    mcpp23017.digitalWrite(rearLeftYellowLight, HIGH);
    mcpp23017.digitalWrite(rearRightYellowLight, HIGH);

    mcpp23017.digitalWrite(rearLeftWhiteLight, HIGH);
    mcpp23017.digitalWrite(rearRightWhiteLight, HIGH);
}

void turnOffAllLights()
{
    mcpp23017.writeGPIOAB(LOW);
}

void processLight(void *parameter)
{
    Serial.print("ProcessLight is running on: ");
    Serial.println(xPortGetCoreID());

    int lightStage = 0;
    resetLight();

    for (;;)
    {
        xQueueReceive(queue, &lightStage, portMAX_DELAY);

        switch (lightStage)
        {
        case LIGHT_STAGE_ON:
            resetLight();
            break;
        case LIGHT_STAGE_OFF:
            turnOffAllLights();
            break;
        default:
            Serial.println(lightStage);
            break;
        }
    }
}