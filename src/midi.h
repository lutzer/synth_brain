#ifndef MIDI_H
#define MIDI_H

typedef unsigned char byte;

#define MIDI_DATA_MAX_SIZE 7

enum MidiCommand : unsigned char {
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
        byte status = 0;
        byte data[MIDI_DATA_MAX_SIZE];

        byte channel();
        MidiCommand command();
};

typedef void (*messageHandlerPtr)(MidiMessage);

class MidiReader {

    messageHandlerPtr messageHandler = 0;

    public:
        MidiMessage message;

        MidiReader(messageHandlerPtr handler);
        void parse(byte b);

};

#endif