#include "voice.h"

#include "uart.h"

Voice::Voice(uchar midiChannel) {
    this->channel = midiChannel;
}

void Voice::playNote(uchar note) {
    //uart_putstring("### play note \n");
}
void Voice::stopNote(uchar note) {}
void Voice::setPitchBend(uint bend) {}