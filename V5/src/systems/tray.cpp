#include "main.h"
#include "tray.h"

// Constructor
Tray::Tray(uint8_t _defaultState, okapi::Controller _controller, Intake _intake) : SystemManager(_defaultState), controller(_controller), intake(_intake) {}

// Sub-class specific functions

void Tray::setSpeed(double _speed) {
    this->power = _speed;
    this->trayMotor.move(this->power);
}

void Tray::setPower(double _power) {
    this->power = _power;
}

void Tray::setTarget(double _target) {
    this->target = _target;
    this->trayMotor.move_absolute(this->target, this->power);
}

void Tray::layCubes() {
    this->changeState(LIFT_STATE);
}

void Tray::lower() {
    this->changeState(LOWER_STATE);
}

double Tray::getPowerFunction(uint32_t time) {
    return 60 * pow(2, -0.003*time) + 15;
}

double Tray::getReversePowerFunction(uint32_t time) {
    return 70 * pow(2, 0.003*(time-1000)) + 15;
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
        if(this->timedOut(LIFT_TIMEOUT)) {
            this->changeState(HOLD_STATE);
            intake.reset();
        }
        this->setPower(getPowerFunction(pros::millis()-this->timeOfLastChange));
        this->setTarget(MAX_TRAY);
        break;
    case LOWER_STATE:
        if(this->timedOut(LOWER_TIMEOUT)) {
            this->changeState(IDLE_STATE);
        }
        this->setPower(getReversePowerFunction(pros::millis())-this->timeOfLastChange);
        this->setTarget(0);
        break;
    case HOLD_STATE:
        break;
    }
}