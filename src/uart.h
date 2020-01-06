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