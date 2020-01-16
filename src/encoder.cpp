/*
 * @Author: Lutz Reiter - http://lu-re.de 
 * @Date: 2020-01-06 19:14:10 
 * @Last Modified by:   Lutz Reiter - http://lu-re.de 
 * @Last Modified time: 2020-01-06 19:14:10 
 */

#include "encoder.h"

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include <util/atomic.h>

#include "config.h"
#include "utils/macros.h"

#define MAX_INT_DIV2 INT32_MAX/2

enum EncoderState : uchar {
    CW_START = 0x01,
    CW_STEP1 = 0x02,
    CW_STEP2 = 0x03,
    CW_FINAL = 0x10,

    CCW_START = 0x04,
    CCW_STEP1 = 0x05,
    CCW_STEP3 = 0x06,
    CCW_FINAL = 0x20,

    UNDEFINED = 0x40,
};

const uchar transitionTable[7][4] = {
    { UNDEFINED, CCW_START, CW_START, UNDEFINED },  // init state -> 1,2

    { CW_STEP1, UNDEFINED, UNDEFINED, UNDEFINED }, // cw start -> 0
    { UNDEFINED, CW_STEP2, CW_START, UNDEFINED }, // cw step1 -> 1, <- 2
    { CW_STEP1, UNDEFINED, UNDEFINED, CW_FINAL }, // cw step2 -> 3, <- 0

    { CCW_STEP1, UNDEFINED, UNDEFINED, UNDEFINED }, // ccw start -> 0
    { UNDEFINED, CCW_START, CCW_STEP3, UNDEFINED }, // ccw step1 -> 2, <- 0
    { UNDEFINED, CCW_STEP1, UNDEFINED , CCW_FINAL } // ccw step2 -> 3, <- 2
};

volatile long Encoder::_static_encoder_absolute_position = 0;

Encoder::Encoder(EncoderEventHandlerPtr handler) {

    // set PD2 and PD3 as input
    configure_input(ENCODER_PIN1);
    configure_input(ENCODER_PIN2);

    // enable pullup
    set_pin_high(ENCODER_PIN1);
    set_pin_high(ENCODER_PIN2);

    // enable interrupts on portc
    PCICR |= (1 << PCIE1);

    // enable change interrupts on pins
    PCMSK1 |= (1 << PCINT8) | (1 << PCINT9);

    // register change handler
    encoderChangeHandler = handler;

    // init encoder position
    lastRead = Encoder::_static_encoder_absolute_position;
}

void Encoder::update() {
    if (int change = this->getChange())
        encoderChangeHandler(change);
}

int Encoder::getChange() {
    int delta = 0;
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        delta = Encoder::_static_encoder_absolute_position - lastRead;
    }   
    lastRead += delta;

    // if reading is in bounds
    if (abs(delta) < MAX_INT_DIV2)
        return delta;

    // if the maximum bounds of long is reached, reset position
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        Encoder::_static_encoder_absolute_position = 0;
        lastRead = 0;
    }
    return delta > 0 ? -1 : 1;
}

int Encoder::getAbsolute() {
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        return Encoder::_static_encoder_absolute_position;
    }
    return 0;
}

ISR(PCINT1_vect) {
    static uchar state = 0;

    uchar pins = 
        (ENCODER_PIN1_REG & _BV(ENCODER_PIN1)) >> (ENCODER_PIN1 - 1) 
        | (ENCODER_PIN2_REG & _BV(ENCODER_PIN2)) >> ENCODER_PIN2;

    state = transitionTable[state & 0x0F][pins];

    if (state == 0x10)
        Encoder::_static_encoder_absolute_position++;
    if (state == 0x20)
        Encoder::_static_encoder_absolute_position--;
}