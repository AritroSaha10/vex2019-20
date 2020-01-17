#include "systems/intake.h"
#include "main.h"
#include "systems/tray.h"

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
okapi::Controller autoCon;

Intake autoIntake(0x10, autoCon);
Tray autoTray = Tray(0x10, autoCon, autoIntake);

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

/*auto autoDrive = ChassisControllerFactory::create
);*/
void updateSysMan(void* param) {
	while(1) {
		autoIntake.update();
		autoTray.update();
		pros::delay(5);
	}
}

void autonomous() {
	autoTray.resetTray();
	autoDrive.setMaxVelocity(160);
	pros::Task update(updateSysMan, (void*)"PROS", TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "Update system manager");
	autoDrive.moveDistanceAsync(1.72_m);
	autoIntake.intake(INTAKE_SPEED);
	pros::delay(3500);
	autoIntake.stop();
	autoDrive.waitUntilSettled();
	autoDrive.turnAngle(-145_deg);
	autoDrive.moveDistance(2_m);
	autoTray.layCubes();
	while(autoTray.getTrayState() == 0x11) {
		pros::lcd::print(1, "Hello");
		pros::delay(2);
	}
	pros::lcd::print(1, "Hello again");
	autoTray.lower();

///
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
