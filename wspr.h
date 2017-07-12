#ifndef WSPR_H_
#define WSPR_H_

/*
 * wspr_encode_channel_symbols
 *
 * Creates the 162 channel symbols from callsign, locator and power level
 */
void wspr_encode(uint8_t *callsign, uint8_t *locator, uint8_t power, uint8_t *ch_syms);

#endif
