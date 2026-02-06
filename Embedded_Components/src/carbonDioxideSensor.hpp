#include <Wire.h>
#include <7Semi_SCD4x.h>

// I2C speed for bring-up (100 kHz recommended; raise later if stable)
#define I2C_ADDR 0x62
#define I2C_FREQ_HZ 100000UL

// Set to -1 to use board defaults
#define I2C_SDA_PIN -1
#define I2C_SCL_PIN -1

void co2SensorSetup();
void co2SensorRead();