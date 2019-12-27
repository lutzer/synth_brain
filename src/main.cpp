/**
 * Copyright (C) PlatformIO <contact@platformio.org>
 * See LICENSE for details.
 */

#include <avr/io.h>
#include <util/delay.h>

#define LED1_PIN 7

int main(void)
{
    // make the LED pin an output for PORTB5
    DDRD = 1 << LED1_PIN;

    while (1)
    {
        _delay_ms(500);

        // toggle the LED
        PORTD ^= 1 << LED1_PIN;
    }

    return 0;
}