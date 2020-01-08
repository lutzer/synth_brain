/*
 * @Author: Lutz Reiter - http://lu-re.de 
 * @Date: 2020-01-07 13:39:21 
 * @Last Modified by: Lutz Reiter - http://lu-re.de
 * @Last Modified time: 2020-01-07 17:04:49
 */

#ifndef DAC_H
#define DAC_H

#include <avr/io.h>

typedef unsigned char uchar;

/*
*   Sends Data to MPC4822 using the DAC_CS PIN
*/
class Dac {
    
    public:
        Dac();

        // Sends data to dac, range is 0 - 4095 (12 bit)
        void send(uchar channel, uint16_t data);
};

#endif