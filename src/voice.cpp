/*
 * @Author: Lutz Reiter - http://lu-re.de 
 * @Date: 2020-01-06 19:12:14 
 * @Last Modified by: Lutz Reiter - http://lu-re.de
 * @Last Modified time: 2020-01-07 19:50:22
 */

#include "voice.h"

#include "config.h"

#define DAC_STEPS_PER_SEMITONE 32
#define PITCH_BEND_DIVISOR 8192 / DAC_STEPS_PER_SEMITONE / PITCH_BEND_SEMITONES

Voice::Voice(Dac *dac, uchar dacChannel, GateChangeHandler gateHandler) {
    this->channel = 0;
    this->updated = false;
    this->gateHandler = gateHandler;

    this->note = 0;
    this->gate = 0;
    this->pitchbend = 0;

    this->dac = dac;
    this->dacChannel = dacChannel;
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
    bend /= PITCH_BEND_DIVISOR;

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

void Voice::update() {
    if (this->updated) {
        this->dac->send(this->dacChannel, this->note * 32 + this->pitchbend);
        this->gateHandler(this->gate);
        this->updated = false;
    }
        
        
}