/*
 * @Author: Lutz Reiter - http://lu-re.de 
 * @Date: 2020-01-06 19:12:14 
 * @Last Modified by: Lutz Reiter - http://lu-re.de
 * @Last Modified time: 2020-01-29 13:25:15
 */

#include "voice.h"

#include "config.h"

#include "utils/ringbuffer.h"

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

VoiceGroup::VoiceGroup(uchar maxVoices) {
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

    if (cmd == MidiCommand::System_Reset) {
        for (uint8_t i = 0; i < numberOfVoices; i++)
            voices[i]->stopAll();
        return;
    }



    if (this->midiMode == MidiMode::SPLIT) {
        for (uint8_t i = 0; i < numberOfVoices; i++) {
            if (msg.channel() == voices[i]->channel) {
                switch (cmd) {
                    case MidiCommand::Note_On:
                        voices[i]->playNote(msg.data[0]);
                        break;
                    case MidiCommand::Note_Off:
                        voices[i]->stopNote(msg.data[0]);
                        break;
                    case MidiCommand::Pitch_Bend:
                        voices[i]->setPitchBend(0);
                        break;
                    default:
                        break;
                }
            }
        }
    } else if (this->midiMode == MidiMode::MONOPHONIC) {
        for (uint8_t i = 0; i < numberOfVoices; i++) {
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
                        break;
                    default:
                        break;
                }
            }
        }  
    } else if (this->midiMode == MidiMode::PARAPHONIC) {
        if (msg.channel() == voices[0]->channel) { // reacts to midi msg only on channel of voice1
            static uchar currentVoice = 0;
            switch (cmd) {
                case MidiCommand::Note_On:
                    for (uint8_t i=0; i <= numberOfVoices; i++) {
                        Voice *voice = voices[(currentVoice + i) % numberOfVoices];
                        if (!voice->gate || i == numberOfVoices) {
                            voice->playNote(msg.data[0]);
                            currentVoice = (currentVoice + i) % numberOfVoices;
                            break;
                        }
                    }
                    break;
                case MidiCommand::Note_Off:
                    for (uint8_t i=0; i < numberOfVoices; i++) {
                        Voice *voice = voices[(currentVoice + numberOfVoices - 1) % numberOfVoices];
                        if (voice->note == msg.data[0]) {
                            voice->stopNote(msg.data[0]);
                            break;
                        }
                    }
                    break;
                case MidiCommand::Pitch_Bend:
                    voices[currentVoice]->setPitchBend(0);
                    break;
                default:
                    break;
            }
        }
    }
}