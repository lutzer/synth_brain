#include "params.h"

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "utils/Ringbuffer.h"
#include "uart.h"
#include "midi.h"

MidiReader *midiIn;

void processMidiMessage(MidiMessage message) {
    #ifdef DEBUG
    uart_putstring("midi:");
    uart_putstring(message.toString());
    uart_putstring("\n");
    #endif
}

int main(void) {

    // init gate pins
    DDRD |= _BV(LED1_PIN);
    DDRD |= _BV(LED2_PIN); 

    PORTD |= _BV(LED2_PIN); // turn on led1

    uart_init(); // init serial

    midiIn = new MidiReader(&processMidiMessage);

    while (1)
    {
        PORTD &= ~_BV(LED1_PIN); // turn off led2

        // read midi data from rx port
        while (uart_data_available()) {
            PORTD |= _BV(LED2_PIN); // turn on led2 when receiving
            char c = uart_getchar();
            midiIn->parse(c);
        }
    }
    return 0;
}