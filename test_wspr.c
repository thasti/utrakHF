#ifndef MSP430

#include <stdio.h>
#include <inttypes.h>
#include "wspr.h"

int main(void) {
    struct wspr_message msg;
    int i;
    int errors = 0;

    uint8_t validation[162] = {
        3, 3, 0, 0, 2, 0, 0, 0, 1, 0, 2, 0, 1, 3, 1, 2, 2, 2, 1, 0, 0, 3, 2, 3, 1, 3, 3, 2, 2, 0,
        2, 0, 0, 0, 3, 2, 0, 1, 2, 3, 2, 2, 0, 0, 2, 2, 3, 2, 1, 1, 0, 2, 3, 3, 2, 1, 0, 2, 2, 1,
        3, 2, 1, 2, 2, 2, 0, 3, 3, 0, 3, 0, 3, 0, 1, 2, 1, 0, 2, 1, 2, 0, 3, 2, 1, 3, 2, 0, 0, 3,
        3, 2, 3, 0, 3, 2, 2, 0, 3, 0, 2, 0, 2, 0, 1, 0, 2, 3, 0, 2, 1, 1, 1, 2, 3, 3, 0, 2, 3, 1,
        2, 1, 2, 2, 2, 1, 3, 3, 2, 0, 0, 0, 0, 1, 0, 3, 2, 0, 1, 3, 2, 2, 2, 2, 2, 0, 2, 3, 3, 2,
        3, 2, 3, 3, 2, 0, 0, 3, 1, 2, 2, 2 
    };

    msg.callsign[0] = ' ';
    msg.callsign[1] = 'K';
    msg.callsign[2] = '1';
    msg.callsign[3] = 'A';
    msg.callsign[4] = 'B';
    msg.callsign[5] = 'C';
    msg.locator[0] = 'F';
    msg.locator[1] = 'N';
    msg.locator[2] = '4';
    msg.locator[3] = '2';
    msg.power = 37;

    wspr_encode(&msg);

    for (i = 0; i < WSPR_NUM_SYMBOLS; i++) {
        if (i % 30 == 0)
            printf("\n");
        printf("%d ", msg.tx_symbol_buffer[i]);
    }

    printf("\n\n");

    for (i = 0; i < WSPR_NUM_SYMBOLS; i++) {
        if (msg.tx_symbol_buffer[i] != validation[i]) {
            printf("Symbol error at index %d\n", i);
            errors++;
        }
    }

    if (errors == 0) {
        printf("Tests successful!\n");
    } else {
        printf("Tests finished with %d errors.\n", errors);
    }
    printf("\n");
}

#endif
