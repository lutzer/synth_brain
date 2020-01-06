/*
 * @Author: Lutz Reiter - http://lu-re.de 
 * @Date: 2020-01-06 19:14:02 
 * @Last Modified by: Lutz Reiter - http://lu-re.de
 * @Last Modified time: 2020-01-06 22:50:23
 */

#ifndef ENCODER_H
#define ENCODER_H

#define EXTERNAL_NUM_INTERRUPTS 2

typedef unsigned char uchar;
typedef void (*EncoderEventHandlerPtr)(int change);

/*
* Reads and debounces shaft encoder connected to the two interupt pins PD2, PD3
*/
class Encoder {

    volatile long absolutePosition;
    long lastRead;

    EncoderEventHandlerPtr encoderChangeHandler;

    public:
        Encoder(EncoderEventHandlerPtr handler);

        void update();

        int getChange();
        int getAbsolute();

        // not for external use
        void _processInterrupt();

};

#endif
