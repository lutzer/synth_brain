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