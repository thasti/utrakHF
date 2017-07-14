#ifndef WSPR_H_
#define WSPR_H_

#define WSPR_PAYLOAD_BYTES      4
#define WSPR_PAYLOAD_BITS       28
#define WSPR_NUM_SYMBOLS        162

/*
 * wspr_encode_channel_symbols
 *
 * Creates the 162 channel symbols from callsign, locator and power level
 */
void wspr_encode(uint8_t *callsign, uint8_t *locator, uint8_t power, uint8_t *ch_syms);

#endif
