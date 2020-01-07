#ifndef MACROS_H
#define MACROS_H

// macros for setting pin outputs
#define SET_PIN_LOW(pin) {pin ## _PORT &= ~(1 << pin);}
#define SET_PIN_HIGH(pin) {pin ## _PORT |= 1 << pin;}
#define TOOGLE_PIN(pin) {pin ## _PORT ^= 1 << pin;}

// macro for enabling output or input
#define CONFIGURE_INPUT(pin) {pin ## _DDR &= ~(1 << pin);}
#define CONFIGURE_OUTPUT(pin) {pin ## _DDR |= (1 << pin);}

#endif