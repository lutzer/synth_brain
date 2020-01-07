/*
 * @Author: Lutz Reiter - http://lu-re.de 
 * @Date: 2020-01-06 19:12:26 
 * @Last Modified by: Lutz Reiter - http://lu-re.de
 * @Last Modified time: 2020-01-07 16:57:31
 */

#ifndef VOICE_H
#define VOICE_H

#include "dac.h"

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

        Dac *dac;
        uchar dacChannel;
  
        Voice(GateChangeHandler gateHandler ,Dac *dac, uchar dacChannel);
        void playNote(uchar note);
        void stopNote(uchar note);
        void setPitchBend(uint bend);
        void stopAll();

        void setChannel(uchar channel);

        void update();
};

#endif