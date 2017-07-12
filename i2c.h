#ifndef I2C_H_
#define I2C_H_

enum i2c_config_state {I2C_DISABLE, I2C_ENABLE};

void i2c_config(i2c_config_state state);
void i2c_write(uint8_t addr, uint8_t data, uint8_t len);

#endif
