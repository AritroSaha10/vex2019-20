#pragma once
#include "main.h"
#include "vector"

#define FL 1
#define FR 2
#define BL 9
#define BR 10

// ---------------- PLACEHOLDERS --------------------//
#define M1 3
#define M2 4
#define M3 7
#define M4 8
// ---------------- PLACEHOLDERS --------------------//

double joystickSlew(double input);
std::vector<double> getEncoders(std::vector<int> ports);

