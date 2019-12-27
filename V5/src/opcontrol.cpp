#include "main.h"
#include "tracking.h"
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

extern float x;
extern float y;
extern float theta;

//Controller master;

okapi::Controller master;

auto drive = ChassisControllerFactory::create(
	{+FL_PORT, +BL_PORT}, {-FR_PORT, -BR_PORT},
	AbstractMotor::gearset::green,
	{4_in, 11.5_in}
);

void opcontrol() {
	float speed = 1.0f;
	bool intakeHigh = false;
	bool intakeHeld = false;
	/*pros::Vision andyVision(VISION_PORT);
	pros::vision_signature_s_t PURPLE[3];
	PURPLE[0] = pros::Vision::signature_from_utility(PURPLE_SIG, 2931, 3793, 3362, 5041, 6631, 5836, 4.800, 1);
	PURPLE[1] = pros::Vision::signature_from_utility(PURPLE_SIG2, 2227, 3669, 2948, 2047, 3799, 2923, 3.6, 0);*/
	while (1) {	
	
	//INTAKE
	if(master.getDigital(ControllerDigital::L2) && master.getDigital(ControllerDigital::R2)) {
		if(intakeHeld == false) {
		if(intakeHigh) {
		move({LINTAKE}, 0);
		move({RINTAKE}, 0);
		intakeHigh = false;
		}
		else {
		move({LINTAKE}, -127);
		move({RINTAKE}, 127);
		intakeHigh = true;
		}
		}
		intakeHeld = true;
	}
	else {
		intakeHeld = false;
	}

	//OUTTAKE
	if(master.getDigital(ControllerDigital::R1)) {
		move({LINTAKE}, 127);
		move({RINTAKE}, -127);
	}
	else if(!intakeHigh){
		move({LINTAKE, RINTAKE}, 0);
	}

	//LIFT
	if(master.getDigital(ControllerDigital::X)) {
		move({LIFT}, 127);
		speed = 0.5f;
	}
	else {
		move({LIFT}, 0);
	}

	//TRAY

	drive.tank(joystickSlew(master.getAnalog(ControllerAnalog::leftY))*speed,
				   joystickSlew(master.getAnalog(ControllerAnalog::rightY))*speed,0.05);

	pros::lcd::set_text(2, "I'm working and printing fool");
	pros::delay(10);
	//}
		//pros::vision_object_s_t testCube = andyVision.get_by_sig(0, PURPLE_SIG2);
		//pros::lcd::print(5, "location of purple cube: %f", testCube.left_coord);
	}
}

double joystickSlew(double input) {
	return pow(input, 3);
}
