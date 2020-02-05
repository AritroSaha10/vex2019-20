#include "macros.h"
#include "globals.h"
#include "main.h"

#define MID_HEIGHT 3100
#define LOW_HEIGHT 2250

bool stacking = false;
bool lifting = false;

void finishedLiftCallback() {
    lifting = false;
    lift.setCallback(nullCallback);
}

void stackCubes() {
    if(stacking || lifting) {
        return;
    }
}

void disengageStack() {
    if(!stacking) {
        return;
    }

}

void liftToMid() {
    if(stacking) {
        return;
    }
    lifting = true;
    lift.moveTo(MID_HEIGHT, finishedLiftCallback);
}

void liftToLow() {
    if(stacking) {
        return;
    }
    lifting = true;
    lift.moveTo(LOW_HEIGHT, finishedLiftCallback);
}

void dropLift() {
    if(stacking) return;
    lifting = true;
    lift.moveTo(0, finishedLiftCallback);
}

void incrementLift(int dir) {
    if(lifting) {
        return;
    }
    if(dir == 0) {
        lift.lock();
    }
    else {
        lift.move(dir == 1 ? true : false);
    }
}

void flipout()
{
    lift.moveTo(1000, nullCallback);
    intake.out(-127);
    float time = pros::millis();
    while (pros::millis() - time < 1100)
    {
        lift.update();
        intake.update();
    }
    intake.reset();
    lift.moveTo(0, nullCallback);
    time = pros::millis();
    while (pros::millis() - time < 800)
    {
        lift.update();
    }
    pros::delay(750);
}