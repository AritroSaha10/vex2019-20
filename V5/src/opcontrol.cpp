#include "main.h"
#include "tracking.h"
#include "globals.h"
#include "systems/toggle.h"
#include <initializer_list>
#include <string>
#include <sstream>

double const accel = 0.045;
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

auto drive = ChassisControllerFactory::create(
	{+FL_PORT, +BL_PORT}, {-FR_PORT, -BR_PORT},
	AbstractMotor::gearset::green,
	{4_in, 11.5_in}
);

void layStack(Intake intake, Tray& tray, Toggle& t) {
	tray.layCubes();
	while(t.checkState() != 0) {
		tray.update();
		if(master.getDigital(ControllerDigital::L2)) {
			tray_motor.move_velocity(70);
		}
		else if(master.getDigital(ControllerDigital::R2)) {
			tray_motor.move_velocity(-70);
		}
		else if(tray.getState() != 0x12 && tray.getState() != 0x11) {
			tray_motor.move_velocity(0);
		}

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
	update.remove();
	intake.control();

	// Acceleration curving
	double lSpeed =0;
	double rSpeed = 0;
	double reqRSpeed = 0;
	double reqLSpeed = 0;

	// Speed limit
	float speed = 1.0f;

	bool dropping = false;

	// Toggle switches
	Toggle fullIntake = Toggle({ControllerDigital::L2, ControllerDigital::R2}, master);
	Toggle controlIntake = Toggle({ControllerDigital::R1}, master, true);
	Toggle engageTray = Toggle({ControllerDigital::L1}, master);
	Toggle liftButton = Toggle({ControllerDigital::Y}, master);

	tray_motor.set_brake_mode(MOTOR_BRAKE_HOLD);
	/*pros::Vision andyVision(VISION_PORT);
	pros::vision_signature_s_t PURPLE[3];
	PURPLE[0] = pros::Vision::signature_from_utility(PURPLE_SIG, 2931, 3793, 3362, 5041, 6631, 5836, 4.800, 1);
	PURPLE[1] = pros::Vision::signature_from_utility(PURPLE_SIG2, 2227, 3669, 2948, 2047, 3799, 2923, 3.6, 0);*/
	int lastEncoder = getEncoders({TRAY})[0];
	while (1) {
	tray.update();
	intake.update();
	lift.update();
	encoder = getEncoders({LIFT, TRAY});

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

	if(master.getDigital(ControllerDigital::X)) {
		lift.raise(true);
	}
	else if(master.getDigital(ControllerDigital::Y)) {
		lift.raise(false);
	}
	else if(master.getDigital(ControllerDigital::B)) {
		lift.lower();
	}
	else if(!dropping){
		lift.lock();
	}

	if(master.getDigital(ControllerDigital::down)) {
		dropping = true;
		lift.drop();
	}
	if(dropping) {
		if(lift.getState() == 0x10) {
			dropping = false;
		}
	}

	//TRAY
	if(master.getDigital(ControllerDigital::L2)) {
		tray_motor.move_velocity(70);
	}
	else if(master.getDigital(ControllerDigital::R2)) {
		tray_motor.move_velocity(-70);
	}
	else if(tray.getState() != 0x12 && lift.getState() == 0x10) {
		tray_motor.move_velocity(0);
	}
	int stack = engageTray.checkState();
	if(stack == 1) {
		layStack(intake, tray, engageTray);
		tray.lower();
	}

	// Acceleration curve
	reqLSpeed = joystickSlew(master.getAnalog(ControllerAnalog::leftY));
	reqRSpeed = joystickSlew(master.getAnalog(ControllerAnalog::leftX));
	if (reqRSpeed > (rSpeed+accel))
		rSpeed+=accel;
	else if (reqRSpeed < (rSpeed-accel))
		rSpeed -= accel;
	else
		rSpeed = reqRSpeed;
	if (reqLSpeed > (lSpeed+accel))
		lSpeed+=accel;
	else if (reqLSpeed < (lSpeed-accel))
		lSpeed -= accel;
	else
		lSpeed = reqLSpeed;
	drive.arcade(lSpeed, rSpeed, 0.05f);
	
	// Diagnostics
	pros::delay(10);
	pros::lcd::print(1, "reqRSpeed: %f, reqLSpeed: %f", reqRSpeed, reqLSpeed);
	pros::lcd::print(2,"Lift: %f", lift.getPosition());
	//}
		//pros::vision_object_s_t testCube = andyVision.get_by_sig(0, PURPLE_SIG2);
		//pros::lcd::print(5, "location of purple cube: %f", testCube.left_coord);
		//pros::lcd::print(5, "loc x  %f; loc y %f", x, y);
	}
}

double joystickSlew(double input) {
	return pow(input, 3);
}
