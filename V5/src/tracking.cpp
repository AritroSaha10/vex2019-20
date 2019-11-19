#include "tracking.h"
#include "chassis.h"

double lDist, rDist;

void tracking(void* param) {
	while(true) {
		std::vector<double> encoders = getEncoders({FL, FR});
		lDist = encoders[0];
		rDist = encoders[1];
		std::cout << "Left Distance " << lDist;
		std::cout << "Right Distance " << rDist;
		pros::delay(2);
	}
}
