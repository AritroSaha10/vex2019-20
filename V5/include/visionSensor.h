#ifndef VISION_SENSOR_H
#define VISION_SENSOR_H

#include "main.h"

#define PURPLE_SIG 1

pros::vision_signature_s_t PURPLE_CUBE = pros::Vision::signature_from_utility(PURPLE_SIG, 2931, 3793, 3362, 5041, 6631, 5836, 4.800, 1);	
pros::Vision andyVision (VISION_PORT);

#endif
