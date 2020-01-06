#ifndef MACROS_H
#define MACROS_H

#define SET_PIN_HIGH_I(port, ddr, pin) ddr |= 1 << pin
#define SET_PIN_HIGH(spec) SET_PIN_HIGH_I(spec)

#define SET_PIN_LOW_I(port, ddr, pin) ddr &= ~(1 << pin)
#define SET_PIN_LOW(spec) SET_PIN_LOW_I(spec)

#endif