#ifndef VISION_SENSOR_H
#define VISION_SENSOR_H

#include "main.h"

#define PURPLE_SIG 1
#define PURPLE_SIG2 2

//static pros::vision_signature_s_t PURPLE[3];

static pros::vision_signature_s_t purple;
static pros::Vision andyVision (VISION_PORT);

static int x = 5;

#endif
