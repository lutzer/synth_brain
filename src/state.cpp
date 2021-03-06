/*
 * @Author: Lutz Reiter - http://lu-re.de 
 * @Date: 2020-01-21 09:45:06 
 * @Last Modified by: Lutz Reiter - http://lu-re.de
 * @Last Modified time: 2020-01-29 13:30:25
 */

#include <util/atomic.h>

#include "state.h"
#include "storage.h"

#include "utils/math.h"
#include "utils/timers.h"

#ifdef DEBUG
#include "utils/debug.h"
#endif

// eeprom registers
#define ADDRESS_MIDI_MODE (uint8_t*)0
#define ADDRESS_MIDI_CHANNELS (uint8_t*)10
#define ADDRESS_CALIBRATION_VALUES (uint8_t*)20

#define TIMEOUT_TIMER_OVERFLOWS 2500 // 1 overflow = 3,2ms

volatile uint16_t Statemachine::_triggerTimeoutOverflows = 0;

void timerTimeoutFunc() {
    Statemachine::_triggerTimeoutOverflows++;
}

Statemachine::Statemachine(StateChangeHandler handler) : handler(handler) {
    Timer2::addCallback(&timerTimeoutFunc);

    this->state = State();
}

void Statemachine::load() {
    this->state.midiMode = (MidiMode)storage_read_byte(ADDRESS_MIDI_MODE, 0);

    this->state.midiChannels[0] = storage_read_byte(ADDRESS_MIDI_CHANNELS, 0);
    this->state.midiChannels[1] = storage_read_byte(ADDRESS_MIDI_CHANNELS + 1, 1);

    this->state.calibration[0] = 50;
    for(uint8_t i = 1; i < NUMBER_OF_CALIBRATION_VALUES; i++)
        this->state.calibration[i] = storage_read_byte(ADDRESS_CALIBRATION_VALUES + (i-1), 50);

    #ifdef DEBUG
    debug_print("state loaded\n");
    #endif
    
    this->handler(this->state);
}

void Statemachine::save() {
    storage_write_byte(ADDRESS_MIDI_MODE, this->state.midiMode);
    storage_write_byte(ADDRESS_MIDI_CHANNELS, this->state.midiChannels[0]);
    storage_write_byte(ADDRESS_MIDI_CHANNELS + 1, this->state.midiChannels[1]);

    for(uint8_t i = 1; i < NUMBER_OF_CALIBRATION_VALUES; i++)
        storage_write_byte(ADDRESS_CALIBRATION_VALUES + (i-1), state.calibration[i]);

    #ifdef DEBUG
    debug_print("state saved\n");
    #endif
}

void Statemachine::reducer(ActionName action, int param) {
    #ifdef DEBUG
    debug_print("reducer: %i,%i\n", action, param);
    #endif

    switch (this->state.status) {
        case CtrlState::INIT:
            if (action == ActionName::ENCODER_TURN) {
                this->state.menuStatus = (MenuState)constrain(this->state.menuStatus + param, 0, 6);
            }
            if (action == ActionName::ENCODER_PUSH && this->state.menuStatus == MenuState::MENU_CHANNEL1) {
                this->state.status = CtrlState::CONTROL_CHANNEL1;
            }
            if (action == ActionName::ENCODER_PUSH && this->state.menuStatus == MenuState::MENU_CHANNEL2) {
                this->state.status = CtrlState::CONTROL_CHANNEL2;
            }
            if (action == ActionName::ENCODER_PUSH && this->state.menuStatus == MenuState::MENU_CALIBRATE_A1) {
                this->state.status = CtrlState::CALIBRATE_A1;
            }
            if (action == ActionName::ENCODER_PUSH && this->state.menuStatus == MenuState::MENU_CALIBRATE_A2) {
                this->state.status = CtrlState::CALIBRATE_A2;
            }
            if (action == ActionName::ENCODER_PUSH && this->state.menuStatus == MenuState::MENU_CALIBRATE_A3) {
                this->state.status = CtrlState::CALIBRATE_A3;
            }
            if (action == ActionName::ENCODER_PUSH && this->state.menuStatus == MenuState::MENU_CALIBRATE_A4) {
                this->state.status = CtrlState::CALIBRATE_A4;
            }
            if (action == ActionName::TIMEOUT && this->state.menuStatus != MenuState::MENU_OFF) {
                this->state.menuStatus = MenuState::MENU_OFF;
            }
            this->resetTimeout();
            break;
        case CtrlState::CONTROL_CHANNEL1:
            if (action == ActionName::ENCODER_TURN) {
                this->state.midiChannels[0] = (MenuState)constrain(this->state.midiChannels[0] + param, 0, 15);
            }
            if (action == ActionName::ENCODER_PUSH) {
                this->state.status = CtrlState::INIT;
                this->saveChanges = true;
            }
            if (action == ActionName::TIMEOUT) {
                this->state.status = CtrlState::INIT;
                this->state.menuStatus = MenuState::MENU_OFF;
                this->saveChanges = true;
            }
            this->resetTimeout();
            break;
        case CtrlState::CONTROL_CHANNEL2:
            if (action == ActionName::ENCODER_TURN) {
                this->state.midiChannels[1] = (MenuState)constrain(this->state.midiChannels[1] + param, 0, 15);
            }
            if (action == ActionName::ENCODER_PUSH) {
                this->state.status = CtrlState::INIT;
                this->saveChanges = true;
            }
            if (action == ActionName::TIMEOUT) {
                this->state.status = CtrlState::INIT;
                this->state.menuStatus = MenuState::MENU_OFF;
                this->saveChanges = true;
            }
            this->resetTimeout();
            break;
        case CtrlState::CALIBRATE_A1:
        case CtrlState::CALIBRATE_A2:
        case CtrlState::CALIBRATE_A3:
        case CtrlState::CALIBRATE_A4:
            uint8_t tableIndex = this->state.status - CtrlState::CALIBRATE_A1 + 1;
            if (action == ActionName::ENCODER_TURN) {
                this->state.calibration[tableIndex] = constrain(this->state.calibration[tableIndex] + param, 0, 99);
            }
            if (action == ActionName::ENCODER_PUSH) {
                this->state.status = CtrlState::INIT;
                this->saveChanges = true;
            }
            if (action == ActionName::TIMEOUT) {
                this->state.status = CtrlState::INIT;
                this->state.menuStatus = MenuState::MENU_OFF;
                this->saveChanges = true;
            }
            this->resetTimeout();
            break;
    }

    // always accept mode button pushes
    if (action == ActionName::MODE_BUTTON_PUSH) {
        this->state.midiMode = (MidiMode)((this->state.midiMode + 1) % 3);
        this->saveChanges = true;
    }

    // checks regulary if it needs to save changes
    if (action == ActionName::TIMEOUT && this->saveChanges) {
        this->saveChanges = false;
        this->save();
    }

    this->stateChanged = true;  
}

void Statemachine::update() {
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        if (Statemachine::_triggerTimeoutOverflows > TIMEOUT_TIMER_OVERFLOWS) {
            this->reducer(ActionName::TIMEOUT);
            // Statemachine::_triggerTimeoutOverflows = 0;
        }
    }
    if (this->stateChanged) {
        this->handler(this->state);  
        this->stateChanged = false;
    }
}

void Statemachine::resetTimeout() {
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        Statemachine::_triggerTimeoutOverflows = 0;
    }
}