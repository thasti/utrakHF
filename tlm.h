#ifndef TLM_H_
#define TLM_H_

/*
 * tlm_encode_wspr_primary()
 *
 * Encodes a "regular" WSPR packet with regular callsign and four-digit 
 * locator. Reuses the power field for coarse altitude information.
 */
void tlm_encode_wspr_primary(int32_t lat, int32_t lon, uint16_t alt, uint8_t *tx_buffer);

/*
 * tlm_encode_wspr_secondary()
 *
 * Encodes an extended WSPR packet. Uses invalid digits in the callsign
 * to differentiate from regular calls, but using them for identification.
 * Four digit locator is correctly encoded, other callsign bits and power
 * field are reused for additional telemetry.
 */
void tlm_encode_wspr_secondary(int32_t lat, int32_t lon, uint16_t alt, uint16_t temp, uint16_t vsol, uint16_t vbat, uint8_t *tx_buffer);

#endif
