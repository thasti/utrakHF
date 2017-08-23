#include <msp430.h>
#include <inttypes.h>
#include "adc.h"
#include "hw.h"

volatile uint16_t adc_result;

uint16_t adc_get_voltage(adc_voltage_channel channel) {
	uint16_t i;
	uint16_t voltage;
	
    /* configure ADC
     *   * conversion clock ACLK = FDCO/32
     *   * FDCO/32 = 5.37 MHz/2 -> 370 ns period
     *   * worst case source impedance = 1 MOhm
     *   * input capacitance ~10 pF
     *   * ~80 us required sampling time
     *   * using 256 cycles for S&H is more than enough
     *   * max sample rate is ~13 kHz, so low speed mode can be used
     */
    ADC10CTL0 = ADC10SHT_8 + ADC10ON;
	ADC10CTL1 = ADC10SHP + ADC10SSEL0;
	ADC10CTL2 = ADC10RES + ADC10SR;
    switch (channel) {
    case CHANNEL_VSOL:
	    ADC10MCTL0 = ADC_CHAN_VSOL;
        break;
    case CHANNEL_VBAT:
	    ADC10MCTL0 = ADC_CHAN_VBAT;
        break;
    }
	ADC10IE = ADC10IE0;

    /* take ADC reading */
	voltage = 0;
	for (i = 0; i < 10; i++) {
		ADC10CTL0 |= ADC10ENC + ADC10SC;
		__bis_SR_register(CPUOFF + GIE);
		voltage += adc_result * 32 / 10;
	}
	voltage /= 10;

    /* post process readings */
    switch (channel) {
    case CHANNEL_VBAT:
        voltage = voltage * 2;
        break;
    default:
        break;
    }
	
    /* disable ADC */
	ADC10IE &= ~ADC10IE0;
    ADC10CTL0 &= ~ADC10ENC;
	ADC10CTL0 &= ~ADC10ON;

	return voltage;
}

int16_t adc_get_temperature(void) {
	long temperature;

	/* configure ADC
     *   * 30 microsecond conversion time minimum
     *   * conversion clock ACLK = FDCO/2
     *   * FDCO/32 = 5.37 MHz/2 -> 370 ns period
     *   * ~30 us required sampling time
     *   * using 128 cycles for S&H is more than enough
     *   * max sample rate is ~13 kHz, so low speed mode can be used
     */
	ADC10CTL0 = ADC10SHT_6 + ADC10ON;
	ADC10CTL1 = ADC10SHP + ADC10SSEL0;	
	ADC10CTL2 = ADC10RES + ADC10SR;
	ADC10MCTL0 = ADC10SREF_1 + ADC10INCH_10;
	ADC10IE |= ADC10IE0;

	/* Configure internal reference */
	while(REFCTL0 & REFGENBUSY);
	REFCTL0 |= REFVSEL_0 + REFON;
    hw_delay_us(50);

	/* take ADC reading */
	ADC10CTL0 |= ADC10ENC + ADC10SC;
	__bis_SR_register(CPUOFF + GIE);
	temperature = adc_result;
	temperature = (temperature - CALADC10_15V_30C) *  (85-30) / (CALADC10_15V_85C-CALADC10_15V_30C) + 30;

	/* disable ADC */
	REFCTL0 &= ~REFON;
	ADC10IE &= ~ADC10IE0;
    ADC10CTL0 &= ~ADC10ENC;
	ADC10CTL0 &= ~ADC10ON;

	return temperature;
}

#pragma vector=ADC10_VECTOR
__interrupt void ADC10_ISR(void)
{
    switch(ADC10IV)
    {
        case  0: break;                         // No interrupt
        case  2: break;                         // conversion result overflow
        case  4: break;                         // conversion time overflow
        case  6: break;                         // ADC10HI
        case  8: break;                         // ADC10LO
        case 10: break;                         // ADC10IN
        case 12:
            adc_result = ADC10MEM0;             // ADC10MEM0
            __bic_SR_register_on_exit(CPUOFF);
            break;
		default: break;
	}
}
