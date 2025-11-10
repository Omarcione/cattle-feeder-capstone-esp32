#pragma once
#include <Arduino.h>
#include <Rfid134.h>

// Simple namespace wrapper so main.cpp stays clean
namespace RFID134Mod {

// Start the reader on a given UART + pins.
// Default: ESP32 Serial2, RX pin 16, baud 9600, 8N2 (many WL-134s prefer 8N2).
void begin(int rxPin = 16, int txPin = -1, uint32_t baud = 9600,
           HardwareSerial& uart = Serial2);

// Must be called often from loop(); parses bytes and prints tags.
void poll();

// Optional: set a callback if you don't want default Serial prints.
using OnTagCb = void(*)(const Rfid134Reading&);
void setOnTagCallback(OnTagCb cb);

} // namespace RFID134Mod
