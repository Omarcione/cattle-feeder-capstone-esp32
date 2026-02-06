/***************************************************************
 * @file    SingleShot.ino
 * @brief   Example for running the 7Semi SCD4x sensor in 
 *          single-shot mode using fixed delays (no data-ready polling).
 *
 * Features demonstrated:
 * - CO2+RHT single shot: wait ~5000 ms
 * - RHT-only single shot: wait ~1000 ms
 *
 * Sensor configuration used:
 * - Mode            : Single-shot (CO₂+RHT or RHT-only)
 * - Delay After CO₂ : ~5000 ms
 * - Delay After RHT : ~1000 ms
 * - Interval        : 5000 ms between single-shots
 * - I²C Frequency   : 100 kHz
 * - Data Output     : CO₂ (ppm), Temperature (°C), RH (%)
 *
 * Notes:
 * - Sensor must be IDLE (not in periodic mode) before single-shot.
 * - CO₂ single-shot is only valid on SCD41; SCD40 often outputs no CO₂.
 * - Avoid using data-ready polling in single-shot mode; rely on fixed delays.
 *
 * Connections:
 * - SDA -> Default board SDA (or custom pin if supported)
 * - SCL -> Default board SCL (or custom pin if supported)
 * - VIN -> 3.3V / 5V (depending on module)
 * - GND -> GND
 *
 * @author   7Semi
 * @license  MIT
 * @version  1.0
 * @date     24 September 2025
 ***************************************************************/

#include <Wire.h>
#include <7Semi_SCD4x.h>
#include "carbonDioxideSensor.hpp"

SCD4x_7Semi scd;

const uint32_t PERIOD_BETWEEN_SHOTS = 5000UL;  // how often to trigger single-shot
const uint32_t SS_DELAY_CO2_MS = 5000UL;       // wait after measureSingleShot()
const uint32_t SS_DELAY_RHT_MS = 1000UL;       // wait after measureSingleShotRhtOnly()

void co2SensorSetup() {
  Serial.begin(115200);
  while (!Serial) {}

  Serial.println(F("SCD4x: Single-shot (fixed delay)"));
  while (!scd.begin()) {
    // while (!scd.begin(I2C_ADDR,I2C_SDA_PIN, I2C_SCL_PIN, I2C_FREQ_HZ)) {
    Serial.println(F("ERROR: not detected"));
    while (1) delay(1000);
  }

  // Ensure we’re not in periodic mode
  scd.stopPeriodicMeasurement();
  delay(500);
  // --- RESET ANY PERSISTED OFFSET ---
  scd.stopPeriodicMeasurement();
  delay(500);

  float oldOff = NAN;
  if (scd.getTemperatureOffset(oldOff)) {
    Serial.print(F("Stored offset (before): "));
    Serial.println(oldOff, 2);
  }

  if (!scd.setTemperatureOffset(15.0f)) {
    Serial.println(F("WARN: setTemperatureOffset(0) failed"));
  }

  // Persist & reload so it sticks
  scd.persistSettings();
  scd.reInit();
  delay(200);

  // Confirm it's zero now
  float nowOff = NAN;
  if (scd.getTemperatureOffset(nowOff)) {
    Serial.print(F("Stored offset (now): "));
    Serial.println(nowOff, 2);
  } else {
    Serial.println(F("WARN: getTemperatureOffset failed"));
  }

  // Optional: print variant to know if you have SCD40 vs SCD41
  uint16_t var = 0;
  if (scd.getSensorVariantRaw(var)) {
    Serial.print(F("Variant raw: 0x"));
    Serial.println(var, HEX);
  }
}

void co2SensorRead() {
  static uint32_t last = 0;
  if (millis() - last < PERIOD_BETWEEN_SHOTS) return;
  last = millis();

  // ---- Single-shot CO2 + RHT ----
  if (!scd.measureSingleShot()) {
    Serial.println(F("measureSingleShot() failed (CO2+RHT)"));
    return;
  }

  delay(SS_DELAY_CO2_MS);  // DO NOT poll data-ready in single-shot mode

  // co2, tc, rh is the measurements
  uint16_t co2;
  float tc, rh;
  if (scd.readMeasurement(co2, tc, rh)) {
    Serial.print(F("[CO2+RHT] CO2 "));
    Serial.print(co2);
    Serial.print(F(" ppm  "));
    Serial.print(F("T "));
    Serial.print(tc, 2);
    Serial.print(F(" C  "));
    Serial.print(F("RH "));
    Serial.print(rh, 1);
    Serial.println(F(" %"));
  } else {
    Serial.println(F("readMeasurement() failed after single-shot (CO2+RHT)"));
  }
}
