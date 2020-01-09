/*
 * @Author: Lutz Reiter - http://lu-re.de 
 * @Date: 2020-01-06 19:13:50 
 * @Last Modified by: Lutz Reiter - http://lu-re.de
 * @Last Modified time: 2020-01-09 09:43:56
 */

#include "midi.h"

char calculateMidiDataSize(MidiCommand cmd) {
    switch (cmd) {
        case MidiCommand::Note_On:
        case MidiCommand::Note_Off:
        case MidiCommand::Pitch_Bend:
            return 2;
        case MidiCommand::System_Reset:
            return 0;
        // filter out all other messages
        default:
            return -1;
    }
};

MidiMessage::MidiMessage() {
    this->status = 0;
    for(int i = 0;i < MIDI_DATA_MAX_SIZE; i++)
        this->data[i] = 0;
}

MidiCommand MidiMessage::command() {
    return (MidiCommand)(this->status < 0xF0 ? status & 0xF0 : status);
}

byte MidiMessage::channel() {
    return this->status & 0x0F;
}

MidiMessage MidiMessage::clone() {
    MidiMessage msg;
    msg.status = this->status;
    for(int i = 0;i < MIDI_DATA_MAX_SIZE; i++)
        msg.data[i] = this->data[i];
    return msg;
}

MidiReader::MidiReader(messageHandlerPtr handler) {
    this->messageHandler = handler;
}

void MidiReader::parse(byte b) {
    static unsigned char dataHead = 0, dataSize = -1;

    if (b & 0x80) { //status byte
        message.status = b;
        dataHead = 0;
        dataSize = calculateMidiDataSize(message.command());
    } else if (dataSize > 0) { // data byte
        message.data[dataHead++] = b;
    }

    // if msg is complete call callback
    if (dataSize > -1 && dataHead >= dataSize) {
        this->messageHandler(message);
        dataHead = 0;
    }
}

void MidiHandler::addVoice(Voice *voice) {
    this->voices[this->numberOfVoices++] = voice;
}

void MidiHandler::handle(MidiMessage msg) {
    byte cmd = msg.command();

    for (int i = 0; i < numberOfVoices; i++) {
        if (cmd == MidiCommand::System_Reset) {
            voices[i]->stopAll();
        } else if (msg.channel() == voices[i]->channel) {
            switch (cmd) {
                case MidiCommand::Note_On:
                    voices[i]->playNote(msg.data[0]);
                    break;
                case MidiCommand::Note_Off:
                    voices[i]->stopNote(msg.data[0]);
                    break;
                case MidiCommand::Pitch_Bend:
                    voices[i]->setPitchBend(0);
                default:
                    break;
            }  
        }
    }
}
