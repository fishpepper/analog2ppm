#ifndef UART_H__
#define UART_H__

#include "main.h"

void uart_init(void);
void uart_putc(uint8_t c);
int uart_putc_s(char c, FILE *stream);
void uart_puts(uint8_t *s);
uint8_t uart_getc(uint8_t *val);

#endif
