#include "main.h"
#include "tray.h"

#define MAX_TRAY 2400

// Constructor
Tray::Tray(uint8_t _defaultState, okapi::Controller _controller, Intake _intake) : SystemManager(_defaultState), controller(_controller), intake(_intake) {}

// Sub-class specific functions

void Tray::setSpeed(double _speed) {
    this->power = -_speed;
    this->trayMotor.move(this->power);
}

void Tray::setPower(double _power) {
    this->power = -_power;
}

void Tray::setTarget(double _target) {
    this->target = -_target;
    this->trayMotor.move_absolute(this->target, this->power);
}

void Tray::layCubes() {
    this->changeState(LIFT_STATE);
}

void Tray::lower() {
    this->changeState(LOWER_STATE);
}

double Tray::getPowerFunction(double time) {
    return 80 * pow(2, -0.0015*time) + 47;
}

double Tray::getReversePowerFunction(double time) {
    return 70 * pow(2, 0.003*(time-1000)) + 60;
}

// Overrides

bool Tray::changeState(uint8_t newState) {
    bool processed = SystemManager::changeState(newState);
    if(!processed) {
        return false;
    }

    switch(newState) {
        case DISABLED_STATE:
            this->setSpeed(0);
            break;
        case RESET_STATE:
            this->changeState(IDLE_STATE);
            break;
        case IDLE_STATE:
            this->setPower(0);
            this->setTarget(0);
            break;
        case LIFT_STATE:
            this->setPower(75);
            this->setTarget(MAX_TRAY);
            break;
        case LOWER_STATE:
            this->setPower(80);
            this->setTarget(0);
            break;
        case HOLD_STATE:
            this->setPower(60);
            this->setTarget(this->position);
            break;
    }

    return true;
}

void Tray::update() {
    this->position = this->trayMotor.get_position();
    this->error = this->target - this->position;

    switch(this->state) {
    case DISABLED_STATE:
        break;
    case RESET_STATE:
        break;
    case IDLE_STATE:
        break;
    case LIFT_STATE:
        if(this->timedOut(LIFT_TIMEOUT) || abs(this->error)<20) {
            this->changeState(HOLD_STATE);
            intake.reset();
        }
        pros::lcd::print(5, "%f", getPowerFunction(pros::millis()-this->timeOfLastChange));
        this->setPower(getPowerFunction(-this->position));
        this->trayMotor.modify_profiled_velocity(this->power);
        break;
    case LOWER_STATE:
        if(this->timedOut(LOWER_TIMEOUT) || abs(this->error)<20) {
            this->changeState(IDLE_STATE);
        }
        pros::lcd::print(5, "%f", getReversePowerFunction(pros::millis()-this->timeOfLastChange));
        this->setPower(-getReversePowerFunction(-this->position));
        this->trayMotor.modify_profiled_velocity(this->power);
        break;
    case HOLD_STATE:
        break;
    }
}