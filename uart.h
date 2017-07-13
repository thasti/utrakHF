#ifndef UART_H_
#define UART_H_

void uart_puts(char *s, uint16_t len);
void uart_putc(char s);
char uart_getc(void);
void uart_flush_tx(void);
void uart_flush_rx(void);

#endif
