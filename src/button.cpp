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
#include "utils/timers.h"

volatile uint32_t Buttons::_static_counters = 0;

void buttonTimerFunc() {
    unsigned static int overflows = 0;
    overflows++;

    if (overflows % BUTTON_COUNTER_OVERFLOWS == 0) {
        uint8_t low = Buttons::_static_counters;
        uint8_t high = Buttons::_static_counters >> 8;

        // bitwise vertical increment until count to 4 = 0b11 in each column
        Buttons::_static_counters = (low | high) | ((~(low | high) | high) << 8);
        overflows = 0;
    }
}

Buttons::Buttons(ButtonsChangeHandler handler) {
    this->handler = handler;

    configure_input(ENCODER_BUTTON);
    set_pin_high(ENCODER_BUTTON);

    //create buttonmask
    this->buttonMask = _BV(ENCODER_BUTTON);

    // add timer callback to debounce buttons
    Timer2::addCallback(&buttonTimerFunc);
}

void Buttons::update() {
    static uchar state = 0;
    
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