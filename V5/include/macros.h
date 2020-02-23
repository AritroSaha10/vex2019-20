#pragma once

#include "main.h"

void stackCubes();
void disengageStack();
void liftToMid();
void liftToLow();
void dropLift();
void flipout();
void incrementLift(int dir);

extern bool stacking;
extern bool lifting;
extern bool suspendDrive;
