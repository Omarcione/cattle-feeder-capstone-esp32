#include <Arduino.h>
#include "secrets.h"
#include <pgmspace.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include "WiFi.h"
#include <ArduinoJson.h>
#include <MQTTClient.h>
#include "protocol.hpp"

#define THINGNAME "COW_FEEDER"
#define AWS_IOT_PUBLISH_TOPIC "esp32/pub" // MQTT topic to publish to

void initWiFi();
void connectAWS();
void publishMessage(TelemetryData_t data);
void initTime();
void disconnectWiFi();
