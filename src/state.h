/*
 * @Author: Lutz Reiter - http://lu-re.de 
 * @Date: 2020-01-09 09:36:02 
 * @Last Modified by: Lutz Reiter - http://lu-re.de
 * @Last Modified time: 2020-01-09 09:53:33
 */

#ifndef STATE_H
#define STATE_H

#include "midi.h"

typedef unsigned char uchar;

enum CtrlState : uchar {
    INIT,

    CONTROL_CHANNEL1,
    CONTROL_CHANNEL1_SET,

    CONTROL_CHANNEL2,
    CONTROL_CHANNEL2_SET,

    CONTROL_MIDI_MODE,
    CONTROL_MIDI_MODE_SET,

    CALIBRATE_LOW,
    CALIBRATE_LOW_SET,
    
    CALIBRATE_HIGH,
    CALIBRATE_HIGH_SET
};

enum ActionName : uchar {
    ENCODER_TURN,
    ENCODER_PUSH
};

struct State {
    CtrlState status = INIT;
    MidiMode midiMode = SPLIT;
    uchar midiChannels[2] = {0x0, 0x1};
    uint16_t calibration[2] = { 0, 4095 };
};

class Statemachine {

    void reducer(ActionName action, int param) {}

    public:
        // statedata
        State state;

        // loads/saves in eeprom  
        void load() {}
        void save() {}

        // actions
        void encoder_push() {}
        void encoder_turn(uchar change) {}
};

#endif