#ifndef LIGHT_H
#define LIGHT_H

#define frontMainLightLeft 9
#define frontMainLightRight 14
#define frontLeftYellowLight 2
#define frontRightYellowLight 4
#define frontLeftYellowLight2 1
#define frontRightYellowLight2 3
#define frontLeftRedLight 5
#define frontRightRedLight 7

#define rearLeftWhiteLight 12
#define rearRightWhiteLight 15
#define rearLeftYellowLight 6
#define rearRightYellowLight 13
#define rearLeftRedLight 11
#define rearRightRedLight 10

#define LIGHT_STAGE_OFF 1001
#define LIGHT_STAGE_ON 1002

#define innerLight 0

void setupLight();
void processLight(void *parameter);
void setLightStage(int stage);

#endif