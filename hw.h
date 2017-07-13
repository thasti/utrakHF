#ifndef HW_H_
#define HW_H_

#define PSEL0_ADC       P1SEL0
#define PSEL1_ADC       P1SEL1
#define BIT_ADC_VSOL    BIT1
#define BIT_ADC_VBAT    BIT2

#define POUT_VEN        P1OUT
#define PDIR_VEN        P1DIR
#define BIT_VEN_GPS     BIT4
#define BIT_VEN_RF      BIT5

#define POUT_I2C        P1OUT
#define PDIR_I2C        P1DIR
#define PSEL0_I2C       P1SEL0
#define PSEL1_I2C       P1SEL1
#define BIT_I2C_SCL     BIT7
#define BIT_I2C_SDA     BIT6

#define POUT_UART       P2OUT
#define PDIR_UART       P2DIR
#define PSEL0_UART      P2SEL0
#define PSEL1_UART      P2SEL1
#define BIT_UART_TXD    BIT0
#define BIT_UART_RXD    BIT1

#define POUT_1PPS       P2OUT
#define PDIR_1PPS       P2DIR
#define BIT_1PPS        BIT2

#define POUT_CLKIN      PJOUT
#define PIN_CLKIN       PJIN
#define PDIR_CLKIN      PJDIR
#define PSEL0_CLKIN     PJSEL0
#define PSEL1_CLKIN     PJSEL1
#define BIT_CLKIN       BIT4

#define POUT_UNUSED1    P1OUT
#define PDIR_UNUSED1    P1DIR
#define BIT_UNUSED1_A   BIT0
#define BIT_UNUSED1_B   BIT3

#define POUT_UNUSED2    P2OUT
#define PDIR_UNUSED2    P2DIR
#define BIT_UNUSED2_A   BIT3
#define BIT_UNUSED2_B   BIT4
#define BIT_UNUSED2_C   BIT5
#define BIT_UNUSED2_D   BIT6
#define BIT_UNUSED2_E   BIT7

typedef enum {MODULE_DISABLE, MODULE_ENABLE} hw_module_state;

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
 * hw_watchdog_feed()
 *
 * Resets the watchdog timer
 */
void hw_watchdog_feed(void);

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

/*
 * hw_enter_low_power_mode()
 *
 * Enters low power consumption operating mode.
 *
 * The CPU will be woken up by the heartbeat timer if the global 
 * ISR_FLAG_WAKE_CPU flag is set. There is no other way to leave this mode.
 */
void hw_enter_low_power_mode(void);

#endif
