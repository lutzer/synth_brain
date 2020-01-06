#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "params.h"

#include "uart.h"
#include "midi.h"
#include "encoder.h"
#include "voice.h"

#include "utils/macros.h"

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

void onGate0Change(bool enabled) {
    if (enabled)
        PORTD |= _BV(GATE0_PIN);
    else
        PORTD &= ~_BV(GATE0_PIN);
}

void onGate1Change(bool enabled) {
    if (enabled)
        PORTD |= _BV(GATE1_PIN);
    else
        PORTD &= ~_BV(GATE1_PIN);
    
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
    voice[0] = new Voice(&onGate0Change);
    voice[1] = new Voice(&onGate1Change);

    uart_init(); // init serial

    sei(); // enable global interrupts

    while (1)
    {
        // read midi data from rx port
        while (uart_data_available()) {
            char c = uart_getchar();
            midiIn->parse(c);
        }

        voice[0]->update();
        voice[1]->update();

        encoder->update();
    }
    return 0;
}