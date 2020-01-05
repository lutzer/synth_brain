#include "params.h"

#include <avr/io.h>
#include <string.h>
#include "uart.h"

#ifndef BAUD
#define BAUD 9600
#endif

#include <util/setbaud.h>

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
    //loop_until_bit_is_set(UCSR0A, RXC0);
    return UDR0;
}

bool uart_data_available() {
    return UCSR0A & (1 << RXC0);
}