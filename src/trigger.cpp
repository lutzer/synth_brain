/*
 * @Author: Lutz Reiter - http://lu-re.de 
 * @Date: 2020-01-06 22:30:02 
 * @Last Modified by: Lutz Reiter - http://lu-re.de
 * @Last Modified time: 2020-01-08 01:34:46
 */

#include <avr/io.h>
#include <avr/interrupt.h>  

#include "trigger.h"

#include "config.h"
#include "utils/macros.h"

// choosing compare counter to make one overflow the length of ~10 ms, PULSE_LENGTH = F_CPU / PRESCALER / COMPARE_COUNTER
#define COMPARE_COUNTER_100TH 195
#define COMPARE_COUNTER_1000TH 20

// set prescaler of clock/1024
#define PRESCALER _BV(CS02) | _BV(CS00)

volatile int OneShotTrigger::_static_timer_overflows = 1;

OneShotTrigger::OneShotTrigger(unsigned int length) {
    configure_output(TRIGGER_PIN);

    //count up to this val
    if (length > 20) {
        OCR0A = COMPARE_COUNTER_100TH;
        OneShotTrigger::_static_timer_overflows = length / 10;
    } else {
        OCR0A = COMPARE_COUNTER_1000TH;
        OneShotTrigger::_static_timer_overflows = length;
    }

    // ctc mode
    TCCR0A = _BV(WGM01);

    // // enable timer interrupt
    TIMSK0 = (1 << OCIE0A);
}

void OneShotTrigger::fire() {
    // reset timer
    TCNT0 = 0x0;

    // start 
    TCCR0B = PRESCALER;

    // start trigger pulse
    set_pin_high(TRIGGER_PIN);
}

// TODO: use free running timer and two comparators
ISR (TIMER0_COMPA_vect) 
{
    static unsigned char overflows = 0;
    overflows = (overflows + 1) % OneShotTrigger::_static_timer_overflows;

    if (overflows == 0) {
        // stop trigger pulse
        set_pin_low(TRIGGER_PIN);
        // stop timer
        TCCR0B = 0;
    }
}