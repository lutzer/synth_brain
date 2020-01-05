#include "params.h"
#include "midi.h"

char calculateDataSize(byte status) {
    switch (status >> 4) {
        case MidiCommand::Channel_Pressure:
        case MidiCommand::Patch_Change:
            return 1;
        case MidiCommand::Other_Command:
            if (status == 0xF1 || status == 0xF3)
                return 1;
            else if (status == 0xF2)
                return 2;
            else
                return 0;
        default:
            return 2;
    }
};

MidiCommand MidiMessage::command() {
    return (MidiCommand)(this->status >> 4);
}

byte MidiMessage::channel() {
    return this->status & 0x0F;
}

MidiReader::MidiReader(messageHandlerPtr handler) {
    this->messageHandler = handler;
}

void MidiReader::parse(byte b) {
    static unsigned char dataHead = 0, dataSize = 2;

    if (b & 0x80) { //status byte
        this->message.status = b;
        dataHead = 0;
        dataSize = calculateDataSize(b);
    } else if (this->message.status) { // data byte
        this->message.data[dataHead++] = b;
    }

    // if msg completed call callback
    if (dataHead >= dataSize) {
        this->messageHandler(this->message);
        dataHead = 0;
    }
}
