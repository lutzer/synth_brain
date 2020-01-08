/*
 * @Author: Lutz Reiter - http://lu-re.de 
 * @Date: 2020-01-07 19:23:17 
 * @Last Modified by: Lutz Reiter - http://lu-re.de
 * @Last Modified time: 2020-01-07 19:23:38
 */

#include "statemachine.h"

Statemachine::Statemachine(int initalState) {
    this->state = initalState;
    this->stateChanged = true;
}

int Statemachine::getState() {
    this->stateChanged = false;
    return this->state;
}

bool Statemachine::changed() {
    return stateChanged;
}

