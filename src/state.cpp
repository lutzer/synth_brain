#include "state.h"

#include "utils/math.h"

void Statemachine::reducer(ActionName action, int param) {
    switch (this->state.status) {
        case CtrlState::INIT:
            if (action == ActionName::ENCODER_TURN) {
                this->state.menuStatus = (MenuState)constrain(this->state.menuStatus + param, 0, 4);
            }
            if (action == ActionName::ENCODER_PUSH && this->state.menuStatus == MenuState::MENU_CHANNEL1) {
                this->state.status = CtrlState::CONTROL_CHANNEL1;
            }
            if (action == ActionName::ENCODER_PUSH && this->state.menuStatus == MenuState::MENU_CHANNEL2) {
                this->state.status = CtrlState::CONTROL_CHANNEL2;
            }
            break;
        case CtrlState::CONTROL_CHANNEL1:
            if (action == ActionName::ENCODER_TURN) {
                this->state.midiChannels[0] = (MenuState)constrain(this->state.midiChannels[0] + param, 0, 15);
            }
            if (action == ActionName::ENCODER_PUSH) {
                this->state.status = CtrlState::INIT;
            }
        case CtrlState::CONTROL_CHANNEL2:
            if (action == ActionName::ENCODER_TURN) {
                this->state.midiChannels[1] = (MenuState)constrain(this->state.midiChannels[1] + param, 0, 15);
            }
            if (action == ActionName::ENCODER_PUSH) {
                this->state.status = CtrlState::INIT;
            }
    }

    // always accept mode button pushes
    if (action == ActionName::MODE_BUTTON_PUSH) {
        this->state.midiMode = (MidiMode)((this->state.midiMode + 1) % 3);
    }

    this->handler(this->state);    
}