#include <Arduino.h>
#include "rfid.hpp"
#include "load_cell.hpp"

String cmd;

void setup()
{
    Serial.begin(115200);
    Serial.println("initializing...");

    initRFID();

    if (!loadcell_init()) {
        Serial.println("Load cell init failed.");
        while (true) delay(100);
    }
    
    Serial.println("starting...");
    Serial.println("Type 'cal' + enter to calibrate load cell.");
}

void loop()
{
    float weight;
    
    updateRFID();

    if (loadcell_read(weight, millis())) {
        Serial.print("Load cell: ");
        Serial.println(weight, 3);
    }

    // ---- command parser ----
    while (Serial.available()) {
        char c = Serial.read();

        // newline or carriage return ends command
        if (c == '\n' || c == '\r') {
        cmd.trim();

        if (cmd == "cal") {
            Serial.println("Entering calibration mode...");
            loadcell_calibrate(Serial);
        }

        cmd = "";   // reset buffer
        } else {
        cmd += c;   // accumulate characters
        }
    }
}