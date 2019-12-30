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
float rDist, lDist, aDelta, rLast, lLast, halfA;
//constants
const float lrOffset = 13/2*2.54; //13 inch wheel base, halved and converted to cm
const float bOffset = 0; /*PLACEHOLDER*/

void tracking(void* param) {
	lLast = 0;
	rLast = 0;
	x = 0;
	y = 0;
	angle = 0;
	while(1) {
		float localCoord[2];
		std::vector<double> rawEncoders = getEncoders({FL, BL, FR, BR});
		encoders.push_back((rawEncoders[1] + rawEncoders[0])/2);
		encoders.push_back((rawEncoders[2] + rawEncoders[3])/2);

		lDelta = encoders[0]-lLast;
		rDelta = encoders[1]-rLast;
		lLast = encoders[0];
		rLast = encoders[1];
		lDist = lDelta * DEGREE_TO_CM;
		rDist = rDelta * DEGREE_TO_CM;
		encoders.resize(0);

		aDelta = (lDist - rDist)/(lrOffset*2);
		if(aDelta) {
			float radius = rDist / aDelta;
			halfA = aDelta/2;
			float sinHalf = sin(halfA);
			localCoord[1] = ((radius+lrOffset)*sinHalf)*2;
		}
		else {
			aDelta = 0;
			localCoord[1] = rDist;
		}

		float p = halfA + angle; // The global ending angle of the robot
		float cosP = cos(p);
		float sinP = sin(p);

		// Update the global position
		y += localCoord[1] * cosP;
		x += localCoord[1] * sinP;

		//Update angle
		angle += aDelta;

		pros::lcd::print(2, "x: %f, y: %f, angle: %f", x, y, angle*180/M_PI);
		pros::lcd::print(3, "LC: %f", localCoord[1]);
		pros::delay(5);
	}
}
