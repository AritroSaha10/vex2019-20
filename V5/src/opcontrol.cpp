#include "main.h"
#include "chassis.h"
#include "visionSensor.h"
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

okapi::Controller master;

auto drive = ChassisControllerFactory::create(
	{+FL, +BL}, {-FR, -BR},
	AbstractMotor::gearset::green,
	{4_in, 11.5_in}
);

void opcontrol() {
	while (1) {	
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
		drive.tank(joystickSlew(master.getAnalog(ControllerAnalog::leftY)),
				   joystickSlew(master.getAnalog(ControllerAnalog::rightY)),0.05);

	/*OkapiLib Arcade
	drive.arcade(master.getAnalog(ControllerAnalog::leftY),
				master.getAnalog(ControllerAnalog::rightY));
	*/
		pros::lcd::set_text(2, "I'm working and printing fool");
		pros::delay(10);
	//}
	vision_object_s_t testCube = andyVision.get_by_sig(0, PURPLE_SIG);
	pros::lcd::print(2, "location of purple cube: %f", testCube.left_coord);
	}
}

double joystickSlew(double input) {
	return pow(input, 3);
}
		
