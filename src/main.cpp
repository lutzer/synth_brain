#include <avr/io.h>
#include <util/delay.h>
#include "uart.h"

// leds are on PIN D7 and D6
#define LED1_PIN 7
#define LED2_PIN 6

int main(void)
{
    DDRD |= 1 << LED1_PIN;
    DDRD |= 1 << LED2_PIN; 

    uart_init();

    PORTD |= 1 << LED2_PIN;

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