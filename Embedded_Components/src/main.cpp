#include <Arduino.h>
#include "rfid.hpp"

void setup()
{
    Serial.begin(115200);
    Serial.println("initializing...");

    initRFID();

    Serial.println("starting...");
}

void loop()
{
    updateRFID();
}
