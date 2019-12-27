#ifndef _TRACKING_H_
#define _TRACKING_H_

#include "tracking.h"
#include "main.h"
#include "chassis.h"
#include <math.h> 
#define _USE_MATH_DEFINES
#define DEGREE_TO_CM (M_PI*8.255/360) //cm per tick

/*
	Tracks encoder positions; No parameters
*/
void tracking(void* param);

#endif
