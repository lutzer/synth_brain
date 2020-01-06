#ifndef ENCODER_H
#define ENCODER_H

typedef void (*EncoderEventHandlerPtr)(int change);

/*
* Encoder connected to two interupt pins PD2, PD3
*/
class Encoder {

    long absolutePosition;
    long lastRead;

    EncoderEventHandlerPtr encoderChangeHandler;

    public:
        Encoder(EncoderEventHandlerPtr handler);

        void update();

        int getChange();
        int getAbsolute();

        // not for external use
        void _process(char pinStates);

};

#endif
