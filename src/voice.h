#ifndef VOICE_H
#define VOICE_H

typedef unsigned char uchar ;
typedef unsigned int uint;

class Voice {
  
    public:
        uchar channel;
        uchar note;
        bool gate;
  
        Voice(uchar midiChannel/*,dac*/);
        void playNote(uchar note);
        void stopNote(uchar note);
        void setPitchBend(uint bend);
};

#endif