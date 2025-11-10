#include "rfid.hpp"

namespace RFID134Mod {

// Forward-declare static variables so Notify can see them
static OnTagCb userCb_ = nullptr;
static HardwareSerial* uart_ = &Serial2;
static Rfid134<HardwareSerial, class Notify>* reader_ = nullptr;

// ---- Notification handler ----
class Notify {
public:
  static void OnError(Rfid134_Error err) {
    Serial.print("RFID error: "); Serial.println(err);
  }
  static void OnPacketRead(const Rfid134Reading& reading) {
    // If user provided a callback, use it
    if (userCb_) {
      userCb_(reading);
      return;
    }

    // Default print (country + 64-bit ID with zero padding)
    Serial.print("TAG: ");
    Serial.print(reading.country);
    Serial.print(" ");

    char buf[13];
    sprintf(buf, "%06lu", (uint32_t)(reading.id / 1000000UL));
    Serial.print(buf);
    sprintf(buf, "%06lu", (uint32_t)(reading.id % 1000000UL));
    Serial.print(buf);

    if (reading.isAnimal) Serial.print(" animal");
    if (reading.isData)   Serial.print(" data");
    Serial.println();
  }
};

// ---- Module functions ----
void begin(int rxPin, int txPin, uint32_t baud, HardwareSerial& uart) {
  uart_ = &uart;
  uart_->begin(baud, SERIAL_8N2, rxPin, txPin);

  // Create the template instance bound to this UART
  static Rfid134<HardwareSerial, Notify> reader(*uart_);
  reader_ = &reader;
  reader_->begin();
}

void poll() {
  if (reader_) reader_->loop();
}

void setOnTagCallback(OnTagCb cb) {
  userCb_ = cb;
}

} // namespace RFID134Mod
