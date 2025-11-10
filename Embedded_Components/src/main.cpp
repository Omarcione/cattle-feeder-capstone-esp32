#include <Arduino.h>
#include "rfid.hpp"

constexpr uint32_t USB_BAUD = 115200;  // your USB console speed

void setup() {
  Serial.begin(USB_BAUD);
  RFID134Mod::begin(/*rxPin=*/16);  // change if you wired differently
  Serial.println("WL-134 ready.");
}

void loop() {
  RFID134Mod::poll();
}
