#include "main.h"
#include "tracking.h"
#include "systems/toggle.h"
#include "systems/intake.h"
#include "systems/tray.h"
#include <initializer_list>
#include <string>
#include <sstream>

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

std::vector<double> encoder;
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

void layStack(Intake intake, Tray& tray, Toggle& t) {
	tray.layCubes();
	while(t.checkState() != 0) {
		tray.update();
		// float traySpeed = joystickSlew(master.getAnalog(ControllerAnalog::rightY));
		// move({TRAY}, traySpeed*127);

		float driveSpeed = joystickSlew(master.getAnalog(ControllerAnalog::leftY));
		drive.tank(driveSpeed*0.5f, driveSpeed*0.5f, 0.05f);

		if(driveSpeed < 0) {
			intake.out(driveSpeed * 127 /2);
		}
		else {
			intake.reset();
			move({LINTAKE, RINTAKE}, 0);
		}
		pros::delay(5);
	}
}

std::string IntToStr(double i)
{
	std::ostringstream out;
	out << i;
	return out.str();
}

void opcontrol() {
	int lift = 0;
	int holdLift = 0;
	float speed = 1.0f;
	bool intakeHigh = false;
	bool intakeHeld = false;
	Toggle fullIntake = Toggle({ControllerDigital::L2, ControllerDigital::R2}, master);
	Toggle controlIntake = Toggle({ControllerDigital::R1}, master);
	Toggle engageTray = Toggle({ControllerDigital::L1}, master);
	Intake intake = Intake(0x10, master);
	Tray tray = Tray(0x10, master, intake);
	tray.reset();
	/*pros::Vision andyVision(VISION_PORT);
	pros::vision_signature_s_t PURPLE[3];
	PURPLE[0] = pros::Vision::signature_from_utility(PURPLE_SIG, 2931, 3793, 3362, 5041, 6631, 5836, 4.800, 1);
	PURPLE[1] = pros::Vision::signature_from_utility(PURPLE_SIG2, 2227, 3669, 2948, 2047, 3799, 2923, 3.6, 0);*/
	int lastEncoder = getEncoders({TRAY})[0];
	while (1) {	
	encoder = getEncoders({LIFT, TRAY});	
	
	intake.update();
	tray.update();

	if(master.getDigital(ControllerDigital::X))
		lift = 1;
	else {
		lift = 0;
		holdLift = 0;
		release(LIFT);
	}

	if(abs(getEncoders({TRAY})[0]-lastEncoder) > 5) {
		master.setText(0, 0, IntToStr(getEncoders({TRAY})[0]));
		lastEncoder = getEncoders({TRAY})[0];
	}

	//INTAKE
	int in = fullIntake.checkState();
	if(in == 1) {
		intake.intake(INTAKE_SPEED);
	}
	else if(in == 0) {
		intake.stop();
	}

	//OUTTAKE
	int control = controlIntake.checkState();
	if(control == 1) {
		intake.control();
	}
	else if(control == 0) {
		intake.stop();
	}

		
	//LIFT
	if (holdLift && encoder[0] > (LIFT_LIMIT-30)) {
		move({LIFT}, 0);
		hold(LIFT);
	} else if (holdLift) {
		move({LIFT}, 0);
		holdLift = 0;
		lift = 1;
	} else if (lift) {
		tray.setTargetPowerControl(-(encoder[1]-2000), 10);
		move({LIFT}, 117);
		holdLift = (encoder[0] > (LIFT_LIMIT-10)) ? 1 : 0;
	} else if (!lift && encoder[0] > 100) {
		move({LIFT}, -(LIFT_SPEED-50));
		tray.setTargetPowerControl(0, 10);
	}	
	//TRAY
	int stack = engageTray.checkState();
	if(stack == 1) {
		layStack(intake, tray, engageTray);
		tray.lower();
	}
		//tray.setTarget(-900);
	//drive.tank(joystickSlew(master.getAnalog(ControllerAnalog::leftY))*speed,
	//			   joystickSlew(master.getAnalog(ControllerAnalog::rightY))*speed,0.05);

	drive.arcade(joystickSlew(master.getAnalog(ControllerAnalog::leftY)), joystickSlew(master.getAnalog(ControllerAnalog::leftX)), 0.05f);

	pros::delay(10);
	// pros::lcd::print(1, "tray getstate %u", tray.getTrayState());
	//}
		//pros::vision_object_s_t testCube = andyVision.get_by_sig(0, PURPLE_SIG2);
		//pros::lcd::print(5, "location of purple cube: %f", testCube.left_coord);
		//pros::lcd::print(5, "loc x  %f; loc y %f", x, y);
	}
}

double joystickSlew(double input) {
	return pow(input, 3);
}
