#include "main.h"

#include <initializer_list>

/**
 * Runs the operator control code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the operator
 * control mode.
 *
 * If no competition control is connected, this function will run immediately
 * following initialize().
 *
 * If the robot is disabled or communications is lost, the
 * operator control task will be stopped. Re-enabling the robot will restart the
 * task, not resume it from where it left off.
 */

using namespace pros;
using namespace okapi;

//Controller master;
pros::Motor frontLeftMtr (1);
pros::Motor backLeftMtr (2);
pros::Motor frontRightMtr (9);
pros::Motor backRightMtr (10);

okapi::Controller master;

auto drive = ChassisControllerFactory::create(
	{1, 9}, {2, 10},
	AbstractMotor::gearset::green,
	{4_in, 11.5_in}
);

void opcontrol() {

	//TRADITIONAL CONTROLLER
	//pros::Controller master(pros::E_CONTROLLER_MASTER);

	/*Traditional
	int left = master.get_analog(ANALOG_LEFT_Y);
	int right = master.get_analog(ANALOG_RIGHT_Y);

	backLeftMtr = left;
	frontLeftMtr = left;
	frontRightMtr = right;
	backRightMtr = right;

	pros::delay(20);
	*/

	//OkapiLib
	/*frontLeftMtr.move(100);
	pros::delay(5000);
	frontLeftMtr.move(0);
	backLeftMtr.move(100);
	pros::delay(5000);
	backLeftMtr.move(0);
	frontRightMtr.move(100);
	pros::delay(5000);
	frontRightMtr.move(0);
	backRightMtr.move(100);
	pros::delay(5000);
	backRightMtr.move(0);*/
	drive.tank(master.getAnalog(ControllerAnalog::leftY),
				   master.getAnalog(ControllerAnalog::rightY),0.2);

	/*OkapiLib Arcade
	drive.arcade(master.getAnalog(ControllerAnalog::leftY),
				master.getAnalog(ControllerAnalog::rightY));
	*/

	pros::delay(10);
	//}
}
		
