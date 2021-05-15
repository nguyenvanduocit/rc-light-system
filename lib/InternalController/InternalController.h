#ifndef INTERNAL_CONTROLLER_H
#define INTERNAL_CONTROLLER_H

#define THROTTLE_PULSE_PIN 27
#define STEARING_PULSE_PIN 26

void setupInternalController();
void setStearingAngle(int);
void goForward();
void goBack();
#endif