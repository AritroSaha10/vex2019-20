#pragma once
#include "main.h"
#include "vector"

// allMotors array index --- Drive Motors
#define FL 0
#define FR 1
#define BL 2
#define BR 3

// Physical motor port connections -- Drive Motors
#define FL_PORT 19
#define FR_PORT 11
#define BL_PORT 20
#define BR_PORT 12

// ---------------- PLACEHOLDERS --------------------//
// allMotors array index --- Misc. Motors
#define M1 4
#define M2 5
#define M3 6
#define M4 7

// Physical motor port connections -- Misc. Motors
#define M1_PORT 3
#define M2_PORT 4
#define M3_PORT 7
#define M4_PORT 8
// ---------------- PLACEHOLDERS --------------------//

#define AUTO_SPEED 80

double joystickSlew(double input);
void moveDistance(double dist);
std::vector<double> getEncoders(std::vector<int> ports);

