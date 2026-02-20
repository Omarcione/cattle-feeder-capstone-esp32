#include <Arduino.h>
#include <deque>
#include "protocol.hpp"
#include "rfid.hpp"
#include "load_cell.hpp"
#include "wifi.hpp"
#include "carbonDioxideSensor.hpp"

String cmd;
const uint8_t DEV_ID = 1;

const uint32_t PUBLISH_INTERVAL_MS = 120000; // publish every 2 minutes
const uint32_t HEARTBEAT_INTERVAL_MS = 1800000; // publish heartbeat every 30 minutes
const uint32_t RECORD_INTERVAL_MS = 15000; // read data every 15 seconds

std::deque<TelemetryData_t> data_buffer; // buffer to store unsent data when WiFi is unavailable

unsigned long lastPublishTime = 0;

unsigned long lastRecordTime = 0;

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

    initTime();
}

void loop()
{
    TelemetryData_t latest_data;
    float weight;
    int id, co2;
    
    unsigned long now = millis();

    updateRFID();
    
    if (rfidHasNewReading()) 
        latest_data.rfid_id = rfidGetReading();
    else latest_data.rfid_id = -1;

    if ((latest_data.rfid_id > 0) || (lastRecordTime - now >= RECORD_INTERVAL_MS)) { // record data when there is a new RFID reading or every 15 sec

        loadcell_read(latest_data.weight_g, millis());

        latest_data.co2_ppm = co2SensorRead();
        latest_data.ch4_ppm = 0; // CH4 not implemented yet
        latest_data.device_id = DEV_ID; // TODO: set unique device ID for each feeder

        latest_data.time_ms = time(nullptr);

        data_buffer.push_back(latest_data); // buffer data for later sending

        lastRecordTime = now;
    }


    if (!data_buffer.empty() && now - lastPublishTime >= PUBLISH_INTERVAL_MS) { // publish at most once per 2 minutes if there is data

        initWiFi(); // ensure WiFi is connected before publishing
        connectAWS(); // ensure MQTT is connected before publishing

        while (data_buffer.size() > 0) {
            TelemetryData_t data_to_send = data_buffer.front();
            publishMessage(data_to_send);
            data_buffer.pop_front(); // remove sent data from buffer
        }
        disconnectWiFi(); // disconnect WiFi to save power after publishing

        lastPublishTime = now;
    }

    else if (data_buffer.empty() && now - lastPublishTime >= HEARTBEAT_INTERVAL_MS) { // publish every 15 minutes if there is no data,

        initWiFi(); // ensure WiFi is connected before publishing
        connectAWS(); // ensure MQTT is connected before publishing

        publishMessage(TelemetryData_t{DEV_ID, 0, 0, 0, 0}); // publish empty data to indicate device is alive

        disconnectWiFi(); // disconnect WiFi to save power after publishing

        lastPublishTime = now;
    }
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