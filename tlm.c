#include <inttypes.h>
#include "tlm.h"
#include "wspr.h"

void tlm_encode_wspr_primary(int32_t lat,
        int32_t lon,
        uint16_t alt,
        uint8_t *tx_buffer) {
    // TODO implement telemetry packet encoding
}


void tlm_encode_wspr_secondary(int32_t lat,
        int32_t lon,
        uint16_t alt, 
        uint16_t temp,
        uint16_t vsol,
        uint16_t vbat,
        uint8_t *tx_buffer) {
    // TODO implement telemetry packet encoding
}
