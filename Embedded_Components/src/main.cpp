#include <Arduino.h>
#include "rfid.hpp"
#include "load_cell.hpp"

void setup()
{
    Serial.begin(115200);
    Serial.println("initializing...");

    initRFID();
    initLoadCell();

    Serial.println("starting...");
}

void loop()
{
    updateRFID();
    loadCellRead();
}
