#pragma once

#include "main.h"
#include "systemManager.h"
#include "intake.h"
#include "tray.h"
#include "chassis.h"

class Lift : public SystemManager
{
public:
    static const uint8_t IDLE_STATE = 0x10;
    static const uint8_t LIFT_STATE = 0x11;
    static const uint8_t LOWER_STATE = 0x12;
    static const uint8_t HOLD_STATE = 0x13;

    void drop();
    void raise(bool mid);
    void lock();
    void lower();
    void raiseToMid();
    void raiseToLow();
    void update() override;
    void overridePower(double power);
    uint8_t getState();
    Lift(uint8_t _defaultState, Intake _intake, Tray _tray);

private:
    void stop();
    void setPower(double _power);
    void setTarget(double target);
    bool changeState(uint8_t newState) override;

    pros::Motor liftMotor = pros::Motor(LIFT_PORT);
    Intake intake;
    Tray tray;
    bool mid = false;
};
