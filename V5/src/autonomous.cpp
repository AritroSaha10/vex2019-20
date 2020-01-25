#include "systems/intake.h"
#include "main.h"
#include "systems/tray.h"
#include "systems/lift.h"
#include "systems.h"
#include "autonSelector.h"

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

void nullTask(void* param) {
	pros::delay(10);
}

Intake intake = Intake(0x10, autoCon);
Tray tray = Tray(0x10, autoCon, intake);
Lift lift = Lift(0x10, intake, tray);
Task update(nullTask, (void *)"PROS", TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "null");

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
		intake.update();
		tray.update();
		lift.update();
		pros::delay(5);
	}
}

void flipout(Intake intake, Lift lift) {
	lift.raise(false);
	intake.out(-127);
	float time = pros::millis();
	while(pros::millis()-time < 1100) {
		lift.update();
		intake.update();
	}
	intake.reset();
	lift.lower();
	time = pros::millis();
	while(pros::millis()-time < 800) {
		lift.update();
	}
	lift.reset();
	pros::delay(750);
}

void autonomous() {
	deleteSelector();
	pros::delay(2000);
	tray.fullReset();

	flipout(intake, lift);
	update = pros::Task(updateSysMan, (void *)"PROS", TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "Update system manager");
	if (autonSet == BLUE) {
		autoDrive.setMaxVelocity(130);
		
		autoDrive.moveDistanceAsync(1.1_m);
		intake.intake(120);
		pros::delay(3500);
		intake.stop();
		autoDrive.waitUntilSettled();
		autoDrive.turnAngle(-138_deg);
		autoDrive.moveDistance(0.81_m);
		intake.reset();
		tray.layCubes();
		while(tray.getTrayState() == 0x11) {
			pros::lcd::print(1, "Hello");
			pros::delay(2);
		}
		pros::lcd::print(1, "Hello again");
		tray.lower();
		autoDrive.moveDistanceAsync(-0.3_m);
	} else if(autonSet==RED) {
		autoDrive.setMaxVelocity(130);
		autoDrive.moveDistanceAsync(1.1_m);
		intake.intake(120);
		pros::delay(3000);
		intake.stop();
		autoDrive.waitUntilSettled();
		autoDrive.turnAngle(138_deg);
		autoDrive.moveDistance(0.81_m);
		intake.reset();
		tray.layCubes();
		while (tray.getTrayState() == 0x11)
		{
			pros::lcd::print(1, "Hello");
			pros::delay(2);
		}
		pros::lcd::print(1, "Hello again");
		tray.lower();
		autoDrive.moveDistanceAsync(-0.3_m);
	} else if (autonSet == SIMPLE) {
		autoDrive.setMaxVelocity(140);
		autoDrive.moveDistance(1_m);
		autoDrive.moveDistance(-0.3_m);
	} else if (autonSet == EXPERIMENTAL_BLUE) {
		autoDrive.setMaxVelocity(130);
		autoDrive.moveDistanceAsync(1.1_m);
		intake.intake(120);
		pros::delay(3000);
		intake.stop();
		autoDrive.waitUntilSettled();
		autoDrive.turnAngle(120_deg);
		autoDrive.moveDistance(1.5_m);
		intake.reset();
		tray.layCubes();
		while (tray.getTrayState() == 0x11)
		{
			pros::lcd::print(1, "Hello");
			pros::delay(2);
		}
		pros::lcd::print(1, "Hello again");
		tray.lower();
		autoDrive.moveDistanceAsync(-0.3_m);
	} else if (autonSet == EXPERIMENTAL_RED) {
		autoDrive.setMaxVelocity(130);
		autoDrive.moveDistanceAsync(1.1_m);
		intake.intake(120);
		pros::delay(3000);
		intake.stop();
		autoDrive.waitUntilSettled();
		autoDrive.turnAngle(-120_deg);
		autoDrive.moveDistance(1.5_m);
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
