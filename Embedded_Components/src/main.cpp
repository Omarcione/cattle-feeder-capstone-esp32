#include <Arduino.h>

// put function declarations here:
constexpr int COMPUTER_BAUD_RATE = 9600; 

void setup() {
  Serial.begin(COMPUTER_BAUD_RATE);
  Serial.println("ESP Initialized.");
}

void loop() {
  Serial.print("hello world\n");
  delay(1000);
}
