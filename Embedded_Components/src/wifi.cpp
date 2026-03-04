#include "wifi.hpp"

// Wifi SSID AND PASSWORD are stored in secrets.h file, which is included in .gitignore
// Create your own secrets.h containing declarations for WIFI_SSID and WIFI_PASS

WiFiClientSecure net = WiFiClientSecure();
MQTTClient MQTTclient = MQTTClient(256);

void initWiFi() {
    Serial.begin(115200);
    delay(1000);

    WiFi.mode(WIFI_STA); //Optional
    WiFi.begin(WIFI_SSID, WIFI_PASS);
    Serial.println("\nConnecting");

    // Wait for connection
    while(WiFi.status() != WL_CONNECTED){
        Serial.print(".");
        delay(500);
    }

    Serial.println("\nConnected to the WiFi network");
    Serial.print("Local ESP32 IP: ");
    Serial.println(WiFi.localIP());
    return;
}

void disconnectWiFi() { // to save power when not needed
    WiFi.disconnect(true, false);
    Serial.println("Disconnected from WiFi");
}

// Initialize NTP time synchronization after connecting to WiFi
void initTime() {
    configTime(0, 0, "pool.ntp.org", "time.nist.gov");
    
    // Wait for time to be set (must call time() inside loop to get updated value)
    time_t now = time(nullptr);
    while (now < 8 * 3600 * 2) {
        delay(500);
        now = time(nullptr);  // Update time inside loop
    }
    Serial.println("\nTime synchronized");

    // convert to local time zone (PST/PDT)
    // set TZ environment variable; tzset() initializes libc timezone data
    setenv("TZ", "PST8PDT", 1);
    tzset();

    struct tm timeinfo;
    // use localtime_r instead of gmtime_r so the printed time is Pacific
    localtime_r(&now, &timeinfo);
    Serial.print("Current local time: ");
    char buf[64];
    strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S %Z", &timeinfo);
    Serial.println(buf);
}

void connectAWS() {
    net.setCACert(AWS_CERT_CA);
    net.setCertificate(AWS_CERT_CRT);
    net.setPrivateKey(AWS_CERT_PRIVATE);

    // Connect to MQTT broker on AWS
    MQTTclient.begin(AWS_IOT_ENDPOINT, 8883, net);

    Serial.print("\nAttempting to connect to AWS IoT");

    // Attempt to connect to AWS IoT with timeout
    unsigned long connectionStartTime = millis();
    const unsigned long CONNECTION_TIMEOUT_MS = 30000; // 30 second timeout
    
    while (!MQTTclient.connect(THINGNAME)) {
        Serial.print(".");
        delay(100);
        
        // Check for timeout
        if (millis() - connectionStartTime > CONNECTION_TIMEOUT_MS) {
            Serial.println("\nAWS IoT Connection Timeout!");
            return;
        }
    }

    // Connection successful
    Serial.println("\nSuccessfully Connected to AWS IoT!");
}

void publishMessage(TelemetryData_t data) {
    static uint16_t seq = 0;
    
    // Ensure MQTT connection is active
    if (!MQTTclient.connected()) {
        Serial.println("ERROR: MQTT not connected, cannot publish");
        return;
    }
    
    // Process MQTT operations
    MQTTclient.loop();
    
    JsonDocument doc;

    char timestamp[30];
    // convert packet time to local timezone (PST/PDT) before formatting
    struct tm tm_info;
    localtime_r(&data.time_ms, &tm_info);
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", &tm_info);

    doc["deviceID"] = data.device_id;
    doc["seq"] = seq++;
    doc["cowID"] = data.rfid_id;
    doc["time"] = timestamp;
    doc["weight"] = data.weight_g;
    doc["co2"] = data.co2_ppm;
    doc["ch4"] = data.ch4_ppm;

    char jsonBuffer[512];
    serializeJson(doc, jsonBuffer, sizeof(jsonBuffer));

    bool success = MQTTclient.publish(AWS_IOT_PUBLISH_TOPIC, jsonBuffer);
    
    if (success) {
        Serial.print("Published to ");
        Serial.print(AWS_IOT_PUBLISH_TOPIC);
        Serial.print(": ");
        Serial.println(jsonBuffer);
    } else {
        Serial.println("ERROR: Failed to publish message");
    }
}