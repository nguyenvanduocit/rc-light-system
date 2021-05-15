#include <Arduino.h>
#include <Wire.h>
#include "Light.h"
#include "InternalController.h"
#include "FlutterBlue.h"
void setup()
{
    Serial.begin(115200);
    delay(10);
    setupLight();
    setupInternalController();

    setupFlutterBlue();
}

void loop()
{
}