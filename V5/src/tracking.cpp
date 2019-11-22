#include "tracking.h"
#include "chassis.h"

#define DEGREE_TO_CM M_PI*8.255/360 //cm per tick

double lDist, rDist, dist, deltaL, deltaR, deltaS = 0;
double lastL, lastR, lastS, lastTheta = 0;
double theta, distOfWheels, changeInTheta, x, y;

void tracking(void* param) {
	while(true) {
		
		std::vector<double> encoders = getEncoders({FL, FR});
		lDist = (encoders[0]-lDist)*DEGREE_TO_CM;
		rDist = encoders[1]-rDist*DEGREE_TO_CM;
		dist = (rDist+lDist)/2;
		theta = (deltaL-deltaR)/distOfWheels; 
		changeInTheta = theta-lastTheta;

		lastL = lDist;
		lastR = rDist;
		lastTheta = theta;
		pros::lcd::print(2, "deltaL %f", deltaL);
		pros::lcd::print(3, "deltaR %f", deltaR);
		pros::delay(2);
	}
}
