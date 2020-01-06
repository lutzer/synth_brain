#include "voice.h"

#include "uart.h"

Voice::Voice(GateChangeHandler gateHandler) {
    this->channel = 0;
    this->updated = false;
    this->gateHandler = gateHandler;
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

void Voice::stopAll() {}

void Voice::update() {
    if (this->updated) {
        this->gateHandler(this->gate);
        this->updated = false;
    }
        
}