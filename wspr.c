#include <inttypes.h>
#include "wspr.h"

uint8_t wspr_ascii_to_alphanum(uint8_t c);
uint8_t wspr_ascii_to_alpha(uint8_t c);
uint8_t wspr_ascii_to_num(uint8_t c);

const uint8_t wspr_sync_vector[WSPR_NUM_SYMBOLS] = {
    1, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 1, 1, 0, 0, 0,
    1, 0, 0, 1, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0,
    0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 1, 1, 0, 0, 1, 1,
    0, 1, 0, 0, 0, 1, 1, 0, 1, 0, 0, 0, 0, 1, 1, 0, 1, 0,
    1, 0, 1, 0, 1, 0, 0, 1, 0, 0, 1, 0, 1, 1, 0, 0, 0, 1,
    1, 0, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 1,
    0, 0, 1, 1, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1, 0, 0, 0, 1,
    1, 1, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 1, 1, 0, 0, 0, 0,
    0, 0, 0, 1, 1, 0, 1, 0, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0
};

void wspr_encode(struct wspr_message *message) {
    int i;
    uint32_t callsign_enc, locator_power_enc;

    /* encode ASCII strings and power level to packed binary representations */
    callsign_enc = wspr_ascii_to_alphanum(message->callsign[0]);
    callsign_enc = 36 * callsign_enc + wspr_ascii_to_alphanum(message->callsign[1]);
    callsign_enc = 10 * callsign_enc + wspr_ascii_to_num(message->callsign[2]);
    callsign_enc = 27 * callsign_enc + wspr_ascii_to_alpha(message->callsign[3]);
    callsign_enc = 27 * callsign_enc + wspr_ascii_to_alpha(message->callsign[4]);
    callsign_enc = 27 * callsign_enc + wspr_ascii_to_alpha(message->callsign[5]);

    locator_power_enc = (179 - 10 * wspr_ascii_to_alpha(message->locator[0]) 
                         - wspr_ascii_to_num(message->locator[2])) * 180
                      + 10 * wspr_ascii_to_alpha(message->locator[1])
                      + wspr_ascii_to_num(message->locator[3]);
    locator_power_enc = 128 * locator_power_enc + 64 + message->power;
         
    // create bit stream
    // convolutional encode
    // interleave bits
    /* add sync vector */
    for (i = 0; i < WSPR_NUM_SYMBOLS; i++) {
        message->tx_symbol_buffer[i] = 2*message->tx_symbol_buffer[i] + wspr_sync_vector[i];
    }
}

/*
 * wspr_ascii_to_alphanum()
 *
 * converts an ASCII character for any of the alphanumeric fields
 * in the WSPR spec. Valid ASCII-chars: 'A'..'Z', '0'..'9', ' '
 */
uint8_t wspr_ascii_to_alphanum(uint8_t c) {
    if (c >= '0' && c <= '9') {
        /* return index in range 0 .. 9 */
        return c - '0';
    }
    if (c >= 'A' && c <= 'Z') {
        /* return index in range 10 .. 35 */
        return c - 'A' + 10;
    }
    if (c == ' ') {
        return 36;
    }

    return 0;
}

/*
 * wspr_ascii_to_num()
 *
 * converts an ASCII character for any of the numeric fields
 * in the WSPR spec. Valid ASCII-chars: '0'..'9'
 */
uint8_t wspr_ascii_to_num(uint8_t c) {
    if (c >= '0' && c <= '9') {
        /* return index in range 0 .. 9 */
        return c - '0';
    }

    return 0;
}

/*
 * wspr_ascii_to_alpha()
 *
 * converts an ASCII character for any of the alphabetic fields
 * in the WSPR spec. Valid ASCII-chars: 'A'..'Z', ' '
 */
uint8_t wspr_ascii_to_alpha(uint8_t c) {
    if (c == '0') {
        return 26;
    }
    if (c >= 'A' && c <= 'Z') {
        /* return index in range 0 .. 25 */
        return c - 'A';
    }

    return 0;
}
