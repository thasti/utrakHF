#ifndef SI5351_H_
#define SI5351_H_

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
void si5351_set_channel(uint16_t channel);

#endif

