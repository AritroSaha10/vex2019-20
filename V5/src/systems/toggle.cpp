#include "toggle.h"

Toggle::Toggle(std::vector<okapi::ControllerDigital> _buttons, okapi::Controller _controller, bool defaultState = false) {
    this->controller = _controller;
    this->buttons = _buttons;
    this->state = defaultState;
}

bool Toggle::checkState() {
    for(int i = 0; i < buttons.size(); i++) {
        if(!controller.getDigital(buttons[i])) {
            held = false;
            return state;
        }
    }

    if(!held) {
        state = !state;
    }
    held = true;
    return state;
}