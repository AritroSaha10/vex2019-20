#include "main.h"
#include "tray.h"

#define MAX_TRAY 2400

// Constructor
Tray::Tray(uint8_t _defaultState, okapi::Controller _controller, Intake _intake) : SystemManager(_defaultState), controller(_controller), intake(_intake) {}

// Sub-class specific functions

void Tray::stop() {
    pros::lcd::print(4, "STOP");
    this->power = 0;
    this->target = this->trayMotor.get_position();
    this->trayMotor.move(0);
}

void Tray::setPower(double _power) {
    this->power = _power;
}

void Tray::setTargetPowerControl(double _target, double _power) {
	this->trayMotor.move_absolute(_target, _power);
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

double Tray::getPowerFunction(double time) {
    return 80 * pow(2, -0.0015*time) + 40;
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
            this->stop();
            break;
        case RESET_STATE:
            this->changeState(IDLE_STATE);
            break;
        case IDLE_STATE:
            this->stop();
            this->trayMotor.set_brake_mode(MOTOR_BRAKE_COAST);
            break;
        case LIFT_STATE:
            this->intake.reset();
            // this->setPower(75);
            this->setTarget(MAX_TRAY);
            break;
        case LOWER_STATE:
            // this->setPower(80);
            this->setTarget(0);
            break;
        case HOLD_STATE:
            this->intake.reset();
            this->trayMotor.set_brake_mode(MOTOR_BRAKE_HOLD);
            break;
    }

    return true;
}

uint8_t Tray::getTrayState() {
	return this->state;
}

void Tray::resetTray() {
	this->trayMotor.tare_position();
	this->target = 0;
	this->error = 0;
	this->position = 0;
	this->power = 0;
}

void Tray::update() {
    this->position = this->trayMotor.get_position();
    pros::lcd::print(5, "%f, %u, %f, %f", this->position, this->state, this->trayMotor.get_target_position(), this->trayMotor.get_target_velocity());
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
            pros::lcd::print(2, "HOLD, %f", this->timedOut(LIFT_TIMEOUT) ? 1.0f : 0.0f);
            this->stop();
            this->changeState(HOLD_STATE);
            break;
        }
        this->setPower(getPowerFunction(this->position));
        this->trayMotor.modify_profiled_velocity(this->power);
        this->trayMotor.move(this->power);
        break;
    case LOWER_STATE:
        if(this->timedOut(LOWER_TIMEOUT) || abs(this->error)<20) {
            pros::lcd::print(4, "E: %f, TO: %f", this->error, this->timedOut(LOWER_TIMEOUT) ? 1.0f : 0.0f);
            this->trayMotor.tare_position();
            this->stop();
            this->changeState(IDLE_STATE);
            break;
        }
        this->setPower(getReversePowerFunction(this->position));
        this->trayMotor.modify_profiled_velocity(this->power);
        break;
    case HOLD_STATE:
        break;
    }
}
