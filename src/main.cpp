#include "params.h"

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
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

void handleMidiMessage(MidiMessage message) {
    #ifdef DEBUG
    char msg[16] = "";
    sprintf(msg, "m:%i,%i %i,%i\n", message.command(), message.channel(), message.data[0], message.data[1]);
    uart_putstring(msg);
    #endif

    for (int i = 0; i < 1; i++) {
        if (message.channel() == voice[i]->channel) {
            switch (message.command()) {
                case MidiCommand::Note_On:
                    voice[i]->playNote(message.data[0]);
                    PORTD |= _BV(LED1_PIN);
                    break;
                case MidiCommand::Note_Off:
                    voice[i]->stopNote(message.data[0]);
                    PORTD &= ~_BV(LED1_PIN);
                    break;
                case MidiCommand::Pitch_Bend:
                    voice[i]->setPitchBend(0);
                default:
                    break;
            }  
        }
    }
}

void handleEncoderChange(int change) {
    #ifdef DEBUG
    char msg[10] = "";
    sprintf(msg, "ec:%i\n", change); 
    uart_putstring(msg);
    #endif
}

int main(void) {

    // init gate pins
    DDRD |= _BV(LED1_PIN);
    DDRD |= _BV(LED2_PIN); 

    encoder = new Encoder(&handleEncoderChange);
    midiIn = new MidiReader(&handleMidiMessage);
    voice[0] = new Voice(0);
    voice[1] = new Voice(1);

    PORTD |= _BV(LED2_PIN); // turn on led2 after init

    uart_init(); // init serial

    sei(); // enable global interrupts

    while (1)
    {
        // read midi data from rx port
        while (uart_data_available()) {
            char c = uart_getchar();
            // uart_putchar(c);
            // uart_putchar('.');
            midiIn->parse(c);
        }

        encoder->update();
    }
    return 0;
}