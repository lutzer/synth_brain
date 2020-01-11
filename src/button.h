/*
 * @Author: Lutz Reiter - http://lu-re.de 
 * @Date: 2020-01-09 16:59:10 
 * @Last Modified by:   Lutz Reiter - http://lu-re.de 
 * @Last Modified time: 2020-01-09 16:59:10 
 */

#ifndef BUTTON_H
#define BUTTON_H

#include <avr/io.h>

typedef unsigned char uchar;

typedef void (*ButtonsChangeHandler)(uchar changes, uchar pushed);
/*
* Setup, reads and debounces multiple buttons on portc
*/
class Buttons {

    ButtonsChangeHandler handler;

    uchar pushed = 0;
    uchar buttonMask = 0;
    
    public:
        Buttons(ButtonsChangeHandler handler);
        void update();

        // holds 8x2 bit counters, max = 15
        static volatile uint32_t _static_counters;
};

// typedef void (*ButtonChangeHandler)(bool pushed);
// /*
// * Reads a single button
// * Usage: //button = new Button(&ENCODER_BUTTON_PORT, &ENCODER_BUTTON_DDR, &ENCODER_BUTTON_REG, ENCODER_BUTTON, &onEncoderButtonPress);
// */
// class Button {

//     uint8_t cPin;
//     volatile uint8_t *cReg;

//     ButtonChangeHandler handler;
//     bool pushed;

//     public:
//         Button(
//             volatile uint8_t *cPort, 
//             volatile uint8_t *cDDR, 
//             volatile uint8_t *cReg, 
//             uint8_t cPin,
//             ButtonChangeHandler handler);

//         void update();
// };

#endif