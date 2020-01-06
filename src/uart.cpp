#include "params.h"

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/atomic.h>
#include <string.h>
#include "uart.h"
#include "utils/Ringbuffer.h"

#include <util/setbaud.h>

RingBuffer UART_BUFFER_RX0(UART_BUFFER_SIZE);

void uart_init() {
    UBRR0H = UBRRH_VALUE;
    UBRR0L = UBRRL_VALUE;
    
    UCSR0A &= ~(_BV(U2X0));

    #ifdef DEBUG
        // Enable RX and TX 
        UCSR0B = _BV(RXEN0) | _BV(TXEN0);
    #else
        // Enable only RX 
        UCSR0B = _BV(RXEN0); 
    #endif

    UCSR0C = _BV(UCSZ01) | _BV(UCSZ00); /* 8-bit data */ 

    // enable interrupts
    UCSR0B |= (1 << RXCIE0);
      
}

void uart_putchar(char c) {
    loop_until_bit_is_set(UCSR0A, UDRE0);
    UDR0 = c;
}

void uart_putstring(const char *s) {
    for (size_t i=0; i<strlen(s); i++) {
        uart_putchar(s[i]);
    }
}

char uart_getchar() {
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        return UART_BUFFER_RX0.pop();
    }
    return 0;
}

bool uart_data_available() {
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        return UART_BUFFER_RX0.empty();
    }
    return 0;
}

ISR(USART_RX_vect)
{
    char c = UDR0; // Fetch the received byte value into the variable "ByteReceived"
    UART_BUFFER_RX0.push(c);
}