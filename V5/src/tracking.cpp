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
const float lrOffset = 13.0f/2.0f*2.54f; //13 inch wheel base, halved and converted to cm
const float bOffset = 0; /*PLACEHOLDER*/

void tracking(void* param) {
	//resetChassis();
	lLast = 0;
	rLast = 0;
	x = 0;
	y = 0;
	float left = 0;
	float right = 0;
	angle = 0;
	while(1) {
		float localCoord[2];
		// std::vector<double> rawEncoders = getEncoders({FL, BL, FR, BR});
		// encoders.push_back((rawEncoders[1] + rawEncoders[0])/2);
		// encoders.push_back((rawEncoders[2] + rawEncoders[3])/2);

		encoders = getEncoders({FL, FR});
		lDelta = encoders[0]-lLast;
		rDelta = encoders[1]-rLast;
		lLast = encoders[0];
		rLast = encoders[1];
		lDist = lDelta * DEGREE_TO_CM;
		rDist = rDelta * DEGREE_TO_CM;
		left+=lDist;
		right+=rDist;

		aDelta = (lDist - rDist)/(lrOffset*2.0f);
		if(aDelta) {
			float radius = rDist / aDelta;
			halfA = aDelta/2.0f;
			float sinHalf = sin(halfA);
			localCoord[1] = ((radius+lrOffset)*sinHalf)*2.0f;
		}
		else {
			aDelta = 0;
			localCoord[1] = (rDist+lDist)/2;
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
		pros::lcd::print(3, "L: %f R: %f", left, right);
		pros::lcd::print(4, "size: %i", encoders.size());
		pros::delay(5);
	}
}
