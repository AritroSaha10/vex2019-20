#include "macros.h"
#include "globals.h"
#include "main.h"

#define MID_HEIGHT 700
#define LOW_HEIGHT 450
#define LIFT_MULTIPLIER 0.5f
#define TRAY_LIFT_MAX 360
#define MANUAL_TRAY_VELOCITY 50

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
    tray.setCallback(nullCallback);
    drive.setMaxVelocity(600);
    intake.control();
}

void stackCubes() {
    stacking = true;
    tray.layCubes();
    tray.setCallback(finishedTrayCallback);
}

void autonStack() {
    stacking = true;
    tray.layCubes();
    tray.setCallback(finishedTrayCallback);
    intake.reset();
}

void disengageStack() {
    stacking = true;
    /*Something with the intake*/
    //drive.setMaxVelocity(400);
    //intake.out(40);
    using namespace okapi::literals;
    //drive.moveDistanceAsync(-0.2_m);
    tray.lower();
}

void liftToMid() {
    lifting = true;
    tray.setTargetPowerControl(TRAY_LIFT_MAX, 127);
    pros::delay(200);
    lift.moveTo(MID_HEIGHT, finishedLiftCallback);
}

void liftToLow() {
    if(stacking) {
        return;
    }
    lifting = true;
    tray.setTargetPowerControl(TRAY_LIFT_MAX, 127);
    pros::delay(200);
    lift.moveTo(LOW_HEIGHT, finishedLiftCallback);
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
    if(dir == 0) {
        lift.lock();
    }
    else {
        if(dir == 1) {
            if (lift.getPosition()+150 < TRAY_LIFT_MAX) {
                tray.setTargetPowerControl(lift.getPosition()+150, 127);
            }
            else if (tray.getTarget() > TRAY_LIFT_MAX) {
                tray.setTargetPowerControl(TRAY_LIFT_MAX, 127);
            }
        }
        else if(lift.getPosition() < 250) {
            lift.reset();
            tray.lower();
        }
        if(tray.getPosition() > 100) {
            lift.move(dir == 1 ? true : false);
        }
    }
}

void flipout()
{
    intake.out(-127);
    tray.setTargetPowerControl(100, 127);
    pros::delay(200);
    lift.moveTo(1000, nullCallback);
    tray.setTargetPowerControl(TRAY_LIFT_MAX, 127);
    intake.out(-127);
    float time = pros::millis();
    while (pros::millis() - time < 1500)
    {
        lift.update();
        tray.update();
        intake.update();
    }
    intake.reset();
    lift.move(false);
    time = pros::millis();
    while (pros::millis() - time < 2500)
    {
        if(lift.getPosition() < 350) {
            tray.setTargetPowerControl(0, 127);
        }
        lift.update();
        tray.update();
    }
    lift.reset();
    tray.setTargetPowerControl(0, 127);
    pros::delay(750);
}