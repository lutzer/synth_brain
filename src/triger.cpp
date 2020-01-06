/*
 * @Author: Lutz Reiter - http://lu-re.de 
 * @Date: 2020-01-06 22:30:02 
 * @Last Modified by: Lutz Reiter - http://lu-re.de
 * @Last Modified time: 2020-01-06 23:19:13
 */

#include<avr/io.h>
#include <avr/interrupt.h>  

#include "trigger.h"

#include "params.h"
#include "utils/macros.h"

// choosing compare counter to make one overflow the length of ~10 ms, PULSE_LENGTH = F_CPU / PRESCALER / COMPARE_COUNTER
#define COMPARE_COUNTER_100TH 195
#define COMPARE_COUNTER_1000TH 20

// set prescaler of clock/1024
#define PRESCALER _BV(CS02) | _BV(CS00)

int TIMER_OVERFLOWS = 1;

OneShotTrigger::OneShotTrigger(unsigned int length) {

    //count up to this val
    if (length > 20) {
        OCR0A = COMPARE_COUNTER_100TH;
        TIMER_OVERFLOWS = length / 10;
    } else {
        OCR0A = COMPARE_COUNTER_1000TH;
        TIMER_OVERFLOWS = length;
    }

    // ctc mode
    TCCR0A = _BV(WGM01);

    // // enable timer interrupt
    TIMSK0 = (1 << OCIE0A);
}

void OneShotTrigger::fire() {
    // reset timer
    TCNT0 = 0;
    // start 
    TCCR0B = PRESCALER;

    // start trigger pulse
    SET_PIN_HIGH(TRIGGER_PIN);
}

ISR (TIMER0_COMPA_vect) 
{
    static unsigned char overflows = 0;
    overflows = (overflows + 1) % TIMER_OVERFLOWS;

    if (overflows == 0) {
        // stop trigger pulse
        SET_PIN_LOW(TRIGGER_PIN);
        // stop timer
        TCCR0B = 0;
    }
}