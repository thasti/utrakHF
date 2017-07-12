#ifndef UART_H_
#define UART_H_

enum uart_config_state {UART_DISABLE, UART_ENABLE};

void uart_config(uart_config_state state);
void uart_puts(char *s, uint16_t len);
char uart_getc(void);

#endif
