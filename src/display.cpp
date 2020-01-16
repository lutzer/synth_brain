/*
 * @Author: Lutz Reiter - http://lu-re.de 
 * @Date: 2020-01-08 01:09:55 
 * @Last Modified by: Lutz Reiter - http://lu-re.de
 * @Last Modified time: 2020-01-08 01:11:00
 */

#include <avr/io.h>
#include <util/delay.h>

#include "display.h"

#include "config.h"
#include "utils/macros.h"
#include "utils/timers.h"

#include "hc595.h"

#ifdef DEBUG
#include "utils/debug.h"
#endif

#define DISPLAY_TIMER_OVERFLOWS 1 // 1 overlfow = 3,2ms

volatile bool Display::needs_refresh = false;

// array containing 0, 1, 2, 3, 4, 5, 6, 7, 8, 9
const uchar lcdCharTable[] = {
    0b11111100, // 0
    0b01100000, // 1
    0b11011010, // 2
    0b11110010, // 3
    0b01100110, // 4
    0b10110110, // 5
    0b10111110, // 6
    0b11100000, // 7
    0b11111110, // 8
    0b11110110, // 9
};

void DisplayTimerFunc() {
    unsigned static int overflows = 0;
    overflows++;

    if (overflows % DISPLAY_TIMER_OVERFLOWS == 0) {
        Display::needs_refresh = true;
    }
}

Display::Display() {
    configure_output(LCD_DIGIT1_PIN);
    set_pin_low(LCD_DIGIT1_PIN);

    // pin is used for uart in debug mode
    #ifndef DEBUG
    configure_output(LCD_DIGIT2_PIN);
    set_pin_low(LCD_DIGIT2_PIN);
    #endif

    hc595_init();

    // only refresh
    Timer2::addCallback(&DisplayTimerFunc);

}

void Display::print(uchar number) {
    #ifdef DEBUG
    debug_print("lcd:%i\n", number);
    #endif

    for (int i=0;i < NUMBER_OF_DIGITS; i++) {
        this->data[NUMBER_OF_DIGITS -1 -i] = lcdCharTable[number % 10];
        number /= 10;
    }
}

void Display::print(const char *str) {
    #ifdef DEBUG
    debug_print("lcd:%s\n", str);
    #endif

    for (int i=0;i < NUMBER_OF_DIGITS; i++) {
        if (str[i] >= 48 && str[i] <58)
            this->data[i] = lcdCharTable[str[i] - 48];
        else
            this->data[i] = 0;
    }
}

void Display::setData(int data) {
    #ifdef DEBUG
    debug_print("lcd:%02X\n", data);
    #endif

    this->data[0] = data;
    this->data[1] = data >> 8;
}

void Display::clear() {
    for (int i = 0; i < NUMBER_OF_DIGITS; i++) {
        this->data[i] = 0;
    }
}

void Display::update() {
    static uchar currentDigit = 0;

    if (!Display::needs_refresh)
        return;
    
    if (currentDigit == 0) {
        #ifndef DEBUG
        set_pin_low(LCD_DIGIT2_PIN);
        #endif
        hc595_write(this->data[currentDigit]);
        set_pin_high(LCD_DIGIT1_PIN);
    } else {
        set_pin_low(LCD_DIGIT1_PIN);
        hc595_write(this->data[currentDigit]);
        #ifndef DEBUG
        set_pin_high(LCD_DIGIT2_PIN);
        #endif
    }

    Display::needs_refresh = false;

    currentDigit = (currentDigit + 1) % NUMBER_OF_DIGITS;

}