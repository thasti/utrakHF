#include <inttypes.h>
#include "tlm.h"
#include "wspr.h"

const uint8_t tlm_wspr_power_table[19] = {0, 3, 7, 10, 13, 17,
                                         20, 23, 27, 30, 33, 37,
                                         40, 43, 47, 50, 53, 57, 
                                         60};

uint8_t tlm_encode_wspr_vsol(uint16_t voltage);
uint8_t tlm_encode_wspr_vbat(uint16_t voltage);
uint8_t tlm_encode_wspr_temp(int16_t temperature);

void tlm_encode_wspr_primary(int32_t lat,
        int32_t lon,
        uint16_t alt,
        struct wspr_message *message) {
    int i;
    uint32_t ulat;
    uint32_t ulon;
    
    /* set callsign */
    for (i = 0; i < WSPR_CALLSIGN_BYTES; i++) {
        message->callsign[i] = WSPR_PRIMARY_CALLSIGN[i]; 
    }

    /* set locator */
    ulon = lon + 1800000000;
    ulat = lat + 900000000;

    message->locator[0] = 'A' + ulon / 200000000;
    message->locator[1] = 'A' + ulat / 100000000;
    message->locator[2] = '0' + (ulon % 200000000) / 20000000;
    message->locator[3] = '0' + (ulat % 100000000) / 10000000;

    /* set power level / altitude */
    // TODO review altitude calculation
    //  negative dBm values are possible, maybe more resolution is available.
    alt = alt / 1000;
    if (alt > sizeof(tlm_wspr_power_table)-1) {
        alt = sizeof(tlm_wspr_power_table)-1;
    }
    message->power = tlm_wspr_power_table[alt / 1000];
}

void tlm_encode_wspr_secondary(int32_t lat,
        int32_t lon,
        uint16_t alt, 
        uint16_t temp,
        uint16_t vsol,
        uint16_t vbat,
        struct wspr_message *message) {
    uint32_t ulat;
    uint32_t ulon;
    uint32_t tlm_bignum;
    uint8_t tlm_alt, tlm_temp, tlm_vbat, tlm_vsol, tlm_grid5, tlm_grid6;

    /* set balloon indicator */
    message->callsign[0] = WSPR_BALLOON_INDICATOR_A;
    message->callsign[2] = WSPR_BALLOON_INDICATOR_B;

    /* calculate locator */
    ulon = lon + 1800000000;
    ulat = lat + 900000000;

    message->locator[0] = 'A' + ulon / 200000000;
    message->locator[1] = 'A' + ulat / 100000000;
    message->locator[2] = '0' + (ulon % 200000000) / 20000000;
    message->locator[3] = '0' + (ulat % 100000000) / 10000000;
    tlm_grid5 = ((ulon - ((ulon / 20000000) * 20000000)) / (50000000/60));
    tlm_grid6 = ((ulat - ((ulat / 10000000) * 10000000)) / (25000000/60));
    if (tlm_grid5 > 23) tlm_grid5 = 23;
    if (tlm_grid6 > 23) tlm_grid6 = 23;
    
    /* encode telemetry */
    tlm_alt = (alt - (alt / 1000) * 1000) / 10;
    tlm_vbat = tlm_encode_wspr_vbat(vbat);
    tlm_vsol = tlm_encode_wspr_vsol(vsol);
    tlm_temp = tlm_encode_wspr_temp(temp);

    tlm_bignum = tlm_grid5;
    tlm_bignum = tlm_bignum * 24 + tlm_grid6;
    tlm_bignum = tlm_bignum * 10 + tlm_alt;
    tlm_bignum = tlm_bignum * 12 + tlm_temp;
    tlm_bignum = tlm_bignum * 15 + tlm_vbat;
    tlm_bignum = tlm_bignum * 10 + tlm_vsol;

    /* encode the telemetry value into the callsign chars */
    message->power = tlm_wspr_power_table[tlm_bignum % 19];
    tlm_bignum /= 19;
    message->callsign[5] = (tlm_bignum % 27);
    tlm_bignum /= 27;
    message->callsign[4] = tlm_bignum % 26;
    tlm_bignum /= 26;
    message->callsign[3] = tlm_bignum % 26;
    tlm_bignum /= 26;
    message->callsign[1] = tlm_bignum;

    /* convert values to ASCII */
    if (message->callsign[5] == 26) {
        message->callsign[5] = ' ';
    } else {
        message->callsign[5] += 'A';
    }
    message->callsign[4] += 'A';
    message->callsign[3] += 'A';
    /* message->callsign[1] is < 36, therefore no need to handle a space */
    if (message->callsign[1] < 10) {
        message->callsign[1] += '0';
    } else {
        message->callsign[1] += 'A';
    }
}

uint8_t tlm_encode_wspr_vbat(uint16_t voltage) {
    /* Battery voltage is nominally in range 0 .. 5000 mV */
    /* Encoding in 15 steps:
     *   0   .. 3.6 V = 0,
     *   3.6 .. 3.7 V = 1,
     *   3.7 .. 3.8 V = 2,
     *   ...,
     *   4.9 .. inf V = 14
     */
    if (voltage < 3500) {
        return 0;
    }
    if (voltage > 4999) {
        return 14;
    }

    return (voltage / 100) - 35;
}

uint8_t tlm_encode_wspr_vsol(uint16_t voltage) {
    /* Solar voltage is in range 0 .. 1500 mV */
    /* Encoding in 10 steps: 
     *   0.0 .. 0.2 V = 0
     *   0.2 .. 0.4 V = 1
     *   ..., 
     *   1.8 .. inf V = 9
     */
    if (voltage > 2000) {
        return 9;
    }

    return voltage / 200;
}

uint8_t tlm_encode_wspr_temp(int16_t temperature) {
    /* temperature is in nominal range -40°C .. +15 °C */
    /* Encoding in 12 steps:
     *   -inf .. -35 °C = 0,
     *   -35  .. -30 °C = 1,
     *   ...
     *   10   .. 15 °C = 11
     */
    if (temperature < -40) {
        return 0;
    }
    if (temperature > 15) {
        return 11;
    }

    return (temperature + 40) / 5;

}
