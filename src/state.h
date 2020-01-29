/*
 * @Author: Lutz Reiter - http://lu-re.de 
 * @Date: 2020-01-09 09:36:02 
 * @Last Modified by: Lutz Reiter - http://lu-re.de
 * @Last Modified time: 2020-01-29 13:19:18
 */

#ifndef STATE_H
#define STATE_H

#include "voice.h"
#include "calibration_table.h"

#define NUMBER_OF_MIDI_CHANNELS 2

typedef unsigned char uchar;

enum CtrlState : uchar {
    INIT = 0,
    CONTROL_CHANNEL1 = 1,
    CONTROL_CHANNEL2 = 2,
    CALIBRATE_A1 = 3,
    CALIBRATE_A2 = 4,
    CALIBRATE_A3 = 5,
    CALIBRATE_A4 = 6
};

enum MenuState : uchar {
    MENU_OFF = 0,
    MENU_CHANNEL1 = 1,
    MENU_CHANNEL2 = 2,
    MENU_CALIBRATE_A1 = 3,
    MENU_CALIBRATE_A2 = 4,
    MENU_CALIBRATE_A3 = 5,
    MENU_CALIBRATE_A4 = 6,
};

enum ActionName : uchar {
    ENCODER_TURN,
    ENCODER_PUSH,
    MODE_BUTTON_PUSH,
    TIMEOUT
};

struct State {
    CtrlState status = INIT;
    MenuState menuStatus = MENU_OFF;

    uint8_t calibration[NUMBER_OF_CALIBRATION_VALUES];

    MidiMode midiMode = SPLIT;
    uchar midiChannels[NUMBER_OF_MIDI_CHANNELS] = {0x0, 0x1};
};

typedef void (*StateChangeHandler)(const State &state);

class Statemachine {
    
    State state;
    bool stateChanged = false;
    
    StateChangeHandler handler;

    bool saveChanges = false;

    void reducer(ActionName action, int param = 0);

    public:

        static volatile uint16_t _triggerTimeoutOverflows;

        Statemachine(StateChangeHandler handler);

        // loads/saves in eeprom  
        void load();
        void save();

        // actions
        void encoder_push() {
            this->reducer(ENCODER_PUSH);
        }

        void encoder_turn(int change) {
            this->reducer(ENCODER_TURN, change);
        }

        void mode_button_push() {
            this->reducer(MODE_BUTTON_PUSH);
        }

        void update();

        void resetTimeout();
};

#endif