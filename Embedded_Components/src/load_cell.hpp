#include "HX711.h"
#include <Arduino.h>

// HX711 circuit wiring
const int LOADCELL_DOUT_PIN = 4;
const int LOADCELL_SCK_PIN = 5;

extern HX711 scale;

#pragma once
void initLoadCell();

long loadCellRead();

