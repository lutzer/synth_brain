/*
 * @Author: Lutz Reiter - http://lu-re.de 
 * @Date: 2020-01-07 18:03:14 
 * @Last Modified by:   Lutz Reiter - http://lu-re.de 
 * @Last Modified time: 2020-01-07 18:03:14 
 */

#ifndef DEBUG_H
#define DEBUG_H

#include <stdio.h>
#include "uart.h"

template<typename... Args> void debug_print(const char *string, Args... args) {
    char output[32] = "";
    sprintf(output, string, args...); 
    uart_putstring(output);
}


#endif