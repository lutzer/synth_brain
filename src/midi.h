/*
 * @Author: Lutz Reiter - http://lu-re.de 
 * @Date: 2020-01-06 19:13:43 
 * @Last Modified by: Lutz Reiter - http://lu-re.de
 * @Last Modified time: 2020-01-09 09:44:01
 */

#ifndef MIDI_H
#define MIDI_H

#include "voice.h"

typedef unsigned char byte;

#define MIDI_DATA_MAX_SIZE 7
#define NUMBER_OF_VOICES 2

enum MidiCommand : unsigned char {
    Note_Off = 0x80,
    Note_On = 0x90,
    Aftertouch = 0xA0,
    Continuous_Controller = 0xB0,
    Patch_Change = 0xC0,
    Channel_Pressure = 0xD0,
    Pitch_Bend = 0xE0,
    System_Exclusive = 0xF0,
    Time_Code = 0xF1,
    Song_Position_Pointer = 0xF2,
    Song_Select = 0xF3,
    Tune_Request = 0xF6,
    End_of_SysX = 0xF7,
    Timing_Clock = 0xF8,
    Start = 0xFA,
    Continue = 0xFB,
    Stop = 0xFC,
    System_Reset = 0xFF
};

enum MidiMode : uchar {
    SPLIT = 0x0,
    MONOPHONIC = 0x1,
    PARAPHONIC = 0x2
};

class MidiMessage {

    public:
        MidiMessage();

        byte status = 0;
        byte data[MIDI_DATA_MAX_SIZE];

        byte channel();
        MidiCommand command();

        MidiMessage clone();
};

typedef void (*messageHandlerPtr)(MidiMessage);

class MidiReader {
    messageHandlerPtr messageHandler = 0;
    MidiMessage message;

    public:
        MidiReader(messageHandlerPtr handler);
        void parse(byte b);
};

class MidiHandler {
    Voice *voices[NUMBER_OF_VOICES];
    uchar numberOfVoices = 0;

    MidiMode midiMode;

    public:
        void addVoice(Voice *voice);
        void setMidiMode(const MidiMode mode, const uchar *midiChannels);

        void handle(MidiMessage msg);
};

#endif