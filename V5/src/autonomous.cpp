#include "main.h"

/**
 * Runs the user autonomous code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the autonomous
 * mode. Alternatively, this function may be called in initialize or opcontrol
 * for non-competition testing purposes.
 *
 * If the robot is disabled or communications is lost, the autonomous task
 * will be stopped. Re-enabling the robot will restart the task, not re-start it
 * from where it left off.
 */

using namespace pros;
using namespace okapi;

const int { +FL, +BL } = 1;
const int { -FR, -BR } = 2;
const auto WHEEL_DIAMETER = 4_in;
const auto CHASSIS_WIDTH = 11.5_in;


auto drive = ChassisControllerFactory::create(
	{ +FL, +BL }, { -FR, -BR },
	AbstractMotor::gearset::green,
	{ 4_in, 11.5_in }
);

void autonomous() {
	/*pros::Vision autonSensor;
	pros::vision_signature_s_t PURPLE[3];
	;	*/
	
	/*
	chassis.moveDistance(1_m);

	chassis.turnAngle(90_deg);

	chassis.moveDistance(1.5_ft);
	*/
}
