#include "main.h"
#include "lift.h"

#define MID_HEIGHT 2900
#define LOW_HEIGHT 1450
#define LIFT_SPEED 110

// Constructor
Lift::Lift(uint8_t _defaultState, Intake _intake, Tray _tray) : SystemManager(_defaultState), intake(_intake), tray(_tray) {}

// Sub-class specific functions

void Lift::stop()
{
    pros::lcd::print(4, "STOP");
    this->power = 0;
    this->target = this->liftMotor.get_position();
    this->liftMotor.modify_profiled_velocity(0);
    this->liftMotor.move(0);
}

void Lift::setPower(double _power)
{
    this->power = _power;
}

void Lift::overridePower(double power)
{
    this->liftMotor.move_absolute(this->target, this->power);
}

void Lift::setTarget(double _target)
{
    this->target = _target;
    this->liftMotor.move_absolute(this->target, this->power);
}

void Lift::drop()
{
    this->changeState(LOWER_STATE);
}

// Overrides
bool Lift::changeState(uint8_t newState)
{
    bool processed = SystemManager::changeState(newState);
    if (!processed)
    {
        return false;
    }

    switch (newState)
    {
    case DISABLED_STATE:
        this->stop();
        break;
    case RESET_STATE:
        this->changeState(IDLE_STATE);
        break;
    case IDLE_STATE:
        this->stop();
        liftMotor.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
        break;
    case LIFT_STATE:
        break;
    case LOWER_STATE:
        this->setTarget(0);
        break;
    case HOLD_STATE:
        this->liftMotor.set_brake_mode(MOTOR_BRAKE_HOLD);
        break;
    }

    return true;
}

uint8_t Lift::getState()
{
    return this->state;
}

void Lift::update()
{
    this->position = this->liftMotor.get_position();
    pros::lcd::print(5, "%f, %u, %f, %f", this->position, this->state, this->liftMotor.get_target_position(), this->liftMotor.get_target_velocity());
    this->error = this->target - this->position;

    switch (this->state)
    {
    case DISABLED_STATE:
        break;
    case RESET_STATE:
        break;
    case IDLE_STATE:
        break;
    case LIFT_STATE:
        break;
    case LOWER_STATE:
        break;
    case HOLD_STATE:
        break;
    }
}
