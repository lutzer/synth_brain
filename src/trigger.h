/*
 * @Author: Lutz Reiter - http://lu-re.de 
 * @Date: 2020-01-06 19:11:16 
 * @Last Modified by: Lutz Reiter - http://lu-re.de
 * @Last Modified time: 2020-01-06 22:49:21
 */

#ifndef PULSE_H
#define PULSE_H

/*
* Uses Timer0 to fire one trigger pulse of the specified length on TRIGGER_PIN
*/
class OneShotTrigger {

    public:
        OneShotTrigger(unsigned int length /* in ms */);
        void fire();
    
};

#endif