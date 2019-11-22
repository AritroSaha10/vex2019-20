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

}

std::vector<double> getEncoders(std::vector<int> ports) {
    std::vector<double> returnVec;
    for(int i = 0; i < ports.size(); i++) {
        returnVec.push_back(allMotors[ports[i]].get_position());
    }
    return returnVec;
}