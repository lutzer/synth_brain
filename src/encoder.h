#ifndef ENCODER_H
#define ENCODER_H

#define EXTERNAL_NUM_INTERRUPTS 2

typedef unsigned char uchar;
typedef void (*EncoderEventHandlerPtr)(int change);

/*
* Encoder connected to two interupt pins PD2, PD3
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
