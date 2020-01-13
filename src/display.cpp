/*
 * @Author: Lutz Reiter - http://lu-re.de 
 * @Date: 2020-01-08 01:09:55 
 * @Last Modified by: Lutz Reiter - http://lu-re.de
 * @Last Modified time: 2020-01-08 01:11:00
 */

#include <avr/io.h>

#include "display.h"

#include "config.h"
#include "utils/macros.h"

#include "hc595.h"

Display::Display() {
    configure_output(LCD_DIGIT1_PIN);
    configure_output(LCD_DIGIT2_PIN);

    set_pin_low(LCD_DIGIT1_PIN);
    set_pin_low(LCD_DIGIT2_PIN);

    hc595_init();
}

void Display::show(int data) {
    this->data[0] = data;
    this->data[1] = data << 8;
}

void Display::clear() {
    for (int i = 0; i < NUMBER_OF_DIGITS; i++) {
        this->data[i] = 0;
    }
}

void Display::update() {
    static uchar currentDigit = 0;

    set_pin_low(LCD_DIGIT2_PIN);
    hc595_write(this->data[0]);
    set_pin_high(LCD_DIGIT1_PIN);

    // if (currentDigit == 0) {
    //     set_pin_low(LCD_DIGIT2_PIN);
    //     hc595_write(this->data[currentDigit]);
    //     set_pin_high(LCD_DIGIT1_PIN);
    // } else {
    //     set_pin_low(LCD_DIGIT1_PIN);
    //     hc595_write(this->data[currentDigit]);
    //     set_pin_high(LCD_DIGIT2_PIN);
    // }

    currentDigit = (currentDigit + 1) % NUMBER_OF_DIGITS;
}