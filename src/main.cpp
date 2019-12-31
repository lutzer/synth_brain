#include "params.h"

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "utils/Ringbuffer.h"
#include "uart.h"
#include "midi.h"

RingBuffer serialData = RingBuffer(SERIAL_BUFFER_SIZE);

int main(void)
{

    DDRD |= 1 << LED1_PIN;
    DDRD |= 1 << LED2_PIN; 

    PORTD |= 1 << LED2_PIN; //turn on led1

    MidiReader midiIn;

    uart_init(); // init serial
    UCSR0B |= (1 << RXCIE0); // use serial interrupt

    sei(); // enable global interrupts

    while (1)
    {
        // toggle led off
        PORTD &= ~(1 << LED1_PIN);

        while (serialData.size() > 0) {

            // toggle on when receiving
            PORTD |= 1 << LED1_PIN;

            // read serial data
            cli();
            char c = serialData.pop();
            sei();

            // process midi
            if (midiIn.readByte(c)) {
                #ifdef DEBUG
                uart_putstring("midi:");
                uart_putstring(midiIn.message.toString());
                uart_putstring("\n");
                #endif
            }
        }
    }
    return 0;
}

/* 
* interrupt method reads uart messages and fills ringbuffer with them
*/
ISR(USART_RX_vect) {
    char c = uart_getchar();
    serialData.push(c);
}