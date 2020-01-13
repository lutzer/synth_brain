#include <avr/io.h>
#include <util/delay.h>

#include "hc595.h"

#include "config.h"
#include "utils/macros.h"

#ifdef DEBUG
#include "utils/debug.h"
#endif

void hc595_init() {


    debug_print("portb: %01X\n", DDRB);
    
    configure_output(LCD_SHIFT_CLK_PIN);
    configure_output(LCD_SHIFT_DATA_PIN);
    configure_output(LCD_SHIFT_LATCH_PIN);

    debug_print("portb: %01X\n", DDRB);

    set_pin_low(LCD_SHIFT_CLK_PIN);
    set_pin_low(LCD_SHIFT_LATCH_PIN);
}

void hc595_write(char data) {
    #ifdef DEBUG
    debug_print("lcd:%01X\n", data);
    #endif


    for(uint8_t i=0; i<8; i++) {
        if (data & (1 << i)) {
            set_pin_high(LCD_SHIFT_DATA_PIN);
        } else {
            set_pin_low(LCD_SHIFT_DATA_PIN);
        }
        // pulse clock
        set_pin_high(LCD_SHIFT_CLK_PIN);
        _delay_ms(1);
        set_pin_low(LCD_SHIFT_CLK_PIN);
        _delay_ms(1);
    }
    // pulse latch to write data
    set_pin_high(LCD_SHIFT_LATCH_PIN);
    _delay_ms(1);
    set_pin_low(LCD_SHIFT_LATCH_PIN);
    _delay_ms(1);

    _delay_ms(1000);
}