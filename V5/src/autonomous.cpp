#include "systems/intake.h"
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
okapi::Controller fuckyou;

/*const int { +FL, +BL } = 1;
const int { -FR, -BR } = 2;
const auto WHEEL_DIAMETER = 4_in;
const auto CHASSIS_WIDTH = 11.5_in;
*/

auto autoDrive = ChassisControllerFactory::create(
	{ +FL_PORT, +BL_PORT}, { -FR_PORT, -BR_PORT },
	AbstractMotor::gearset::green,
	{ 3.25_in, 13_in }
);

/*auto autoDrive = ChassisControllerFactory::create(
);*/
void autonomous() {
	Intake autoIntake(IDLE_STATE, fuckyou);
	autoDrive.setMaxVelocity(160);
	autoDrive.moveDistanceAsync(1.72_m);
	autoIntake.intake(INTAKE_SPEED);
	pros::delay(3500);
	autoIntake.intake(0);
	autoDrive.waitUntilSettled();
	autoDrive.turnAngle(-145_deg);
		/*
	*Possible auton path number 1
	chassis.moveDistance(2_ft);

	chassis.turnAngle(135_deg);

	chassis.moveDistance(2_ft);

	chassis.turnAngle(-135_deg);

	chassis.moveDistance(2_ft);

	chassis.turnAngle(-135_deg);

	chassis.moveDistance(3_ft);
	*/

	/*
	*Possible auton path number 2
	chassis.moveDistance(2_ft);

	chassis.moveDistance(-2_ft);

	chassis.turnAngle(-90_deg);

	chassis.moveDistance(1_ft);
	*/
}
