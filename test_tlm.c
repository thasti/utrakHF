#ifndef MSP430

#include <stdio.h>
#include <inttypes.h>
#include "tlm.h"
#include "wspr.h"

int main(void) {
    struct wspr_message msg;
    int i;

    int32_t lat = 50.321e7f;
    int32_t lon = 12.256e7f;
    uint16_t alt = 400;
    uint16_t vsol = 500;
    uint16_t vbat = 4800;

    tlm_encode_wspr_secondary(lat, lon, alt, vsol, vbat, &msg);
    for (i = 0; i < WSPR_CALLSIGN_BYTES; i++) {
        printf("%c", msg.callsign[i]);
    }
    printf(" ");
    for (i = 0; i < WSPR_LOCATOR_BYTES; i++) {
        printf("%c", msg.locator[i]);
    }
    printf(" ");
    printf("%d\n", msg.power);

}

#endif
