#include "main.h"
#include "systemManager.h"

// Constructor
SystemManager::SystemManager(uint8_t defaultState) {
    this->state = defaultState;
    this->lastState = defaultState;
}

// Fixed public functions
double SystemManager::getError() {
    return this->error;
}
double SystemManager::getPosition() {
    return this->position;
}
double SystemManager::getPower() {
    return this->power;
}
double SystemManager::getTarget() {
    return this->target;
}

void SystemManager::reset() {
    this->changeState(RESET_STATE);
}
void SystemManager::enabled() {
    return this->state != DISABLED_STATE;
}
void SystemManager::disable() {
    this->changeState(DISABLED_STATE);
}

// Fixed protected functions
bool SystemManager::timedOut(uint32_t timeout) {
    return this->timeOfLastChange + timeout < pros::millis();
}

// Virtual functions
bool SystemManager::changeState(uint8_t newState) {
    if(this->state == DISABLED_STATE) {
        return false;
    }
    this->lastState = this->state;
    this->state = newState;
    this->timeOfLastChange = pros::millis();
    return true;
}