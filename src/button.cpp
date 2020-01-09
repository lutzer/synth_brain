#include "button.h"

#include "config.h"
#include "utils/macros.h"
#include "utils/math.h"
#include "utils/debug.h"


Buttons::Buttons(ButtonsChangeHandler handler) {
    this->handler = handler;

    configure_input(ENCODER_BUTTON);
    set_pin_high(ENCODER_BUTTON);

    for(int i=0; i < NUMBER_OF_COUNTERS; i++)
        counters[i] = 0;
}

void Buttons::update() {
    // for now only read one button
    uchar reading = read_pin(ENCODER_BUTTON); 

    // sth changed
    if (states != reading) {
        // debug_print("changed: %01X,%01X,%i\n", states, reading, counters[0]);
        if (counters[0] >= DEBOUNCE_CYCLES) {
            // debug_print("reached timer\n");
            states = reading;
            this->handler(0,!states);
            counters[0] = 0;
        }
    }
    counters[0] = min(counters[0] + 1, DEBOUNCE_CYCLES);
}


Button::Button(
    volatile uint8_t *cPort, 
    volatile uint8_t *cDDR, 
    volatile uint8_t *cReg, 
    uint8_t cPin, 
    ButtonChangeHandler handler) : cPin(cPin), cReg(cReg) {

        // configure as input
        *cDDR &= ~( 1 << cPin);

        // set pin high
        *cPort |= (1 << cPin);

        this->handler = handler;
        this->pushed = false;
}

void Button::update() {
    char reading = *cReg & ( 1 << cPin);
    if (!reading != pushed)
        this->handler(!reading);
    pushed = !reading;
}