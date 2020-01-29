/*
 * @Author: Lutz Reiter - http://lu-re.de 
 * @Date: 2020-01-06 19:12:14 
 * @Last Modified by: Lutz Reiter - http://lu-re.de
 * @Last Modified time: 2020-01-29 13:25:15
 */

#include "voice.h"

#include "config.h"

Voice::Voice(Dac *dac, uchar dacChannel, CalibrationTable *calibrationTable, GateChangeHandler gateHandler) {
    this->channel = 0;
    this->updated = false;
    this->gateHandler = gateHandler;

    this->note = 0;
    this->gate = 0;
    this->pitchbend = 0;

    this->dac = dac;
    this->dacChannel = dacChannel;

    this->calibrationTable = calibrationTable;
}

void Voice::playNote(uchar note) {
    if (this->note != note || !this->gate) {
        this->updated = true;
    }
    this->note = note;
    this->gate = true;
}

void Voice::stopNote(uchar note) {
    if (this->note == note) {
        this->gate = false;
        this->updated = true;
    }
}

void Voice::setPitchBend(int bend) {
    bend -= 8192;

    if (this->pitchbend != bend) {
        this->pitchbend = bend;
        this->updated = true;
    }
}

void Voice::stopAll() {
    this->gate = false;
    this->updated = true;
}

void Voice::setChannel(uchar channel) {
    if (this->channel != channel) {
        this->channel = channel;
        this->stopAll();
    }
}

void Voice::playLastNote() {
    this->updated = true;
}

void Voice::update() {
    if (this->updated) {
        uint16_t val = this->calibrationTable->getValue(this->note);
        this->dac->send(this->dacChannel, val);
        this->gateHandler(this->gate, this->dacChannel);
        this->updated = false;
    }
}

void VoiceGroup::addVoice(Voice *voice) {
    this->voices[this->numberOfVoices++] = voice;
}

void VoiceGroup::setMidiMode(const MidiMode mode,const uchar *midiChannels) {
    this->midiMode = mode;
    for (uint8_t i = 0; i < numberOfVoices; i++) {
        voices[i]->setChannel(midiChannels[i]);
    }
}

void VoiceGroup::update() {
    for (uint8_t i = 0; i < numberOfVoices; i++) {
        voices[i]->update();
    }
}

void VoiceGroup::retrigger() {
    for (uint8_t i = 0; i < numberOfVoices; i++)
        voices[i]->playLastNote();
}

void VoiceGroup::handle(MidiMessage msg) {
    MidiCommand cmd = msg.command();
    for (uint8_t i = 0; i < numberOfVoices; i++) {
        if (cmd == MidiCommand::System_Reset) {
            voices[i]->stopAll();
            continue;
        }

        if (this->midiMode == MidiMode::SPLIT) {
            // debug_print("split\n");
            if (msg.channel() == voices[i]->channel) {
                // debug_print("c%i\n",i);
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
        } else if (this->midiMode == MidiMode::MONOPHONIC) {
            // debug_print("mono\n");
            if (msg.channel() == voices[0]->channel) { // reacts to midi msg only on channel of voice1
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
}