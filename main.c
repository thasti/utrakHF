#include <inttypes.h>
#include "main.h"
#include "adc.h"
#include "hw.h"
#include "tlm.h"
#include "si5351.h"

volatile uint16_t isr_flags = 0;

int main(void) {
    hw_init();

    isr_flags |= ISR_FLAG_WAKE_CPU;
    while(1) {
        hw_watchdog_feed();
        hw_enter_low_power_mode();
    }
}
