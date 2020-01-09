#ifndef BUTTON_H
#define BUTTON_H

#include <avr/io.h>

#define NUMBER_OF_COUNTERS 8
#define DEBOUNCE_CYCLES 60000

typedef unsigned char uchar;

typedef void (*ButtonsChangeHandler)(uchar id, bool pushed);
/*
* Setup, reads and debounces multiple buttons on portc
*/
class Buttons {

    ButtonsChangeHandler handler;

    uchar states;
    unsigned int counters[NUMBER_OF_COUNTERS];
    uchar pushed;
    
    public:
        Buttons(ButtonsChangeHandler handler);
        void update();
};

typedef void (*ButtonChangeHandler)(bool pushed);
/*
* Reads a single button
* Usage: //button = new Button(&ENCODER_BUTTON_PORT, &ENCODER_BUTTON_DDR, &ENCODER_BUTTON_REG, ENCODER_BUTTON, &onEncoderButtonPress);
*/
class Button {

    uint8_t cPin;
    volatile uint8_t *cReg;

    ButtonChangeHandler handler;
    bool pushed;

    public:
        Button(
            volatile uint8_t *cPort, 
            volatile uint8_t *cDDR, 
            volatile uint8_t *cReg, 
            uint8_t cPin,
            ButtonChangeHandler handler);

        void update();
};

#endif