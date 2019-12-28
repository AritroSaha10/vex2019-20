#include "tracking.h"
#include "chassis.h"
#define _USE_MATH_DEFINES
#include <math.h>

	//cm per tick

float lDist, rDist, dist, lastTheta, x, y, rCycle, lCycle = 0;
float theta;
float changeInTheta;
float distOfWheels = 26.6; 		//cm
std::vector<double> encoders;

void tracking(void* param) {
	while(1) {
		
		encoders = getEncoders({FL, FR});
		lCycle = ((encoders[0])*DEGREE_TO_CM)-lDist;
		rCycle = ((encoders[1])*DEGREE_TO_CM)-rDist;
		
		
		dist = (rCycle+lCycle)/2;
		theta = (rCycle-lCycle)/distOfWheels; 
		changeInTheta = (theta - lastTheta);
		if (abs((theta)-(lastTheta)) >= 0.001) {
			x += dist*cos(theta);
			y += dist*sin(theta);
		}	

		theta += theta-lastTheta;
	
		pros::lcd::print(2, "x: %f, y: %f, theta: %f", x, y, theta);
		pros::lcd::print(3, "theta: %f, lastTheta: %f", theta, lastTheta);
		pros::lcd::print(4, "change in theta: %f", (theta-lastTheta));
		lastTheta = theta;
		rDist = encoders[0]*DEGREE_TO_CM;
		lDist = encoders[1]*DEGREE_TO_CM;
		pros::delay(2);
	}
}
