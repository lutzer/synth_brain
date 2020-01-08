/*
 * @Author: Lutz Reiter - http://lu-re.de 
 * @Date: 2020-01-07 19:04:36 
 * @Last Modified by: Lutz Reiter - http://lu-re.de
 * @Last Modified time: 2020-01-07 19:22:24
 */

#ifndef STATEMACHINE_H
#define STATEMACHINE_H

#include <string.h>

class Statemachine {

    protected:
        int state;
        bool stateChanged;

        virtual void triggerAction(int action);

    public:
        Statemachine(int initialState);
        int getState();
        bool changed();
};

#endif