#include "chassis.h"
#include <vector>

pros::Motor allMotors[] = {
    pros::Motor(FR_PORT),
    pros::Motor(BR_PORT),
    pros::Motor(FL_PORT),
    pros::Motor(BL_PORT),
    pros::Motor(M1_PORT),
    pros::Motor(M2_PORT),
    pros::Motor(M3_PORT),
    pros::Motor(M4_PORT)};

void chassisSet(float leftSpeed, float rightSpeed) {
    allMotors[FR].move(rightSpeed);
    allMotors[BR].move(rightSpeed);
    allMotors[FL].move(leftSpeed);
    allMotors[BL].move(leftSpeed);
}

void moveIntake() {

    while (pressed)
	{
		if (MasterController.ButtonR2.pressing()) {
			allMotors[M1].spin(directionType::fwd, 100, percentUnits::pct);
			allMotors[M2].spin(directionType::rev, 100, percentUnits::pct);
		}
		else if (MasterController.ButtonR1.pressing()) {
			allMotors[M1].spin(directionType::rev, 100, percentUnits::pct);
			allMotors[M2].spin(directionType::fwd, 100, percentUnits::pct);
		}
		else {
			allMotors[M1].stop(brakeType::hold);
			allMotors[M2].stop(brakeType::hold);
		}

	}
}

std::vector<double> getEncoders(std::vector<int> ports) {
    std::vector<double> returnVec;
    for(int i = 0; i < ports.size(); i++) {
        returnVec.push_back(allMotors[ports[i]].get_position());
    }
    return returnVec;
}
