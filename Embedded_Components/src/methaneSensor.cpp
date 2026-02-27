//possible methane sensor code
#include <Arduino.h>
#include "methaneSensor.hpp"

double getPPM(int pin){
    double adcReading = analogRead(pin);
    double v0 = adcReading * 3.3 / 4095; 
    double rs = (5 - v0) * RL_RES / v0;
    double ppm = pow(rs/CLEANAIR_SENSOR_RES, -2.95) * 1000;
    return ppm;
}

void setup(){
    Serial.begin(115200);
}

void loop(){
    getPPM(15);
    delay(5000);
}