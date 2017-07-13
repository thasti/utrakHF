#ifndef ADC_H_
#define ADC_H_

typedef enum {CHANNEL_VSOL, CHANNEL_VBAT} adc_voltage_channel;

#define ADC_CHAN_VSOL   ADC10INCH_1
#define ADC_CHAN_VBAT   ADC10INCH_2

#define CALADC10_15V_30C  *((unsigned int *)0x1A1A)
#define CALADC10_15V_85C  *((unsigned int *)0x1A1C)

/*
 * hw_adc_get_voltage()
 *
 * returns the voltage of a specified channel in mV
 */
uint16_t adc_get_voltage(adc_voltage_channel channel);

/*
 * hw_adc_get_temperature()
 *
 * the die temperature is returned in °C
 */
int16_t adc_get_temperature(void);

#endif
