#pragma once
#include <Arduino.h>
#include <stdint.h>
#include <Rfid134.h>

#define HardwareSerial_Rx 16
#define HardwareSerial_Tx -1 // Use -1 if TX pin is not needed

// Global ID variable if needed elsewhere
extern uint64_t ID_Number;

// Forward declarations of RFID handler + notify class
class RfidNotify;
extern Rfid134<HardwareSerial, RfidNotify> rfid;

// Function prototypes
void initRFID();
void updateRFID();
bool rfidHasNewReading();
int rfidGetReading();