#include <Arduino.h>
#include "secrets.h"
#include <pgmspace.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include "WiFi.h"
#include <ArduinoJson.h>
#include <MQTTClient.h>

#define THINGNAME "COW_FEEDER"
#define AWS_IOT_PUBLISH_TOPIC "esp32/pub" // MQTT topic to publish to

void initWiFi();
void connectAWS();
void publishMessage(char *string);
