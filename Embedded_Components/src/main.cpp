#include <Arduino.h>
#include "rfid.hpp"
#include "load_cell.hpp"
#include "wifi.hpp"

void setup()
{
    Serial.begin(115200);
    Serial.println("initializing...");

    // initRFID();
    // initLoadCell();
    initWiFi();
    connectAWS();
    publishMessage("Hello Jack, my sunshine");


    Serial.println("starting...");
}

void loop()
{
    // updateRFID();
    // loadCellRead();

}
