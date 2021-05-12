#include <Arduino.h>
#include <Wire.h>
#include "Adafruit_MCP23017.h"

Adafruit_MCP23017 mcpp23017;

void setup()
{
    Serial.begin(115200);
    delay(10);

    mcpp23017.begin();

    for (int i = 0; i < 16; i++)
    {
        mcpp23017.pinMode(i, OUTPUT);
    }
}

void loop()
{
    for (int i = 0; i < 16; i++)
    {
        mcpp23017.digitalWrite(i, HIGH);
    }

    delay(100);

    for (int i = 0; i < 16; i++)
    {
        mcpp23017.digitalWrite(i, LOW);
    }

    delay(100);
}