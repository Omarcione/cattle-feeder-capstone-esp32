#include <Arduino.h>
#include <iostream>


// Configure the UART the WL-134 reader is wired to (TX/RX pins are ESP32 pins).
constexpr int RFID_RX_PIN = 3;         // WL-134 TX -> ESP32 GPIO3 (UART0 RX)
constexpr int RFID_BAUD_RATE = 9600;  


struct RfidFrame {
  String payload;
};

