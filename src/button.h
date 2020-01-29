/*
 * @Author: Lutz Reiter - http://lu-re.de 
 * @Date: 2020-01-09 16:59:10 
 * @Last Modified by:   Lutz Reiter - http://lu-re.de 
 * @Last Modified time: 2020-01-09 16:59:10 
 */

#ifndef BUTTON_H
#define BUTTON_H

#include <avr/io.h>


#define BUTTON_COUNTER_OVERFLOWS 15 // how many overflows for one tick of the 2bit counter to debounce, 1 overflow = 3.2ms

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

#endif