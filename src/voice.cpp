/*
 * @Author: Lutz Reiter - http://lu-re.de 
 * @Date: 2020-01-06 19:12:14 
 * @Last Modified by: Lutz Reiter - http://lu-re.de
 * @Last Modified time: 2020-01-07 17:05:19
 */

#include "voice.h"

#include "uart.h"

Voice::Voice(GateChangeHandler gateHandler, Dac *dac, uchar dacChannel) {
    this->channel = 0;
    this->updated = false;
    this->gateHandler = gateHandler;

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

void Voice::setPitchBend(uint bend) {}

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
        this->gateHandler(this->gate);
        this->updated = false;
        this->dac->send(this->dacChannel, this->note * 32);
    }
        
}