#include "main.h"
#include "lift.h"

#define MID_HEIGHT 3100
#define LOW_HEIGHT 2250
#define LIFT_SPEED 107

// Constructor
Lift::Lift(uint8_t _defaultState, Tray _tray) : SystemManager(_defaultState), tray(_tray) {}

// Sub-class specific functions

void Lift::stop()
{
    this->power = 0;
    this->target = this->liftMotor.get_position();
    this->liftMotor.modify_profiled_velocity(0);
    this->liftMotor.move(0);
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

// Overrides
bool Lift::changeState(uint8_t newState)
{
    if(lockState && (newState != RESET_STATE || newState != HOLD_STATE)) {
        return false;
    }
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
        this->tray.setTargetPowerControl(1000, 100);
        break;
    case LOWER_STATE:
        break;
    case HOLD_STATE:
        this->target = this->liftMotor.get_position();
        this->stop();
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
        this->liftMotor.move(127);
        if(this->position > MID_HEIGHT-10 && mid) {
            this->lock();
        }
        else if(this->position > LOW_HEIGHT-10 && !mid) {
            this->lock();
        }
        break;
    case LOWER_STATE:
        this->liftMotor.move(-100);
        if(this->position < 250) {
            this->tray.setTargetPowerControl(0, 127);
            this->reset();
        }
        break;
    case HOLD_STATE:
        if(abs(this->position - this->target) > 40) {
            this->liftMotor.move_absolute(this->target, 100);
        }
        break;
    }
}

void Lift::raise(bool _mid) {
    this->mid = _mid;
    if(this->state == LIFT_STATE) {
        return;
    }
    this->changeState(LIFT_STATE);
}

void Lift::lower() {
    if(this->state == LOWER_STATE) {
        return;
    }
    this->changeState(LOWER_STATE);
}

void Lift::raiseToLow() {
    lockState = true;
    this->raise(false);
}

void Lift::raiseToMid() {
    lockState = true;
    this->raise(true);
}

void Lift::drop() {
    lockState = true;
    if(this->state == LOWER_STATE) {
        return;
    }
    this->changeState(LOWER_STATE);
}

void Lift::lock() {
    if(this->state == HOLD_STATE) {
        return;
    }
    this->changeState(HOLD_STATE);
}

void Lift::fullReset() {
    this->liftMotor.tare_position();
    SystemManager::fullReset();
}