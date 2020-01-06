#include "voice.h"

#include "uart.h"

Voice::Voice(uchar gate, GateChangeHandler gateHandler) {
    this->channel = 0;
}

void Voice::playNote(uchar note) {
    this->gateHandler(this->gate, true);
    //uart_putstring("### play note \n");
}
void Voice::stopNote(uchar note) {
    this->gateHandler(this->gate, false);
}
void Voice::setPitchBend(uint bend) {}

 void Voice::stopAll() {}