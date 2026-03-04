#pragma once
#include <Arduino.h>
#include <stdint.h>
#include <Rfid134.h>

#define HardwareSerial_Rx 16
#define HardwareSerial_Tx -1 // Use -1 if TX pin is not needed


// Forward declarations of RFID handler + notify class
class RfidNotify;
extern Rfid134<HardwareSerial, RfidNotify> rfid;

// Function prototypes
void initRFID();
void updateRFID();
bool rfidHasNewReading();
uint64_t rfidGetReading();