#include "tracking.h"
#include "chassis.h"
#include "globals.h"
#define _USE_MATH_DEFINES
#include <math.h>

//Globals
float x, y, angle;
//Sensors
float rDelta, lDelta, bDelta;
std::vector<double> encoders;
//Temp
float rDist, lDist, aDelta, rLast, lLast, halfA;
float bDist, bLast;
//constants
const float lrOffset = 11.0f/2.0f; //13 inch wheel base, halved
const float bOffset = -3.5;

#define DRIVE_DEGREE_TO_INCH (M_PI * 4 / 360) //inches per tick
#define TRACKING_WHEEL_DEGREE_TO_INCH (M_PI * 3.25 / 360)

void tracking(void* param) {
	//resetChassis();
	lLast = 0;
	rLast = 0;
	bLast = 0;
	x = 0;
	y = 0;
	float left = 0;
	float right = 0;
	float lateral = 0;
	angle = 0;
	while(1) {
		float localCoord[2];
		// std::vector<double> rawEncoders = getEncoders({FL, BL, FR, BR});
		// encoders.push_back((rawEncoders[1] + rawEncoders[0])/2);
		// encoders.push_back((rawEncoders[2] + rawEncoders[3])/2);

		float leftEncoder = (frontLeftDrive.get_position() + backLeftDrive.get_position())/2;
		float rightEncoder = (frontRightDrive.get_position() + backRightDrive.get_position())/2;
		lDelta = leftEncoder - lLast;
		rDelta = rightEncoder - rLast;
		lLast = leftEncoder;
		rLast =	rightEncoder;
		lDist = lDelta * DRIVE_DEGREE_TO_INCH;
		rDist = rDelta * DRIVE_DEGREE_TO_INCH;
		left+=lDist;
		right+=rDist;

		bDelta = backTrackingWheel.get_value()-bLast;
		bLast = backTrackingWheel.get_value();
		bDist = bDelta * TRACKING_WHEEL_DEGREE_TO_INCH;
		lateral += bDist;

		aDelta = (lDist - rDist)/(lrOffset*2.0f);
		if(aDelta) {
			float radius = rDist / aDelta;
			halfA = aDelta/2.0f;
			float sinHalf = sin(halfA);
			localCoord[1] = ((radius + lrOffset) * sinHalf) * 2.0f;

			float backRadius = bDist / aDelta;
			localCoord[0] = ((radius + lrOffset) * sinHalf) * 2.0f;
		}
		else {
			aDelta = 0;
			localCoord[1] = (rDist+lDist)/2;
			localCoord[0] = bDist;
		}

		float p = halfA + angle; // The global ending angle of the robot
		float cosP = cos(p);
		float sinP = sin(p);

		// Update the global position
		y += localCoord[1] * cosP - localCoord[0] * sinP;
		x += localCoord[1] * sinP + localCoord[0] * cosP;

		//Update angle
		angle += aDelta;

		pros::lcd::print(1, "X: %f, Y: %f, A: %f", x, y, angle/M_PI*360);
		pros::lcd::print(2, "L: %f R: %f B: %f", left, right, lateral);

		pros::delay(5);
	}
}
