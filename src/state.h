#ifndef STATE_H
#define STATE_H

typedef unsigned char uchar;

enum UiState : uchar {
    INIT = 0x0,
    CONTROL_CHANNEL1 = 0x1,
    CONTROL_CHANNEL2 = 0x2,
    CONTROL_MODE = 0x3,
    CALIBRATE_LOW = 0x4,
    CALIBRATE_HIGH = 0x5
};

enum MidiMode : uchar {
    SPLIT = 0x0,
    MONOPHONIC = 0x1,
    PARAPHONIC = 0x2
};

class State {
    public:
        UiState ui = INIT;

        MidiMode midiMode = SPLIT;
        uchar midiChannels[2] = {0x0, 0x1};

        uint16_t calibration[2] = { 0, 4095 };

        void loadSettings();
        void saveSettings();
};

#endif