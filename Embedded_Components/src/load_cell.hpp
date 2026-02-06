#pragma once
#include <Arduino.h>

bool loadcell_init();
bool loadcell_calibrate(Stream &io);                 // blocking, interactive
bool loadcell_read(float &out_value, uint32_t now_ms); // non-blocking; true when new value ready
