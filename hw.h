#ifndef HW_H_
#define HW_H_

enum hw_module_state {MODULE_DISABLE, MODULE_ENABLE};

/*
 * hw_init()
 *
 * Initializes the necessary functionality for low power operation:
 *   * DCO is ACLK and MCLK, SMCLK is XT1
 *   * unused pins are output, tied to GND
 *   * hardware enable pins are output, tied to Vcc (disable voltages)
 *   * UART and I2C pins are output, tied to GND
 *   * ADC pins are input, routed to ADC
 *   * 1PPS input is output, tied to GND
 */
void hw_init(void);

/* 
 * hw_gps_config()
 *
 * Enables GPS and corresponding peripherals:
 *   * UART and 1PPS pins are initialized
 *   * Power MOSFET on
 *   * UART is configured and released from reset
 *
 * Disables GPS and corresponding peripherals:
 *   * UART is put into reset
 *   * Power MOSFET off
 *   * UART and 1PPS pins are deinitalized
 */
void hw_gps_config(hw_module_state state);

/*
 * hw_rf_config()
 *
 * Enables RF section and corresponding peripherals:
 *   * I2C Pins are initialized
 *   * Power MOSFET on
 *   * I2C is configured and released from reset
 *   * XT1 and Timer B are initialized
 *
 * Disables RF section and correspondig peripherals:
 *   * XT1 and Timer B are deinitialied
 *   * I2C is put into reset
 *   * Power MOSFET off
 *   * I2C pins are deinitialized
 */
void hw_rf_config(hw_module_state state);

#endif
