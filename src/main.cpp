/**
 * Copyright (C) PlatformIO <contact@platformio.org>
 * See LICENSE for details.
 */

#include <avr/io.h>
#include <util/delay.h>
#include "uart.h"

// led is on PIN D7
#define LED1_PIN 7

int main(void)
{
    DDRD = 1 << LED1_PIN;

    uart_init();

    while (1)
    {
        
        // toggle led off
        PORTD &= ~(1 << LED1_PIN);

        if (uart_data_available()) {

            // toggle on when receiving
            PORTD |= 1 << LED1_PIN;

            char c = uart_getchar();
            uart_putstring("received:");
            uart_putchar(c);
            uart_putstring("\n");
        }

        // _delay_ms(500)
    }

    return 0;
}