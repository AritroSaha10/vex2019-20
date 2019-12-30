#include "tracking.h"
#include "chassis.h"
#define _USE_MATH_DEFINES
#include <math.h>

//Globals
float x, y, angle;
//Sensors
float rDelta, lDelta, bDelta;
std::vector<double> encoders;
//Temp
float rDist, lDist, aDelta, rLast, lLast;
//constants
const float lrOffset = 13/2*2.54; //13 inch wheel base, halved and converted to cm
const float bOffset = 0; /*PLACEHOLDER*/

void tracking(void* param) {
	float localCoord[2];
	while(1) {
		localCoord[0] = 0;
		localCoord[1] = 0;
		std::vector<double> rawEncoders = getEncoders({FL, BL, FR, BR});
		encoders.push_back((rawEncoders[1] + rawEncoders[0])/2);
		encoders.push_back((rawEncoders[2] + rawEncoders[3])/2);

		lDelta = encoders[0]-lLast;
		rDelta = encoders[1]-rLast;
		lLast = encoders[0];
		rLast = encoders[1];
		lDist = lDelta * DEGREE_TO_CM;
		rDist = rDelta * DEGREE_TO_CM;

		if(rDelta == lDelta) {
			aDelta = 0;
			localCoord[1] = rDist;
		}
		if(lDelta > rDelta) {
			aDelta = (lDist-rDist)/(lrOffset*2);
			localCoord[1] = (2*sin(aDelta/2))*(rDist/aDelta+lrOffset);
		}
		else {
			aDelta = (rDist-lDist)/(lrOffset*2);
			localCoord[1] = (2*sin(aDelta/2))*(lDist/aDelta+lrOffset);
		}
		float localOffset = angle + (aDelta/2);
		x += localCoord[0]*cos(localOffset) - localCoord[1]*sin(localOffset);
		y += localCoord[0]*sin(localOffset) + localCoord[1]*cos(localOffset);

		angle += aDelta;
		pros::lcd::print(2, "x: %f, y: %f, theta: %f", x, y, angle);
	}
}
