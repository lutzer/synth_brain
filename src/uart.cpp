/*
 * @Author: Lutz Reiter - http://lu-re.de 
 * @Date: 2020-01-06 19:13:35 
 * @Last Modified by: Lutz Reiter - http://lu-re.de
 * @Last Modified time: 2020-01-07 18:35:07
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/atomic.h>
#include <string.h>

#ifdef DEBUG
#include <stdio.h>
#endif

#include "config.h"
#include "uart.h"
#include "utils/ringbuffer.h"

#ifndef BAUD
#define BAUD 9600
#endif

#ifndef UART_BUFFER_SIZE
#define UART_BUFFER_SIZE 128
#endif

#include <util/setbaud.h>


RingBuffer<uchar> _static_uart_buffer_rx0(UART_BUFFER_SIZE);

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

#ifdef DEBUG
void uart_putchar(char c) {
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        loop_until_bit_is_set(UCSR0A, UDRE0);
        UDR0 = c;
    }
}

void uart_putfloat(float f) {
    char msg[8] = "";
    sprintf(msg, "%f", (double)f);
    uart_putstring(msg);
}

void uart_putstring(const char *s) {
    for (size_t i=0; i<strlen(s); i++) {
        uart_putchar(s[i]);
    }
}
#endif

char uart_getchar() {
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        return _static_uart_buffer_rx0.pop();
    }
    return 0;
}

bool uart_data_available() {
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        return !_static_uart_buffer_rx0.empty();
    }
    return 0;
}

ISR(USART_RX_vect)
{
    char c = UDR0;
    _static_uart_buffer_rx0.push(c);
}