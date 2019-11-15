#include "tracking.h"

double lDist, rDist;

void tracking(void* param) {
	lDist = frontLeftMtr.get_position();
	rDist = frontRightMtr.get_position();
	std::cout << "Left Distance " << lDist;
	std::cout << "Right Distance " << rDist;
	pros::delay(2);
}
