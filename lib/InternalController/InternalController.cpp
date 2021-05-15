#include <Arduino.h>
#include "InternalController.h"
#include <ESP32Servo.h>

int minUs = 1000;
int maxUs = 2100;

Servo servo1;

void setupInternalController()
{
    ESP32PWM::allocateTimer(0);
    servo1.setPeriodHertz(50);
    servo1.attach(STEARING_PULSE_PIN, minUs, maxUs);
}

void setStearingAngle(int angle)
{
    servo1.write(angle);
}

void goForward() {}

void goBack() {}