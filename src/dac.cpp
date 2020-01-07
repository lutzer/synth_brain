/*
 * @Author: Lutz Reiter - http://lu-re.de 
 * @Date: 2020-01-07 13:39:16 
 * @Last Modified by: Lutz Reiter - http://lu-re.de
 * @Last Modified time: 2020-01-07 17:30:39
 */

#include <avr/io.h>
#include <avr/interrupt.h>

#include "dac.h"

#include "config.h"

#include "utils/macros.h"
#include "utils/math.h"

uchar SPI_DATA_BUFFER[2];
uchar SPI_DATA_BUFFER_COUNTER = 0;

void spi_init_master() {

     //set spi pins as output (MOSI,SCK,SS)
    DDRB |= (1<<PB2) | (1<<PB3) | (1<<PB5);


    // Enable SPI, Master, clock rate F_CPU/2, (msb first)
	SPCR = (1<<SPE) | (1<<MSTR) | (1<<SPI2X) /* | (1<<DORD) */;
}

void spi_transmit(char data) {
	SPDR = data;
	while(!(SPSR & (1<<SPIF)));
}

#define DAC_CHANNEL_BIT 15 // 0 = channel A, 1 = channel B
#define DAC_GAIN_BIT 13 // 0 = no gain, 1 = multiplies vref * 2
#define DAC_SHDN_BIT 12 // 0 = no output, 1 = output

Dac::Dac() {
    CONFIGURE_OUTPUT(DAC_CS_PIN);

    // disable slave
    SET_PIN_HIGH(DAC_CS_PIN);

    spi_init_master();
} 

void Dac::send(uchar channel, uint16_t data) {

    // set channel
    if (channel == 1)
        data |= (1 << DAC_CHANNEL_BIT);
    else
        data &= ~(1 << DAC_CHANNEL_BIT);

    // select gain 2x
    data &= ~(1 << DAC_GAIN_BIT);

    // enable output
    data |= (1 << DAC_SHDN_BIT);

    // pull cs low
    SET_PIN_LOW(DAC_CS_PIN);

    // transmit data
    spi_transmit((data >> 8) & 0xFF); // 1st byte
    spi_transmit(data & 0xFF); // 2nd byte

    // pull cs high
    SET_PIN_HIGH(DAC_CS_PIN);

}

