/*
 * @Author: Lutz Reiter - http://lu-re.de 
 * @Date: 2020-01-06 19:12:26 
 * @Last Modified by: Lutz Reiter - http://lu-re.de
 * @Last Modified time: 2020-01-07 14:33:03
 */

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

        void setChannel(uchar channel);

        void update();
};

#endif