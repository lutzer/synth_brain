#include "midi.h"
#include "uart.h"

MidiCommand MidiMessage::command() {
    return MidiCommand::Note_On;
}

byte MidiMessage::channel() {
    return 1;
}

char* MidiMessage::toString() {
    return "midi message";
}


MidiReader::MidiReader() {
    this->dataHead = 0;
    this->dataSize = 2;
}

bool MidiReader::readByte(byte b) {
    if (b & 0x80) { //status byte
        this->message.status = b;
        this->dataSize = 2;
        this->dataHead = 0;
    } else { // data byte
        this->message.data[this->dataHead++] = b;
    }

    if (this->dataSize > this->dataHead)
        // message incomplete
        return false;
    else {
        // message completed
        this->dataHead = 0;
        return true;
    }
}