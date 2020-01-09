#pragma once

#include "main.h"
#include "systemManager.h"
#include "intake.h"
#include "chassis.h"

#define LIFT_TIMEOUT 7000
#define LOWER_TIMEOUT 7000

class Tray : public SystemManager {
public:
    static const uint8_t IDLE_STATE = 0x10;
    static const uint8_t LIFT_STATE = 0x11;
    static const uint8_t LOWER_STATE = 0x12;
    static const uint8_t HOLD_STATE = 0x13;

    void layCubes();
    void lower();
    void update() override;
    void setTargetPowerControl(double target, double power);
	uint8_t getTrayState();
    Tray(uint8_t _defaultState, okapi::Controller _controller, Intake _intake);

private:
    double getPowerFunction(double time);
    double getReversePowerFunction(double time);
    void stop();
    void setPower(double _power);
    void setTarget(double target);
    bool changeState(uint8_t newState) override;

    pros::Motor trayMotor = pros::Motor(TRAY_PORT, true);
    okapi::Controller controller;
    Intake intake;
};
