#include "chassis.h"
#include <vector>

pros::Motor allMotors [] = {
    pros::Motor(FR),
    pros::Motor(BR),
    pros::Motor(FL),
    pros::Motor(BL),
    pros::Motor(M1),
    pros::Motor(M2),
    pros::Motor(M3),
    pros::Motor(M4)
};

void chassisSet(float leftSpeed, float rightSpeed) {
    allMotors[FR].move(rightSpeed);
    allMotors[BR].move(rightSpeed);
    allMotors[FL].move(leftSpeed);
    allMotors[BL].move(leftSpeed);
}

void moveIntake() {

}

std::vector<double> getEncoders(std::vector<int> ports) {
    std::vector<double> returnVec;
    for(int i = 0; i < ports.size(); i++) {
        returnVec.push_back(allMotors[ports[i]].get_position());
    }
    return returnVec;
}