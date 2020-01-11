/*
 * @Author: Lutz Reiter - http://lu-re.de 
 * @Date: 2020-01-09 14:36:45 
 * @Last Modified by: Lutz Reiter - http://lu-re.de
 * @Last Modified time: 2020-01-09 16:58:25
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/atomic.h>

#include "button.h"

#include "config.h"
#include "utils/macros.h"
#include "utils/math.h"
#include "utils/debug.h"

#define COUNTER_OVERFLOWS 78 // with a clock division of 1024 that should roughly be 4 ms

volatile uint32_t Buttons::_static_counters = 0;

Buttons::Buttons(ButtonsChangeHandler handler) {
    this->handler = handler;

    configure_input(ENCODER_BUTTON);
    set_pin_high(ENCODER_BUTTON);

    //create buttonmask
    this->buttonMask = _BV(ENCODER_BUTTON);

    // setup timer2 for debouncing
    TCNT2 = 0x0; // set timer to 0
    TCCR2A = 0; // set to overflow mode
    TIMSK2 = (1 << TOIE2);  // enable overflow interrupt

    // start timer with clock/256 divider
    TCCR2B = _BV(CS22) | _BV(CS21) | _BV(CS22);

}

void Buttons::update() {
    static uchar state = 0;
    // for now only read one button
    uchar reading = BUTTONS_REG & this->buttonMask; 

    // sth changed
    if (uchar changes = state ^ reading) {
        ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
            uint8_t low = Buttons::_static_counters;
            uint8_t high = Buttons::_static_counters >> 8;

            // check if both bits are 1
            if ((low & changes) && (high & changes)) {
                // set debounce counter to zero on the changed bits
                Buttons::_static_counters = (low & ~changes) | ((high & ~changes) << 8);
            } else {
                return; // dont do anything
            }
        }
        state = reading;
        this->handler(changes,~state);
    }
}

ISR (TIMER2_OVF_vect) {
    unsigned static int overflows = 0;

    // is called every 4 ms
    if (overflows % COUNTER_OVERFLOWS == 0) {
        uint8_t low = Buttons::_static_counters;
        uint8_t high = Buttons::_static_counters >> 8;

        // bitwise vertical increment until count to 4 0b11 in each column
        Buttons::_static_counters = (low | high) | ((~(low | high) | high) << 8);
        overflows = 0;
    }
}


// Button::Button(
//     volatile uint8_t *cPort, 
//     volatile uint8_t *cDDR, 
//     volatile uint8_t *cReg, 
//     uint8_t cPin, 
//     ButtonChangeHandler handler) : cPin(cPin), cReg(cReg) {

//         // configure as input
//         *cDDR &= ~( 1 << cPin);

//         // set pin high
//         *cPort |= (1 << cPin);

//         this->handler = handler;
//         this->pushed = false;
// }

// void Button::update() {
//     char reading = *cReg & ( 1 << cPin);
//     if (!reading != pushed)
//         this->handler(!reading);
//     pushed = !reading;
// }