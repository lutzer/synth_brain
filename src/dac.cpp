/*
 * @Author: Lutz Reiter - http://lu-re.de 
 * @Date: 2020-01-07 13:39:16 
 * @Last Modified by: Lutz Reiter - http://lu-re.de
 * @Last Modified time: 2020-01-07 18:07:49
 */

#include <avr/io.h>

#include "dac.h"

#include "config.h"

#include "spi.h"
#include "utils/macros.h"
#include "utils/math.h"

#define DAC_CHANNEL_BIT 15 // 0 = channel A, 1 = channel B
#define DAC_GAIN_BIT 13 // 0 = no gain, 1 = multiplies vref * 2
#define DAC_SHDN_BIT 12 // 0 = no output, 1 = output

Dac::Dac() {
    configure_input(DAC_CS_PIN);

    // disable slave
    set_pin_high(DAC_CS_PIN);

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
    set_pin_low(DAC_CS_PIN);

    // transmit data
    spi_transmit(data >> 8); // 1st byte
    spi_transmit(data); // 2nd byte

    // pull cs high
    set_pin_high(DAC_CS_PIN);

}

