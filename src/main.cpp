#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "params.h"

#include "uart.h"
#include "midi.h"
#include "encoder.h"
#include "voice.h"

#ifdef DEBUG
#include <stdio.h>
#endif

MidiReader *midiIn;
Encoder *encoder;
Voice *voice[2];

void onMidiMessage(MidiMessage message) {
    #ifdef DEBUG
    char msg[20] = "";
    sprintf(msg, "m:%01X,%u %u,%u\n", message.command(), message.channel(), message.data[0], message.data[1]);
    uart_putstring(msg);
    #endif

    for (int i = 0; i < 2; i++) {
        if (message.channel() == voice[i]->channel) {
            switch (message.command()) {
                case MidiCommand::Note_On:
                    voice[i]->playNote(message.data[0]);
                    break;
                case MidiCommand::Note_Off:
                    voice[i]->stopNote(message.data[0]);
                    break;
                case MidiCommand::Pitch_Bend:
                    voice[i]->setPitchBend(0);
                default:
                    break;
            }  
        }
    }
}

void onGateChange(unsigned char gate, bool enabled) {
    uart_putstring("gate triggered\n");
    if (gate == 0) {
        if (enabled)
            PORTD |= _BV(GATE0_PIN);
        else
            PORTD &= ~_BV(GATE0_PIN);
    } else if (gate == 1) {
        if (enabled)
            PORTD |= _BV(GATE1_PIN);
        else
            PORTD &= ~_BV(GATE1_PIN);
    }
}

void onEncoderChange(int change) {
    #ifdef DEBUG
    char msg[10] = "";
    sprintf(msg, "ec:%i\n", change); 
    uart_putstring(msg);
    #endif
}

int main(void) {

    // init gate pins
    DDRD |= _BV(GATE0_PIN);
    DDRD |= _BV(GATE1_PIN); 

    encoder = new Encoder(&onEncoderChange);
    midiIn = new MidiReader(&onMidiMessage);
    voice[0] = new Voice(0, &onGateChange);
    voice[1] = new Voice(1, &onGateChange);

    uart_init(); // init serial

    sei(); // enable global interrupts

    while (1)
    {
        // read midi data from rx port
        while (uart_data_available()) {
            char c = uart_getchar();
            midiIn->parse(c);
        }

        encoder->update();
    }
    return 0;
}