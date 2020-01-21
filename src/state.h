/*
 * @Author: Lutz Reiter - http://lu-re.de 
 * @Date: 2020-01-09 09:36:02 
 * @Last Modified by: Lutz Reiter - http://lu-re.de
 * @Last Modified time: 2020-01-21 12:49:33
 */

#ifndef STATE_H
#define STATE_H

#include "midi.h"

#define TIMEOUT_TIMER_OVERFLOWS 2500 //1 overflow = 3,2ms

typedef unsigned char uchar;

enum CtrlState : uchar {
    INIT,
    CONTROL_CHANNEL1,
    CONTROL_CHANNEL2,
    //CALIBRATE_LOW,
    //CALIBRATE_HIGH,
};

enum MenuState : uchar {
    MENU_OFF = 0,
    MENU_CHANNEL1 = 1,
    MENU_CHANNEL2 = 2,
    MENU_CALIBRATE_LOW = 3,
    MENU_CALIBRATE_HIGH = 4
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

    uint16_t calibration[2] = { 0, 4095 };

    MidiMode midiMode = SPLIT;
    uchar midiChannels[2] = {0x0, 0x1};
};

typedef void (*StateChangeHandler)(const State &state);

class Statemachine {
    
    State state;
    StateChangeHandler handler;

    void reducer(ActionName action, int param = 0);

    public:

        static volatile uint16_t _triggerTimeoutOverflows;

        Statemachine(StateChangeHandler handler);

        // loads/saves in eeprom  
        void load() {
            this->handler(this->state);
        }
        void save() {}

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