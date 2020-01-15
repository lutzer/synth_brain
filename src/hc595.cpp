/*
 * @Author: Lutz Reiter - http://lu-re.de 
 * @Date: 2020-01-13 14:45:27 
 * @Last Modified by: Lutz Reiter - http://lu-re.de
 * @Last Modified time: 2020-01-13 14:53:43
 */

#include <avr/io.h>
#include <util/delay.h>

#include "hc595.h"

#include "config.h"
#include "utils/macros.h"

void hc595_init() {
    configure_output(LCD_SHIFT_CLK_PIN);
    configure_output(LCD_SHIFT_DATA_PIN);
    configure_output(LCD_SHIFT_LATCH_PIN);
    
    set_pin_low(LCD_SHIFT_DATA_PIN);
    set_pin_low(LCD_SHIFT_CLK_PIN);
    set_pin_low(LCD_SHIFT_LATCH_PIN);
}

void hc595_write(char data) {

    for(uint8_t i=0; i<8; i++) {
        if (data & (1 << i)) {
            set_pin_high(LCD_SHIFT_DATA_PIN);
        } else {
            set_pin_low(LCD_SHIFT_DATA_PIN);
        }
        // pulse clock
        set_pin_high(LCD_SHIFT_CLK_PIN);
        _delay_loop_1(1);
        set_pin_low(LCD_SHIFT_CLK_PIN);
    }
    // pulse latch to write data
    set_pin_high(LCD_SHIFT_LATCH_PIN);
    _delay_loop_1(1);
    set_pin_low(LCD_SHIFT_LATCH_PIN);
}