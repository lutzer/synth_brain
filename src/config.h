#ifndef PARAMS_H
#define PARAMS_H

// enable debugging by build flag or here
// #define DEBUG

// define pins
#define GATE_PIN PD7
#define GATE_PIN_PORT PORTD
#define GATE_PIN_DDR DDRD

#define TRIGGER_PIN PD6
#define TRIGGER_PIN_PORT PORTD
#define TRIGGER_PIN_DDR DDRD

#define DAC_CS_PIN PB2
#define DAC_CS_PIN_PORT PORTB
#define DAC_CS_PIN_DDR DDRB

#define ENCODER_PIN1 PC1
#define ENCODER_PIN1_PORT PORTC
#define ENCODER_PIN1_DDR DDRC
#define ENCODER_PIN1_REG PINC

#define ENCODER_PIN2 PC0
#define ENCODER_PIN2_PORT PORTC
#define ENCODER_PIN2_DDR DDRC
#define ENCODER_PIN2_REG PINC

// shift register for lcd
#define LCD_SHIFT_LATCH_PIN PB0
#define LCD_SHIFT_LATCH_PIN_PORT PORTB
#define LCD_SHIFT_LATCH_PIN_DDR DDRB

#define LCD_SHIFT_DATA_PIN PB1
#define LCD_SHIFT_DATA_PIN_PORT PORTB
#define LCD_SHIFT_DATA_PIN_DDR DDRB

// #define LCD_SHIFT_CLK_PIN PB4
// #define LCD_SHIFT_CLK_PIN_PORT PORTB
// #define LCD_SHIFT_CLK_PIN_DDR DDRB
#define LCD_SHIFT_CLK_PIN PD5
#define LCD_SHIFT_CLK_PIN_PORT PORTD
#define LCD_SHIFT_CLK_PIN_DDR DDRD

// digit selectors for lcd
#define LCD_DIGIT1_PIN PD4
#define LCD_DIGIT1_PIN_PORT PORTD
#define LCD_DIGIT1_PIN_DDR DDRD

#define LCD_DIGIT2_PIN PD1
#define LCD_DIGIT2_PIN_PORT PORTD
#define LCD_DIGIT2_PIN_DDR DDRD

// input pins
#define BUTTONS_REG PINC

#define ENCODER_BUTTON PC2
#define ENCODER_BUTTON_PORT PORTC
#define ENCODER_BUTTON_DDR DDRC

#define MODE_BUTTON PC3
#define MODE_BUTTON_PORT PORTC
#define MODE_BUTTON_DDR DDRC

//led pins
#define MODE_LED1 PC4
#define MODE_LED1_PORT PORTC
#define MODE_LED1_DDR DDRC

#define MODE_LED2 PC5
#define MODE_LED2_PORT PORTC
#define MODE_LED2_DDR DDRC

// set trigger pulse length
#define TRIGGER_PULSE_LENGTH 10 // in ms

// set baudrate for uart
#define BAUD 31250
#define UART_BUFFER_SIZE 128

// full pitch bend +- semitones
#define PITCH_BEND_SEMITONES 2

#endif