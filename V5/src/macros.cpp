#include "macros.h"
#include "globals.h"
#include "main.h"

#define MID_HEIGHT 3100
#define LOW_HEIGHT 2250
#define LIFT_MULTIPLIER 0.5f

bool stacking = false;
bool lifting = false;

void finishedLiftCallback() {
    lifting = false;
    lift.setCallback(nullCallback);
}

void droppedLiftCallback() {
    lifting = false;
    lift.setCallback(nullCallback);
    tray.lower();
}

void finishedTrayCallback() {
    stacking = false;
    
}

void stackCubes() {
    if(stacking || lifting) {
        return;
    }
    stacking = true;
    tray.layCubes();
}

void disengageStack() {
    if(!stacking) {
        return;
    }
    stacking = true;
    /*Something with the intake*/
    tray.lower();
}

void liftToMid() {
    if(stacking) {
        return;
    }
    lifting = true;
    lift.moveTo(MID_HEIGHT, finishedLiftCallback);
    tray.moveTo(1000);
}

void liftToLow() {
    if(stacking) {
        return;
    }
    lifting = true;
    lift.moveTo(LOW_HEIGHT, finishedLiftCallback);
    tray.moveTo(1000);
}

void dropLift() {
    if(stacking) return;
    lifting = true;
    lift.moveTo(0, droppedLiftCallback);
}

void incrementLift(int dir) {
    if(lifting) {
        return;
    }
    tray.moveTo(lift.getPosition()*LIFT_MULTIPLIER);
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