
#include <Arduino.h>
#include "Adafruit_MCP23017.h"
#include "ExternalController.h"

uint8_t turningStage = TURNING_STAGE_IDLE;
uint8_t throttleStage = THROTTLE_STAGE_IDLE;

void checkThrottleSignal(void *parameter)
{
    const TickType_t xLoopDelay = 100 / portTICK_PERIOD_MS; //5ms
    Serial.print("CheckSignal is running on: ");
    Serial.println(xPortGetCoreID());

    unsigned long throttle = 1500;
    unsigned long idleThrottle = pulseIn(THROTTLE_PULSE_PIN, HIGH);

    for (;;)
    {
        throttle = pulseIn(THROTTLE_PULSE_PIN, HIGH);

        if (throttle > IDLE_PULSE_WIDTH + idleThrottle)
        {
            throttleStage = THROTTLE_STAGE_AHEAD;
        }
        else if (throttle < idleThrottle - IDLE_PULSE_WIDTH)
        {
            throttleStage = THROTTLE_STAGE_BACK;
        }
        else
        {
            throttleStage = THROTTLE_STAGE_IDLE;
        }

        vTaskDelay(xLoopDelay);
    }
}

void checkStearingSignal(void *parameter)
{
    const TickType_t xLoopDelay = 5 / portTICK_PERIOD_MS; //5ms
    Serial.print("CheckSignal is running on: ");
    Serial.println(xPortGetCoreID());

    unsigned long throttle = 1500;
    unsigned long idleThrottle = pulseIn(STEARING_PULSE_PIN, HIGH);

    for (;;)
    {
        throttle = pulseIn(THROTTLE_PULSE_PIN, HIGH);

        if (throttle > IDLE_PULSE_WIDTH + idleThrottle)
        {
            turningStage = TURNING_STAGE_LEFT;
        }
        else if (throttle < idleThrottle - IDLE_PULSE_WIDTH)
        {
            turningStage = TURNING_STAGE_RIGHT;
        }
        else
        {
            turningStage = TURNING_STAGE_IDLE;
        }

        vTaskDelay(xLoopDelay);
    }
}

void setupExternalController()
{
    pinMode(THROTTLE_PULSE_PIN, INPUT);
    pinMode(STEARING_PULSE_PIN, INPUT);
    xTaskCreatePinnedToCore(
        checkStearingSignal,         // Function that should be called
        "check turning signal task", // Name of the task (for debugging)
        1000,                        // Stack size (bytes)
        NULL,                        // Parameter to pass
        1,                           // Task priority
        NULL,                        // Task handle
        0                            // Core you want to run the task on (0 or 1)
    );

    xTaskCreatePinnedToCore(
        checkThrottleSignal,          // Function that should be called
        "check throttle signal task", // Name of the task (for debugging)
        1000,                         // Stack size (bytes)
        NULL,                         // Parameter to pass
        1,                            // Task priority
        NULL,                         // Task handle
        0                             // Core you want to run the task on (0 or 1)
    );
}