#pragma once

#include "main.h"
#include "systemManager.h"
#include "chassis.h"

#define INTAKE_HOLD_SPEED 60
static const uint8_t IDLE_STATE = 0x10;
static const uint8_t IN_STATE = 0x11;
static const uint8_t OUT_STATE = 0x12;
static const uint8_t CONTROL_STATE = 0x13;
static const uint8_t HOLD_STATE = 0x14;
static const uint8_t LAY_STATE = 0x15;

class Intake : public SystemManager {
	public:
		void intake(double _power);
		void out(double _power);
		void control();
		void lay();
		void stop();

		void update() override;

		Intake(uint8_t _defaultState, okapi::Controller _controller);

	private:

		void setPower(int power);
		bool changeState(uint8_t newState) override;
		pros::Motor leftIntakeMotor = pros::Motor(LINTAKE_PORT);
		pros::Motor rightIntakeMotor = pros::Motor(RINTAKE_PORT);
		okapi::Controller controller;
};
