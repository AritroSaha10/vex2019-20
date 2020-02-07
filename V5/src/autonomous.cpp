#include "main.h"
#include "autonSelector.h"
#include "globals.h"
#include "macros.h"

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

/*const int { +FL, +BL } = 1;
const int { -FR, -BR } = 2;
const auto WHEEL_DIAMETER = 4_in;
const auto CHASSIS_WIDTH = 11.5_in;
*/

void updateSysMan(void* param) {
	while(1) {
		intake.update();
		tray.update();
		lift.update();
		pros::delay(5);
	}
}

void autonomous() {
	autonSet = BLUE;
	//autonSelector();
	//deleteSelector();
	tray.fullReset();

	flipout();
	update = pros::Task(updateSysMan, (void *)"PROS", TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "Update system manager");
	if (autonSet == BLUE) {
		drive.setMaxVelocity(130);	
		drive.moveDistanceAsync(1.1_m);
		intake.intake(120);
		drive.waitUntilSettled();
		pros::delay(500);
		intake.stop();
		drive.turnAngle(-90_deg);
		drive.moveDistance(0.42_m);
		drive.turnAngle(-90_deg);
		drive.moveDistance(0.75_m);
		tray.layCubesAuton();
		while(tray.getTrayState() == tray.AUTON_LIFT) {
			if (pros::millis() <= 14800)
			{
				pros::lcd::print(1, "Hello");
				pros::delay(2);
			}
			else
			{
				intake.stop();
				drive.moveDistance(-0.3_m);
				intake.reset();
			}
		}
		pros::lcd::print(1, "Hello again");
		tray.lower();
		drive.moveDistanceAsync(-0.3_m);
	} else if(autonSet==RED) {
		drive.setMaxVelocity(130);
		drive.moveDistanceAsync(1.1_m);
		intake.intake(120);
		drive.waitUntilSettled();
		pros::delay(400);
		intake.stop();
		drive.turnAngle(90_deg);
		drive.moveDistance(0.45_m);
		drive.turnAngle(90_deg);
		drive.moveDistance(0.65_m);
		intake.reset();
		tray.layCubes();
		while (tray.getTrayState() == 0x11)
		{
			if (pros::millis() <= 14800)
			{
				pros::lcd::print(1, "Hello");
				pros::delay(2);
			}
			else
			{
				drive.moveDistance(-0.3_m);
			}
		}
		pros::lcd::print(1, "Hello again");
		tray.lower();
		drive.moveDistanceAsync(-0.3_m);
	} else if (autonSet == SIMPLE) {
		drive.setMaxVelocity(140);
		drive.moveDistance(0.4_m);
		intake.intake(-120);
		drive.moveDistance(-0.4_m);
		drive.waitUntilSettled();
		intake.stop();
	} else if (autonSet == EXPERIMENTAL_BLUE) {
		drive.setMaxVelocity(130);
		drive.moveDistanceAsync(1.1_m);
		intake.intake(120);
		pros::delay(2000);
		intake.stop();
		drive.waitUntilSettled();
		drive.turnAngle(120_deg);
		drive.moveDistance(1.5_m);
		intake.reset();
		tray.layCubes();
		while (tray.getTrayState() == 0x11)
		{
			if (pros::millis() <= 14000) {
				pros::lcd::print(1, "Hello");
				pros::delay(2);
			} else {
				drive.moveDistance(-0.3_m);
			}
		}
		pros::lcd::print(1, "Hello again");
		tray.lower();
		drive.moveDistanceAsync(-0.3_m);
	} else if (autonSet == EXPERIMENTAL_RED) {
		drive.setMaxVelocity(130);
		drive.moveDistanceAsync(1.1_m);
		intake.intake(120);
		pros::delay(3000);
		intake.stop();
		drive.waitUntilSettled();
		drive.turnAngle(-120_deg);
		drive.moveDistance(1.5_m);
		intake.reset();
		tray.layCubes();
	}
	intake.reset();
	pros::Task task_delete(update);
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
