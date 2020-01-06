#ifndef VOICE_H
#define VOICE_H

typedef unsigned char uchar;
typedef unsigned int uint;

typedef void (*GateChangeHandler)(bool enabled);

class Voice {

    GateChangeHandler gateHandler;
  
    public:
        uchar channel;
        uchar note;


        bool gate;
        bool updated;
  
        Voice(GateChangeHandler gateHandler/*,dac*/);
        void playNote(uchar note);
        void stopNote(uchar note);
        void setPitchBend(uint bend);
        void stopAll();

        void update();
};

#endif