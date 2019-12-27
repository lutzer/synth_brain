#include <avr/io.h>
#include <string.h>
#include "uart.h"

#ifndef BAUD
// midi baud rate is 31250 bps
#define BAUD 31250
#endif

#include <util/setbaud.h>

void uart_init(void) {
    UBRR0H = UBRRH_VALUE;
    UBRR0L = UBRRL_VALUE;
    
    UCSR0A &= ~(_BV(U2X0));

    UCSR0C = _BV(UCSZ01) | _BV(UCSZ00); /* 8-bit data */ 
    UCSR0B = _BV(RXEN0) | _BV(TXEN0);   /* Enable RX and TX */    
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
    loop_until_bit_is_set(UCSR0A, RXC0);
    return UDR0;
}

bool uart_data_available() {
    return UCSR0A & (1 << RXC0);
}