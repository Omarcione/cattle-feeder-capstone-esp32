#include <Arduino.h>
#include "rfid.hpp"
#include "load_cell.hpp"
#include "wifi.hpp"
#include "carbonDioxideSensor.hpp"
#include "protocol.hpp"

String cmd;

bool new_data_ready = false;
TelemetryData_t latest_data;

void setup()
{
    Serial.begin(115200);
    Serial.println("initializing...");

    initRFID();

    if (!loadcell_init()) {
        Serial.println("Load cell init failed.");
        exit(1);
    }

    if (!co2SensorSetup()) {
        Serial.println("CO2 init failed.");
        exit(1);
    }
    
    Serial.println("starting...");
    Serial.println("Type 'cal' + enter to calibrate load cell.");
}

void loop()
{
    float weight;
    int id, co2;
    
    updateRFID();
    
    if (rfidHasNewReading()) 
        id = rfidGetReading();
    else id = -1;

    loadcell_read(weight, millis());

    co2 = co2SensorRead();
    // ---- command parser ---- 
    //only calibrate implemented rn
    parse_commands();
}

void parse_commands(void) {
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
