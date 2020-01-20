/*
 * @Author: Lutz Reiter - http://lu-re.de 
 * @Date: 2020-01-06 19:12:26 
 * @Last Modified by: Lutz Reiter - http://lu-re.de
 * @Last Modified time: 2020-01-07 19:38:55
 */

#ifndef VOICE_H
#define VOICE_H

#include "dac.h"

typedef unsigned char uchar;
typedef unsigned int uint;

typedef void (*GateChangeHandler)(bool enabled, uchar dacChannel);

class Voice {
    GateChangeHandler gateHandler;
  
    public:
        uchar channel;
        uchar note;
        int pitchbend;

        bool gate;
        bool updated;

        Dac *dac;
        uchar dacChannel;
  
        Voice(Dac *dac, uchar dacChannel, GateChangeHandler gateHandler);
        void playNote(uchar note);
        void stopNote(uchar note);
        void setPitchBend(int bend);
        void stopAll();

        void setChannel(uchar channel);

        void update();
};

#endif