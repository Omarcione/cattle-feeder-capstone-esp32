#include <stdint.h>

typedef struct {
  uint8_t device_id;   
  uint64_t rfid_id;         // -1 when none/new not present, else the ID read from the RFID reader
  float co2_ppm;         // CO2 ppm
  float ch4_ppm;         // CH4 ppm
  float weight_g;        // weight in grams (or your chosen unit)
  time_t time_ms;
} TelemetryData_t;
