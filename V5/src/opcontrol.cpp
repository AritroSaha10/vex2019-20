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

pros::Motor frontLeftMtr (1);
pros::Motor backLeftMtr (2);
pros::Motor frontRightMtr (3);
pros::Motor backRightMtr (4);

auto drive = ChassisControllerFactory::create(
	{1, 2}, {3, 4},
	AbstractMotor::gearset::green,
	{4_in, 11.5_in}
);

void opcontrol() {

	//TRADITIONAL CONTROLLER
	//pros::Controller master(pros::E_CONTROLLER_MASTER);

	//OKAPILIB CONTROLLER
	okapi::Controller master;

		while (true)
	{
		pros::lcd::print(0, "%d %d %d", (pros::lcd::read_buttons() & LCD_BTN_LEFT) >> 2,
		                 (pros::lcd::read_buttons() & LCD_BTN_CENTER) >> 1,
		                 (pros::lcd::read_buttons() & LCD_BTN_RIGHT) >> 0);

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

		drive.tank(master.getAnalog(ControllerAnalog::leftY),
				   master.getAnalog(ControllerAnalog::rightY));

		/*OkapiLib Arcade
		drive.arcade(master.getAnalog(ControllerAnalog::leftY),
					master.getAnalog(ControllerAnalog::rightY));
		*/

		pros::delay(10);
	}
}
