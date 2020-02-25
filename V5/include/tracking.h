#ifndef _TRACKING_H_
#define _TRACKING_H_

#include "tracking.h"
#include "main.h"
#include "chassis.h"
#include <math.h> 
#define _USE_MATH_DEFINES
#define DRIVE_DEGREE_TO_INCH (M_PI*4/900) //cm per tick
#define TRACKING_WHEEL_DEGREE_TO_INCH (M_PI*3.25/360)

/*
	Tracks encoder positions; No parameters
*/
void tracking(void* param);

#endif
