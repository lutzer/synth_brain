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
        _delay_ms(500);

        // toggle the LED
        PORTD ^= 1 << LED1_PIN;

        uart_putchar('t');
    }

    return 0;
}