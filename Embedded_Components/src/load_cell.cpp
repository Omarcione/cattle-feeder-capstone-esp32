#include "load_cell.hpp"

HX711 scale;

void initLoadCell() {
  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
  scale.set_gain(128); // channel A, high gain

  // wait for HX711 ready
  unsigned long start = millis();
  while (!scale.is_ready() && millis() - start < 2000) {
    // do nothing
  }

  if (scale.is_ready()) {
    scale.set_scale();
    scale.tare();
    Serial.println("Scale init complete.\n");
    Serial.println("put weight on scale\n");
    while (Serial.available() == 0);
    Serial.flush();
    Serial.println(scale.get_units(10));
    Serial.println("Waiting for input...\n");
    while (Serial.available() == 0);
  } else {
    Serial.println("HX711 not ready, skipping tare.");
  }
}

long loadCellRead() {
  if (scale.wait_ready_timeout(1000)) {
    long reading = scale.get_units(10);
    Serial.print("Weight: ");
    Serial.println(reading, 2);
    return reading;
  } else {
      Serial.println("HX711 not found.");
      return -1;
  }

  delay(1000);
}