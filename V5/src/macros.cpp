#include "macros.h"
#include "globals.h"
#include "main.h"

bool stacking = false;
bool lifting = false;

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
    lifting = true;
    if(stacking) {
        return;
    }
}

void liftToLow() {
    if(stacking) {
        return;
    }
}

void incrementLift(int dir) {
    if(lifting) {
        return;
    }
    if(dir == 0) {
        lift.lock();
    }
    if(dir == -1) {
        
    }
}