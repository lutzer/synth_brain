#include "midi.h"

MidiCommand MidiMessage::command() {
    return MidiCommand::Note_On;
}

byte MidiMessage::channel() {
    return 1;
}

const char* MidiMessage::toString() {
    return "<msg>";
}

MidiReader::MidiReader(messageHandlerPtr handler) {
    this->messageHandler = handler;
}

void MidiReader::parse(byte b) {
    static unsigned char dataHead, dataSize;

    if (b & 0x80) { //status byte
        this->message.status = b;
        dataSize = 2;
        dataHead = 0;
    } else { // data byte
        this->message.data[dataHead++] = b;
    }

    // if msg completed call callback
    if (dataHead >= dataSize) {
        this->messageHandler(this->message);
        dataHead = 0;
    }

}
