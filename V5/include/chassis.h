#pragma once
#include "main.h"
#include "vector"

// allMotors array index --- Drive Motors
#define FL 1
#define FR 2
#define BL 3
#define BR 4

// Physical motor port connections -- Drive Motors
#define FL_PORT 19
#define FR_PORT 11
#define BL_PORT 20
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
#define TRAY_PORT 4
#define LIFT_PORT 3
// ---------------- PLACEHOLDERS --------------------//

#define AUTO_SPEED 80

double joystickSlew(double input);
void moveDistance(double dist);
std::vector<double> getEncoders(std::vector<int> ports);
void move(std::vector<int> ports, float speed);

