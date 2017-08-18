#include <msp430.h>
#include <inttypes.h>

volatile uint8_t *tx_data;

void i2c_write(uint8_t slave_addr, uint8_t *data, uint8_t len) {
    tx_data = data;
    
    /* configure I2C peripheral for transfer */
    UCB0CTLW0 |= UCSWRST;
    UCB0TBCNT = len;
    UCB0I2CSA = slave_addr;
    UCB0CTLW0  &= ~UCSWRST;
    UCB0IE    |= UCTXIE0 + UCNACKIE + UCSTPIE;

    /* transmit start condition and sleep CPU while transfer occurs */
    UCB0CTLW0 |= UCTR + UCTXSTT;
    __bis_SR_register(CPUOFF);

    /* disable interrupts again and reset I2C peripheral */
    UCB0IE    &= ~(UCTXIE0 + UCNACKIE + UCSTPIE);
    UCB0CTLW0 |= UCSWRST;
}

#pragma vector = USCI_B0_VECTOR 
__interrupt void USCIB0_ISR(void)
{
	__asm__ __volatile__("add   %[src]       ,r0       ":: [src] "m" (UCB0IV));
	__asm__ __volatile__("reti                         "::); // No Interrupt
	__asm__ __volatile__("reti                         "::); // ALIFG
	__asm__ __volatile__("jmp   uscib0_nak_handler     "::); // NACKIFG
	__asm__ __volatile__("reti                         "::); // STTIFG
	__asm__ __volatile__("jmp   uscib0_stp_handler     "::); // STPIFG
	__asm__ __volatile__("reti                         "::); // RXIFG3
	__asm__ __volatile__("reti                         "::); // TXIFG3
	__asm__ __volatile__("reti                         "::); // RXIFG2
	__asm__ __volatile__("reti                         "::); // TXIFG2
	__asm__ __volatile__("reti                         "::); // RXIFG1
	__asm__ __volatile__("reti                         "::); // TXIFG1
	__asm__ __volatile__("reti                         "::); // RXIFG0
	__asm__ __volatile__("jmp   uscib0_txifg0_handler  "::); // TXIFG0
	__asm__ __volatile__("reti                         "::); // BCNTIFG
	__asm__ __volatile__("reti                         "::); // clock low timeout
	__asm__ __volatile__("reti                         "::); // 9th bit
}

__interrupt void uscib0_nak_handler(void)
{
    /* return to program flow */
    __bic_SR_register_on_exit(CPUOFF);
}

__interrupt void uscib0_stp_handler(void)
{
    /* return to program flow */
    __bic_SR_register_on_exit(CPUOFF);
}

__interrupt void uscib0_txifg0_handler(void)
{
    /* transmit the next data byte */
    UCB0TXBUF = *(tx_data++);
}
