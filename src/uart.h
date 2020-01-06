#ifndef UART_H
#define UART_H

#ifndef BAUD
#define BAUD 9600
#endif

#ifndef UART_BUFFER_SIZE
#define UART_BUFFER_SIZE 128
#endif

void uart_init();
void uart_putstring(const char *s);
void uart_putchar(char c);
char uart_getchar();
bool uart_data_available(void);

#endif