#ifndef UART_H
#define UART_H

void uart_putstring(const char *s);
void uart_putchar(char c);
char uart_getchar();
void uart_init(void);

#endif