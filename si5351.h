#ifndef SI5351_H_
#define SI5351_H_

#define SI5351_INIT_REGISTERS   164
#define SI5351_UPDATE_REGISTERS 10
#define SI5351_I2C_ADDR         0x60

/*
 * si5351_init()
 *
 * Initializes the Si5351 and starts transmission
 */
void si5351_init(void);

/* si5351_set_channel()
 *
 * Programs the Si5351 frequency to one of the specified channels.
 */
void si5351_set_channel(uint8_t channel);

/* si5351_startup_delay()
 *
 * Waits for the Si5351 to start up after power on
 */
void si5351_startup_delay(void);

#endif

