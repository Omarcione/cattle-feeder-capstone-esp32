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

// Initialize NTP time synchronization after connecting to WiFi
void initTime() {
    configTime(-8 * 3600, 0, "pool.ntp.org", "time.nist.gov"); // PST timezone, no daylight saving time
    
    Serial.print("Waiting for NTP time sync: ");
    time_t now = time(nullptr);
    while (now < 8 * 3600 * 2) {  // Wait until time is synced
        delay(500);
        Serial.print(".");
        now = time(nullptr);
    }
    Serial.println("\nTime synchronized");
    
    struct tm timeinfo;
    gmtime_r(&now, &timeinfo);
    Serial.print("Current time: ");
    Serial.println(asctime(&timeinfo));
}

void connectAWS() {
    net.setCACert(AWS_CERT_CA);
    net.setCertificate(AWS_CERT_CRT);
    net.setPrivateKey(AWS_CERT_PRIVATE);

    // Connect to MQTT broker on AWS
    MQTTclient.begin(AWS_IOT_ENDPOINT, 8883, net);

    Serial.print("\nAttempting to connect");

    // Attempt to connect to AWS IoT
    while (!MQTTclient.connect(THINGNAME)) {
        Serial.print(".");
        delay(100);
    }

    // Check if connection was successful
    if (!MQTTclient.connected()) {
        Serial.println("\nAWS IoT Timeout!");
        return;
    }

    Serial.print("\nSuccessfully Connected!");
}

void publishMessage(uint64_t cowID, float loadCellReading, float co2Reading, float ch4Reading) {
    StaticJsonDocument<256> doc;

    time_t now = time(nullptr);
    char timestamp[30];
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", gmtime(&now));

    doc["cowID"] = cowID;
    doc["time"] = timestamp;
    doc["weight"] = loadCellReading;
    doc["co2"] = co2Reading;
    doc["ch4"] = ch4Reading;

    char jsonBuffer[512];
    serializeJson(doc, jsonBuffer, sizeof(jsonBuffer));

    MQTTclient.publish(AWS_IOT_PUBLISH_TOPIC, jsonBuffer);
}