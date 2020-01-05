#include "params.h"

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "utils/Ringbuffer.h"
#include "uart.h"
#include "midi.h"
#include "encoder.h"

#ifdef DEBUG
#include <stdio.h>
#endif


MidiReader *midiIn;
Encoder *encoder;

void handleMidiMessage(MidiMessage message) {
    #ifdef DEBUG
    char msg[20] = "";
    sprintf(msg, "midi:%i,%i %i,%i\n", message.command(), message.channel(), message.data[0], message.data[1]); 
    uart_putstring(msg);
    #endif
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

    uart_init(); // init serial

    midiIn = new MidiReader(&handleMidiMessage);

    PORTD |= _BV(LED1_PIN); // turn on led1 after init
    PORTD &= ~_BV(LED2_PIN); // turn off led2

    sei(); // enable global interrupts

    while (1)
    {
        //PORTD |= _BV(LED2_PIN); // turn on led2 when receiving

        // read midi data from rx port
        while (uart_data_available()) {
            PORTD |= _BV(LED2_PIN); // turn on led2 when receiving
            char c = uart_getchar();
            midiIn->parse(c);
        }

        encoder->update();
    }
    return 0;
}