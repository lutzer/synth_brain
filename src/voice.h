/*
 * @Author: Lutz Reiter - http://lu-re.de 
 * @Date: 2020-01-06 19:12:26 
 * @Last Modified by: Lutz Reiter - http://lu-re.de
 * @Last Modified time: 2020-01-29 13:24:13
 */

#ifndef VOICE_H
#define VOICE_H

#include "dac.h"
#include "midi.h"
#include "calibration_table.h"

#include "utils/ringbuffer.h"

#define NUMBER_OF_VOICES 2

typedef unsigned char uchar;
typedef unsigned int uint;

typedef void (*GateChangeHandler)(bool enabled, uchar dacChannel);

enum MidiMode : uchar {
    SPLIT = 0x0,
    MONOPHONIC = 0x1,
    PARAPHONIC = 0x2
};

class Voice {
    GateChangeHandler gateHandler;
  
    public:
        uchar channel;
        uchar note;
        int pitchbend;

        bool gate;
        bool updated;

        Dac *dac;
        uchar dacChannel;

        CalibrationTable *calibrationTable;
  
        Voice(Dac *dac, uchar dacChannel, CalibrationTable *calibrationTable, GateChangeHandler gateHandler);
        void playNote(uchar note);
        void stopNote(uchar note);
        void setPitchBend(int bend);
        void stopAll();
        void playLastNote();

        void setChannel(uchar channel);

        void update();
};

class VoiceGroup {
    Voice *voices[NUMBER_OF_VOICES];
    uchar numberOfVoices = 0;

    MidiMode midiMode;

    public:
        VoiceGroup();

        void addVoice(Voice *voice);
        void setMidiMode(const MidiMode mode, const uchar *midiChannels);

        void handle(MidiMessage msg);
        void retrigger();

        void update();

};

#endif