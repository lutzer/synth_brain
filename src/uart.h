/*
 * @Author: Lutz Reiter - http://lu-re.de 
 * @Date: 2020-01-06 19:12:07 
 * @Last Modified by:   Lutz Reiter - http://lu-re.de 
 * @Last Modified time: 2020-01-06 19:12:07 
 */

#ifndef UART_H
#define UART_H

void uart_init();
char uart_getchar();
bool uart_data_available(void);

#ifdef DEBUG
void uart_putstring(const char *s);
void uart_putchar(char c);
void uart_putfloat(float f);
#endif

#endif