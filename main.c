#include <inttypes.h>
#include "adc.h"
#include "fix.h"
#include "gps.h"
#include "hw.h"
#include "main.h"
#include "si5351.h"
#include "tlm.h"
#include "wspr.h"

struct state;
typedef void state_fn(struct state *);

struct state {
    state_fn *next;
    uint8_t wspr_symbol_count;
};

state_fn charge, get_gps_fix, transmit;

volatile uint16_t isr_flags = 0;
struct gps_fix fix;
uint8_t wspr_payload_buffer[WSPR_PAYLOAD_BYTES];
uint8_t wspr_tx_symbol_buffer[WSPR_NUM_SYMBOLS];

void charge(struct state *state) {
    uint16_t vbat, vsol;

    isr_flags |= ISR_FLAG_WAKE_CPU;
    hw_watchdog_feed();
    hw_enter_low_power_mode();
    isr_flags &= ~ISR_FLAG_WAKE_CPU;
    vbat = adc_get_voltage(CHANNEL_VBAT);
    vsol = adc_get_voltage(CHANNEL_VBAT);
    if (vbat > CHARGE_MIN_VSOL && vsol > CHARGE_MIN_VBAT) {
        /* battery charged, ready to proceed */
        state->next = get_gps_fix; 
        hw_gps_config(MODULE_ENABLE);
        gps_startup_delay();
        gps_disable_nmea_output();
        gps_set_airborne_model();
    }
}

void get_gps_fix(struct state *state) {
    uint16_t vbat, vsol;
    int16_t temp;
    
    while (!(isr_flags & ISR_FLAG_HEARTBEAT));
    isr_flags &= ~ISR_FLAG_HEARTBEAT;

    hw_watchdog_feed();
    vbat = adc_get_voltage(CHANNEL_VBAT);
    vsol = adc_get_voltage(CHANNEL_VBAT);
    temp = adc_get_temperature();
    gps_get_fix(&fix);
    if (vbat < CHARGE_MIN_VBAT || vsol < CHARGE_MIN_VSOL) {
        state->next = charge;
        hw_gps_config(MODULE_DISABLE);
    }
    if (fix.type == 3) {
        /* valid fix exists */
        if ((fix.min % 2) == 0 && fix.sec >= 0 && fix.sec <= 1) {
            /* start WSPR transmission */
            if (((fix.min / 2) % 2) == 0) {
                tlm_encode_wspr_primary(fix.lat, fix.lon, fix.alt, wspr_payload_buffer);
            } else {
                tlm_encode_wspr_secondary(fix.lat, fix.lon, fix.alt, temp, vsol, vbat, wspr_payload_buffer);
            }
            state->wspr_symbol_count = 0;
            state->next = transmit;
            hw_gps_config(MODULE_DISABLE);
            hw_rf_config(MODULE_ENABLE);
            si5351_init();
            isr_flags &= ~ISR_FLAG_WSPR_BAUD;
            hw_reset_wspr_baud_timer();
        }
    }
}

void transmit(struct state *state) {
    while (!(isr_flags & ISR_FLAG_WSPR_BAUD));
    isr_flags &= ~ISR_FLAG_WSPR_BAUD;
    
    hw_watchdog_feed();
    if (state->wspr_symbol_count < WSPR_NUM_SYMBOLS) {
        si5351_set_channel(wspr_tx_symbol_buffer[state->wspr_symbol_count++]);
    } else {
        state->next = charge;
        hw_rf_config(MODULE_DISABLE);
    }
}

int main(void) {
    struct state state = {charge, 0};
    hw_init();
    
    while(1) state.next(&state);
}
