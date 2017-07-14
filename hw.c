#include <msp430.h>
#include <inttypes.h>
#include "hw.h"
#include "main.h"

extern volatile uint16_t isr_flags;

void hw_init(void) {
    /* setup watchdog */
	WDTCTL = WDTPW + WDTCNTCL + WDTIS0;

    /* setup DCO */
	CSCTL0_H = 0xA5;
	CSCTL1 = DCOFSEL_0;
	CSCTL2 = SELA__DCOCLK + SELS__XT1CLK + SELM__DCOCLK;
	CSCTL3 = DIVA__32 + DIVS__32 + DIVM__4;
	CSCTL4 = XT1OFF + XT2OFF;

    /* setup voltage enable pins */
    PDIR_VEN |= BIT_VEN_GPS + BIT_VEN_RF;
    POUT_VEN |= BIT_VEN_GPS + BIT_VEN_RF;

    /* setup unused pins */
    PDIR_UNUSED1 |= BIT_UNUSED1_A + BIT_UNUSED1_B;
    POUT_UNUSED1 &= ~(BIT_UNUSED1_A + BIT_UNUSED1_B);
    PDIR_UNUSED2 |= BIT_UNUSED2_A + BIT_UNUSED2_B + BIT_UNUSED2_C +
        BIT_UNUSED2_D + BIT_UNUSED2_E;
    POUT_UNUSED2 |= ~(BIT_UNUSED2_A + BIT_UNUSED2_B + BIT_UNUSED2_C +
        BIT_UNUSED2_D + BIT_UNUSED2_E);

    /* setup ADC pins */
    PSEL0_ADC |= BIT_ADC_VSOL + BIT_ADC_VBAT; 

    /* setup GPS and RF pins */
    hw_gps_config(MODULE_DISABLE);
    hw_rf_config(MODULE_DISABLE);

    /* setup heartbeat timer
     *   * clock source: ACLK = FDCO/32
     *   * prescaler: 8
     *   * clock speed: 5.37 MHz / 32 / 8 = ~21 kHz
     *   * heartbeat at 1 s intervals
     *   * continuous mode
     */
	TA0CCR0 = 0;
	TA0CTL = TASSEL_1 + MC_2 + ID_3 + TAIE + TACLR;
	TA0CCTL0 |= CCIE;

    /* setup WSPR baud rate timer
     *   * clock source. SMCLK = FXT1/32
     *   * prescaler 8
     *   * clock speed: 16.3676 MHz / 32 / 8 = ~64kHz
     *   * WSPR symbol time = 8192/12000 s
     *   * continuous mode
     */
    TB0CCR0 = 0;
    TB0CTL = TBSSEL_2 + MC_2 + ID_3;
    TB0CCTL0 = CCIE;
	
    /* enable interrupts globally */
    __bis_SR_register(GIE);
}

void hw_watchdog_feed(void) {
	WDTCTL = WDTPW + WDTCNTCL + WDTIS0;
}

void hw_gps_config(hw_module_state state) {
    switch (state) {
    case MODULE_DISABLE:
        /* disable GPS voltage */
        POUT_VEN |= BIT_VEN_GPS;

        /* disable UART */
        UCA0CTL1 = UCSWRST;

        /* set pins to GPIO */
        POUT_UART &= ~(BIT_UART_TXD + BIT_UART_RXD);
        PDIR_UART |= BIT_UART_TXD + BIT_UART_RXD;
        PSEL0_UART &= ~(BIT_UART_TXD + BIT_UART_RXD);
        PSEL1_UART &= ~(BIT_UART_TXD + BIT_UART_RXD);
        break;

    case MODULE_ENABLE:
        /* set to USCI alternate function */
        PSEL0_UART &= ~(BIT_UART_TXD + BIT_UART_RXD);
        PSEL1_UART |= BIT_UART_TXD + BIT_UART_RXD;
        /* configure and enable UART
         *   * ACLK = FDCO/32 is used for baud rate generation
         *   * FDCO = 5.37 MHz
         *   * N = 17.48, therefore OS16 enabled
         *   * BR = INT(N) / 16 = 1
         *   * BRF = INT(((N/16) - INT(N/16)) * 16) = 1
         *   * BRS = table(N - INT(N)) = 0.48 -> 0x55
         */
        UCA0CTL1 = UCSWRST;
        UCA0CTL1 |= UCSSEL_1;
        UCA0BR0 = 1;
        UCA0BR1 = 0;
        UCA0MCTLW = (0x55<<8) + (1<<4) + UCOS16;
        UCA0CTL1 &= ~UCSWRST;
        
        /* enable GPS voltage */
        POUT_VEN &= ~BIT_VEN_GPS;
        break;
    }

}

void hw_rf_config(hw_module_state state) {
    switch (state) {
    case MODULE_DISABLE:
        /* disable XT1 oscillator */
        SFRIE1 &= ~OFIE;
        CSCTL0_H = 0xA5;
        CSCTL4 |= XT1BYPASS + XTS;
        CSCTL4 |= XT1OFF;

        /* disable RF voltage */
        POUT_VEN |= BIT_VEN_RF;

        /* disable I2C */
        UCB0CTLW0 |= UCSWRST;

        /* set pins to GPIO */
        POUT_I2C &= ~(BIT_I2C_SCL + BIT_I2C_SDA);
        PDIR_I2C |= BIT_I2C_SDA + BIT_I2C_SCL;
        PSEL0_I2C &= ~(BIT_I2C_SCL + BIT_I2C_SDA);
        PSEL1_I2C &= ~(BIT_I2C_SCL + BIT_I2C_SDA);

        POUT_CLKIN &= ~BIT_CLKIN;
        PDIR_CLKIN |= BIT_CLKIN;
        PSEL0_CLKIN &= ~BIT_CLKIN;
        PSEL1_CLKIN &= ~BIT_CLKIN;

        POUT_1PPS &= ~BIT_1PPS;
        PDIR_1PPS |= ~BIT_1PPS;
        break;

    case MODULE_ENABLE:
        /* set to I2C alternate function */
        PSEL0_I2C &= ~(BIT_I2C_SCL + BIT_I2C_SDA);
        PSEL1_I2C |= BIT_I2C_SCL + BIT_I2C_SDA;

        /* set oscillator input to XT1 */
        PSEL0_CLKIN |= BIT_CLKIN;
        PSEL1_CLKIN |= BIT_CLKIN;

        /* set 1PPS pin to input */
        PDIR_1PPS &= ~BIT_1PPS;

        /* configure I2C
         *   * clock source = ACLK = FDCO/32
         *   * FDCO = 5.37 MHz
         *   * max baud rate = ACLK/4 = ~42 kHz
         *   * don't release reset
         */
        UCB0CTLW0 |= UCSWRST;
        UCB0CTLW0 |= UCMODE_3  + UCMST + UCSYNC + UCSSEL_0;
        UCB0CTLW0 &= ~UCSSEL_1;
        UCB0CTLW1 |= UCASTP_2;
        UCB0BRW    = 0x0004;
        UCB0IE    |= UCRXIE+UCNACKIE+UCBCNTIE; 	
        
        /* enable RF voltage */
        POUT_VEN |= BIT_VEN_RF;

        /* enable XT1 oscillator */
        CSCTL0_H = 0xA5;
        CSCTL4 |= XT1BYPASS + XTS;
        CSCTL4 &= ~XT1OFF;
        do {
            CSCTL5 &= ~XT1OFFG;
            SFRIFG1 &= ~OFIFG;
        } while (SFRIFG1 & OFIFG);
        SFRIE1 |= OFIE;
        break;
    }
}

void hw_enter_low_power_mode(void) {
    __bis_SR_register(LPM3_bits);
}

void hw_reset_wspr_baud_timer(void) {
    TB0CCR0 = 0;
    TB0R = 65535;
}

#pragma vector=UNMI_VECTOR
__interrupt void UNMI_ISR(void)
{
    /* just hang here until the fault is gone */
	do {
		CSCTL5 &= ~XT1OFFG;
		SFRIFG1 &= ~OFIFG;
		__delay_cycles(25000);
	} while (SFRIFG1 & OFIFG);
}

#pragma vector = TIMER0_A0_VECTOR
__interrupt void TIMER0_A0_ISR (void)
{
	TA0CCR0 += (20976 - 1);
	isr_flags |= ISR_FLAG_HEARTBEAT;
    if (isr_flags & ISR_FLAG_WAKE_CPU) {
        __bic_SR_register_on_exit(LPM3_bits);
    }
}

#pragma vector = TIMER0_B0_VECTOR
__interrupt void Timer_B (void)
{
    TB0CCR0 += (43647 - 1);
    isr_flags |= ISR_FLAG_WSPR_BAUD;
}
