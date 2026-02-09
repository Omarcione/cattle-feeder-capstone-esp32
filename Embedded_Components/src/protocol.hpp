#include <stdint.h>

typedef struct {
  uint8_t device_id;   
  uint32_t seq;             // monotonically increasing message counter
  uint64_t ts_ms;           // timestamp in milliseconds
  int32_t rfid_id;         // -1 when none/new not present, else the ID read from the RFID reader
  int32_t co2_ppm;         // CO2 ppm
  float weight_g;        // weight in grams (or your chosen unit)
} TelemetryData_t;
