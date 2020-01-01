#pragma once
#include "main.h"
#include "vector"

// allMotors array index --- Drive Motors
#define FL 2
#define FR 0
#define BL 3
#define BR 1

// Physical motor port connections -- Drive Motors
#define FL_PORT 20
#define FR_PORT 11
#define BL_PORT 19
#define BR_PORT 12

// ---------------- PLACEHOLDERS --------------------//
// allMotors array index --- Misc. Motors
#define LINTAKE 4
#define RINTAKE 5
#define TRAY 6
#define LIFT 7

// Physical motor port connections -- Misc. Motors
#define LINTAKE_PORT 5
#define RINTAKE_PORT 6
#define TRAY_PORT 2
#define LIFT_PORT 3
// ---------------- PLACEHOLDERS --------------------//

#define AUTO_SPEED 80
#define INTAKE_SPEED 117
#define LIFT_LIMIT 2506

double joystickSlew(double input);
std::vector<double> getEncoders(std::vector<int> ports);
void auton();
void resetChassis();
void moveIntake(int cond);
void move(std::vector<int> ports, float speed);
void motorTemp(void* param);
void moveAbsolute(std::vector<int> ports, float distance);
