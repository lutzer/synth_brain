/*
 * @Author: Lutz Reiter - http://lu-re.de 
 * @Date: 2020-01-08 01:09:55 
 * @Last Modified by: Lutz Reiter - http://lu-re.de
 * @Last Modified time: 2020-01-21 12:35:51
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

#define DISPLAY_TIMER_OVERFLOWS 2 // 1 overlfow = 3,2ms

volatile bool Display::_needsRefresh = false;

// array containing 0, 1, 2, 3, 4, 5, 6, 7, 8, 9
const uchar lcdNumTable[] = {
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

const uchar lcdCharTable[] = {
    0b11101100, // 0=M
    0b10011100, // 1=C
    0b00011100, // 2=L
    0b01101110, // 3=H
    0b10110110, // 4=S
    0b11101110, // 5=A
    0b10011100, // 6=[
    0b11110000, // 7=]
};

void DisplayRefreshFunc() {
    static uint8_t overflows = 0;
    overflows++;

    if (overflows % DISPLAY_TIMER_OVERFLOWS == 0) {
        Display::_needsRefresh = true;
        overflows = 0;
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

    // only refresh when timer requests an update
    Timer2::addCallback(&DisplayRefreshFunc);

    setDots(0);

}

void Display::showDot(uchar dot, bool show) {
    if (show)
        this->dots |= (1 << dot);
    else
        this->dots &= ~(1 << dot);
}

void Display::setDots(const uchar dots) {
    this->dots = dots;
}

void Display::print(uchar number) {
    #ifdef DEBUG
    debug_print("lcd:%i\n", number);
    #endif

    for (uint8_t i=0;i < NUMBER_OF_DIGITS; i++) {
        this->data[NUMBER_OF_DIGITS -1 -i] = lcdNumTable[number % 10];
        number /= 10;
    }
}

void Display::print(const char *str) {
    #ifdef DEBUG
    debug_print("lcd:%s\n", str);
    #endif

    for (uint8_t i=0;i < NUMBER_OF_DIGITS; i++) {
        if (str[i] >= 48 && str[i] <58) //if its a number
            this->data[i] = lcdNumTable[str[i] - 48];
        // else if (str[i] == 77) // M
        //     this->data[i] = lcdCharTable[0];
        else if (str[i] == 67) // C
            this->data[i] = lcdCharTable[1];
        // else if (str[i] == 76) // L
        //     this->data[i] = lcdCharTable[2];
        // else if (str[i] == 72) // H
        //     this->data[i] = lcdCharTable[3];
        // else if (str[i] == 83) // S
        //     this->data[i] = lcdCharTable[4];
        else if (str[i] == 65) // A
            this->data[i] = lcdCharTable[5];
        else if (str[i] == 91) // [
            this->data[i] = lcdCharTable[6];
        else if (str[i] == 93) // ]
            this->data[i] = lcdCharTable[7];
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

    if (!Display::_needsRefresh)
        return;
    
    if (currentDigit == 0) {
        #ifndef DEBUG
        set_pin_low(LCD_DIGIT2_PIN);
        #endif
        hc595_write(this->data[currentDigit] | (dots & 0x1) );
        set_pin_high(LCD_DIGIT1_PIN);
    } else {
        set_pin_low(LCD_DIGIT1_PIN);
        hc595_write(this->data[currentDigit] | ((dots & 0x2) >> 1) );
        #ifndef DEBUG
        set_pin_high(LCD_DIGIT2_PIN);
        #endif
    }

    Display::_needsRefresh = false;

    currentDigit = (currentDigit + 1) % NUMBER_OF_DIGITS;

}