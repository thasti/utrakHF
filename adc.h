#ifndef ADC_H_
#define ADC_H_

enum adc_voltage_channel {CHANNEL_VSOL, CHANNEL_VBAT};

/*
 * hw_adc_get_voltage()
 *
 * returns the voltage of a specified channel in mV
 */
uint16_t adc_get_voltage(hw_adc_voltage_channel channel);

/*
 * hw_adc_get_temperature()
 *
 * the die temperature is returned in °C
 */
int16_t adc_get_temperature(void);

#endif
