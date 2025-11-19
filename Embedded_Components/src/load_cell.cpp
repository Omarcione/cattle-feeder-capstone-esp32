#include "load_cell.hpp"

HX711 scale;

void initLoadCell() {
  Serial.begin(57600);
  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);

  // wait for HX711 ready
  unsigned long start = millis();
  while (!scale.is_ready() && millis() - start < 2000) {
    // do nothing
  }

  if (scale.is_ready()) {
    scale.tare();
    Serial.println("Scale init complete.");
  } else {
    Serial.println("HX711 not ready, skipping tare.");
  }
}

long loadCellRead() {
  if (scale.wait_ready_timeout(1000)){
    long weight = scale.read();
    Serial.println(weight);
    return weight;
  }
  return -1;
}