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

void publishMessage(char *string) {
    JsonDocument doc;
    doc["time"] = millis();
    doc["message"] = string;
    char jsonBuffer[512];
    serializeJson(doc, jsonBuffer); // print to client

    MQTTclient.publish(AWS_IOT_PUBLISH_TOPIC, jsonBuffer);
}