#ifndef MACROS_H
#define MACROS_H

// macros for setting pin outputs
#define set_pin_low(pin) {pin ## _PORT &= ~(1 << pin);}
#define set_pin_high(pin) {pin ## _PORT |= 1 << pin;}
#define toggle_pin(pin) {pin ## _PORT ^= 1 << pin;}

// macro for enabling output or input
#define configure_input(pin) {pin ## _DDR &= ~(1 << pin);}
#define configure_output(pin) {pin ## _DDR |= (1 << pin);}

// macros for reading pins
#define read_pin(pin) (pin ## _REG & (1 << pin))

#endif