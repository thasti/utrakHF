#ifndef TLM_H_
#define TLM_H_
#include "wspr.h"

/* six-digit callsign string */
#define WSPR_PRIMARY_CALLSIGN       "DK3SB "
/* two one-digit balloon indicators for secondary packets */
/* one of '0' or 'Q' */
#define WSPR_BALLOON_INDICATOR_A    '1'
/* one of '0' .. '9' */
#define WSPR_BALLOON_INDICATOR_B    '1'

/*
 * tlm_encode_wspr_primary()
 *
 * Encodes a "regular" WSPR packet with regular callsign and four-digit 
 * locator. Reuses the power field for temperature information.
 */
void tlm_encode_wspr_primary(int32_t lat, int32_t lon, int16_t temp, struct wspr_message *message);

/*
 * tlm_encode_wspr_secondary()
 *
 * Encodes an extended WSPR packet. Uses invalid digits in the callsign
 * to differentiate from regular calls, but using them for identification.
 * Four digit locator is correctly encoded, other callsign bits and power
 * field are reused for additional telemetry.
 */
void tlm_encode_wspr_secondary(int32_t lat, int32_t lon, uint16_t alt, uint16_t vsol, uint16_t vbat, struct wspr_message *message);

#endif
