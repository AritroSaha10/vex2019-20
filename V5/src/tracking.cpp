#include "tracking.h"
#include "chassis.h"

#define DEGREE_TO_CM M_PI*8.255/360 	//cm per tick

float lDist, rDist, dist, lastTheta, x, y = 0;
float theta;
float changeInTheta;
float distOfWheels = 26.6; 		//cm
std::vector<double> encoders;

void tracking(void* param) {
	while(1) {
		
		encoders = getEncoders({FL, FR});
		lDist = (encoders[0])*DEGREE_TO_CM-lDist;
		rDist = (encoders[1])*DEGREE_TO_CM-rDist;
		
		dist = (rDist+lDist)/2;
		theta = (rDist-lDist)/distOfWheels; 
		changeInTheta = (theta - lastTheta);
		if (abs((theta)-(lastTheta)) >= 0.1) {
			x += dist*sin(theta-lastTheta+(M_PI/2));
			y += dist*sin(theta-lastTheta);
		}		

		theta += theta-lastTheta;
	
		pros::lcd::print(2, "x: %f, y: %f, theta: %f", x, y, theta);
		pros::lcd::print(3, "theta: %f, lastTheta: %f", theta, lastTheta);
		pros::lcd::print(4, "change in theta: %f", (theta-lastTheta));
		lastTheta = theta;
		pros::delay(2);
	}
}
