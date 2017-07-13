#include <msp430.h>
#include <inttypes.h>
#include "uart.h"

void uart_puts(char *s, uint16_t len) {
	uint8_t i;

	for (i = 0; i < len; i++) {
		while (!(UCA0IFG & UCTXIFG));
		UCA0TXBUF = *(s++);
	}
}

char uart_getc(void) {
    while(!(UCA0IFG & UCRXIFG));
	UCA0IFG &= ~UCRXIFG;
	return UCA0RXBUF;
}
