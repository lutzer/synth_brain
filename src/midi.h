#ifndef MIDI_H
#define MIDI_H

typedef char byte;
typedef unsigned char uchar;

#define MIDI_DATA_MAX_SIZE 2

enum MidiCommand {
    Note_Off = 0x8,
    Note_On = 0x9,
    Aftertouch = 0xA,
    Continuous_Controller = 0xB,
    Patch_Change = 0xC,
    Channel_Pressure = 0xD,
    Pitch_Bend = 0xE,
    Other_Command = 0xF
};

class MidiMessage {
    public:
        byte status;
        byte data[MIDI_DATA_MAX_SIZE];

        byte channel();
        MidiCommand command();

        char* toString();
};

class MidiReader {

    uchar dataHead;
    uchar dataSize;

    public:
        MidiMessage message;

        MidiReader();
        bool readByte(byte b);
};

#endif