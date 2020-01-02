#pragma once

#include "main.h"

class SystemManager {
    public:
    SystemManager(uint8_t defaultState);

    // Get Functions
    double getTarget();
    double getError();
    double getPosition();
    double getPower();

    // Public state check functions
    void reset();
    bool enabled();
    void disable();
    // Enables machine, allowing state to be changed
    virtual void enable();
    
    // Called every loop
    virtual void update();

    protected:
    double target;
    double error;
    double position;
    double power;
    uint32_t timeOfLastChange = 0;

    static const uint8_t DISABLED_STATE = 0x00;
    static const uint8_t RESET_STATE = 0x01;
    uint8_t lastState = DISABLED_STATE;
    uint8_t state = DISABLED_STATE;
    
    virtual bool changeState(uint8_t newState);
    bool timedOut(uint32_t timeout);
};