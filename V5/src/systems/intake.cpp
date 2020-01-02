#include "intake.h"
#include "api.h"

// Subclass-specific functions

void Intake::setPower(int power) {
    this->leftIntakeMotor.move(-power);
    this->rightIntakeMotor.move(power);
}

Intake::Intake(uint8_t _defaultState, okapi::Controller _controller) : SystemManager(_defaultState), controller(_controller) {}

// Overrides
void Intake::update() {
    this->position = (this->leftIntakeMotor.get_position() + this->rightIntakeMotor.get_position()) / 2;
    this->error = this->position - this->target;

    switch(this->state) {
        case IDLE_STATE:
            break;
        case IN_STATE:
            break;
        case OUT_STATE:
            break;
        case CONTROL_STATE: {
            float newIntakeSpeed = joystickSlew(this->controller.getAnalog(okapi::ControllerAnalog::rightY)) * 127;
            this->setPower(-newIntakeSpeed);
            break; }
        case HOLD_STATE:
            break;
        case LAY_STATE:
            break;
        case RESET_STATE:
            break;
        case DISABLED_STATE:
            break;
    }
}

bool Intake::changeState(uint8_t newState) {
    bool processed = SystemManager::changeState(newState);
    if(!processed) {
        return false;
    }

    switch(newState) {
        case IDLE_STATE:
            this->setPower(0);
            break;
        case IN_STATE:
            this->setPower(power);
            break;
        case OUT_STATE:
            this->setPower(power);
            break;
        case CONTROL_STATE:
            this->setPower(0);
            break;
        case HOLD_STATE:
            this->leftIntakeMotor.tare_position();
            this->rightIntakeMotor.tare_position();
            this->target = 0;
            this->leftIntakeMotor.move_absolute(0, INTAKE_HOLD_SPEED);
            this->rightIntakeMotor.move_absolute(0, INTAKE_HOLD_SPEED);
            break;
        case LAY_STATE:
            this->setPower(0);
            break;
        case RESET_STATE:
            this->changeState(IDLE_STATE);
            break;
        case DISABLED_STATE:
            this->setPower(0);
            break;
    }

    return true;
}
