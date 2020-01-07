/*
 * @Author: Lutz Reiter - http://lu-re.de 
 * @Date: 2020-01-07 13:39:16 
 * @Last Modified by: Lutz Reiter - http://lu-re.de
 * @Last Modified time: 2020-01-07 13:48:48
 */

#include <avr/io.h>

#include "dac.h"

#include "config.h"
#include "utils/macros.h"

Dac::Dac() {
    CONFIGURE_OUTPUT(DAC_CS_PIN);

    // disable slave
    SET_PIN_HIGH(DAC_CS_PIN);

    // configure spi
    


}  