#include <inttypes.h>
#include "hw.h"
#include "si5351.h"
#include "i2c.h"

/* initialization register map */
const uint8_t si5351_reg_init[SI5351_INIT_REGISTERS][2] = {
    {  0, 0x00}, {  1, 0x00}, {  2, 0x08}, {  3, 0x00},
    {  9, 0x00}, 
    { 15, 0x0C},
    { 16, 0x87}, { 17, 0x87}, { 18, 0x6F}, { 19, 0x5F},
    { 20, 0x87}, { 21, 0x87}, { 22, 0x87}, { 23, 0x87},
    { 24, 0x00}, { 25, 0x00}, { 26, 0x7C}, { 27, 0xCC},
    { 28, 0x00}, { 29, 0x16}, { 30, 0x1D}, { 31, 0xC8},
    { 32, 0x0D}, { 33, 0x64}, { 34, 0x3E}, { 35, 0x66},
    { 36, 0x00}, { 37, 0x13}, { 38, 0x88}, { 39, 0x61},
    { 40, 0x24}, { 41, 0x50}, { 42, 0x00}, { 43, 0x00},
    { 44, 0x00}, { 45, 0x00}, { 46, 0x00}, { 47, 0x00},
    { 48, 0x00}, { 49, 0x00}, { 50, 0x00}, { 51, 0x00},
    { 52, 0x00}, { 53, 0x00}, { 54, 0x00}, { 55, 0x00},
    { 56, 0x00}, { 57, 0x00}, { 58, 0x00}, { 59, 0x01},
    { 60, 0x00}, { 61, 0x17}, { 62, 0x00}, { 63, 0x00},
    { 64, 0x00}, { 65, 0x00}, { 66, 0x00}, { 67, 0x01},
    { 68, 0x00}, { 69, 0x1A}, { 70, 0x00}, { 71, 0x00},
    { 72, 0x00}, { 73, 0x00}, { 74, 0x00}, { 75, 0x00},
    { 76, 0x00}, { 77, 0x00}, { 78, 0x00}, { 79, 0x00},
    { 80, 0x00}, { 81, 0x00}, { 82, 0x00}, { 83, 0x00},
    { 84, 0x00}, { 85, 0x00}, { 86, 0x00}, { 87, 0x00},
    { 88, 0x00}, { 89, 0x00}, { 90, 0x00}, { 91, 0x00},
    { 92, 0x00}, { 93, 0x00}, { 94, 0x00}, { 95, 0x00},
    { 96, 0x00}, { 97, 0x00}, { 98, 0x00}, { 99, 0x00},
    {100, 0x00}, {101, 0x00}, {102, 0x00}, {103, 0x00},
    {104, 0x00}, {105, 0x00}, {106, 0x00}, {107, 0x00},
    {108, 0x00}, {109, 0x00}, {110, 0x00}, {111, 0x00},
    {112, 0x00}, {113, 0x00}, {114, 0x00}, {115, 0x00},
    {116, 0x00}, {117, 0x00}, {118, 0x00}, {119, 0x00},
    {120, 0x00}, {121, 0x00}, {122, 0x00}, {123, 0x00},
    {124, 0x00}, {125, 0x00}, {126, 0x00}, {127, 0x00},
    {128, 0x00}, {129, 0x00}, {130, 0x00}, {131, 0x00},
    {132, 0x00}, {133, 0x00}, {134, 0x00}, {135, 0x00},
    {136, 0x00}, {137, 0x00}, {138, 0x00}, {139, 0x00},
    {140, 0x00}, {141, 0x00}, {142, 0x00}, {143, 0x00},
    {144, 0x00}, {145, 0x00}, {146, 0x00}, {147, 0x00},
    {148, 0x00}, {149, 0x00}, {150, 0x00}, {151, 0x00},
    {152, 0x00}, {153, 0x00}, {154, 0x00}, {155, 0x00},
    {156, 0x00}, {157, 0x00}, {158, 0x00}, {159, 0x00},
    {160, 0x00}, {161, 0x00}, {162, 0x00}, {163, 0x00},
    {164, 0x00}, {165, 0x00}, {166, 0x00}, {167, 0x00},
    {168, 0x00}, {169, 0x00}, {170, 0x00}, 
    {177, 0x00}, 
    {183, 0xD2},
    {187, 0xC0},
};

/* channel 0 (wspr symbol 0) register map */
const uint8_t si5351_reg_ch0[SI5351_UPDATE_REGISTERS][2] = {
    {26, 0x7C}, {27, 0xCC}, {31, 0xC8}, {32, 0x0D},
    {33, 0x64}, {34, 0x3E}, {35, 0x66}, {39, 0x61},
    {40, 0x24}, {41, 0x50}
};

/* channel 1 (wspr symbol 1) register map */
const uint8_t si5351_reg_ch1[SI5351_UPDATE_REGISTERS][2] = {
    {26, 0x7C}, {27, 0xCC}, {31, 0xC8}, {32, 0x0F},
    {33, 0x64}, {34, 0x9F}, {35, 0xD7}, {39, 0x00},
    {40, 0x1D}, {41, 0x48}
};

/* channel 2 (wspr symbol 2) register map */
const uint8_t si5351_reg_ch2[SI5351_UPDATE_REGISTERS][2] = {
    {26, 0x7F}, {27, 0x5C}, {31, 0x21}, {32, 0x9D},
    {33, 0x14}, {34, 0x7F}, {35, 0x5C}, {39, 0x20},
    {40, 0x75}, {41, 0xA0}
};

/* channel 3 (wspr symbol 3) register map */
const uint8_t si5351_reg_ch3[SI5351_UPDATE_REGISTERS][2] = {
    {26, 0x7C}, {27, 0xCC}, {31, 0xC8}, {32, 0x13},
    {33, 0x64}, {34, 0x7C}, {35, 0x88}, {39, 0xC2},
    {40, 0x4E}, {41, 0x20}
};

void si5351_init(void) {
    int i;
    uint8_t reg_write[2];
    for (i = 0; i < SI5351_INIT_REGISTERS; i++) {
        reg_write[0] = si5351_reg_init[i][0];
        reg_write[1] = si5351_reg_init[i][1];
        i2c_write(SI5351_I2C_ADDR, reg_write, 2);
    }
}

void si5351_set_channel(uint8_t channel) {
    const uint8_t (*si5351_reg_update)[2];
    uint8_t reg_write[2];
    int i;

    switch (channel) {
    case 0:
        si5351_reg_update = si5351_reg_ch0;
        break;
    case 1:
        si5351_reg_update = si5351_reg_ch1;
        break;
    case 2:
        si5351_reg_update = si5351_reg_ch2;
        break;
    case 3:
        si5351_reg_update = si5351_reg_ch3;
        break;
    default: 
        si5351_reg_update = si5351_reg_ch0;
        break;
    }

    for (i = 0; i < SI5351_UPDATE_REGISTERS; i++) {
        reg_write[0] = si5351_reg_update[i][0];
        reg_write[1] = si5351_reg_update[i][1];
        i2c_write(SI5351_I2C_ADDR, reg_write, 2);
    }
}

void si5351_startup_delay(void) {
    hw_delay_ms(10);
}
