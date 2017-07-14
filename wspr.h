#ifndef WSPR_H_
#define WSPR_H_

#define WSPR_CALLSIGN_BYTES     6
#define WSPR_LOCATOR_BYTES      4
#define WSPR_NUM_SYMBOLS        162

struct wspr_message {
    uint8_t callsign[WSPR_CALLSIGN_BYTES];  /* ASCII string */
    uint8_t locator[WSPR_LOCATOR_BYTES];    /* ASCII string */
    uint8_t power;                          /* valid dBm values */
    uint8_t tx_symbol_buffer[WSPR_NUM_SYMBOLS];
};

/*
 * wspr_encode_channel_symbols
 *
 * Creates the 162 channel symbols from callsign, locator and power level
 */
void wspr_encode(struct wspr_message *message);

#endif
