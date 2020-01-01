#pragma once

#include "main.h"
#include "systemManager.h"
#include "chassis.h"

class Intake : public SystemManager {
    public:
    static const uint8_t IDLE_STATE = 0x10;
    static const uint8_t IN_STATE = 0x11;
    static const uint8_t OUT_STATE = 0x12;
    static const uint8_t CONTROL_STATE = 0x13;
    static const uint8_t HOLD_STATE = 0x14;
    static const uint8_t LAY_STATE = 0x15;

    void intake();
    void out();
    void control();
    void lay();

    private:


    pros::Motor leftIntakeMotor = pros::Motor(LINTAKE_PORT);
    pros::Motor rightIntakeMotor = pros::Motor(RINTAKE_PORT);
};