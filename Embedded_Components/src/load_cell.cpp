#include "load_cell.hpp"
#include <HX711_ADC.h>
#include <EEPROM.h>

// ------------------- hardware pins (fixed) -------------------
static const int LOADCELL_DOUT_PIN = 4;
static const int LOADCELL_SCK_PIN  = 5;

// ------------------- EEPROM config -------------------
static const int EEPROM_SIZE = 512;
static const int CAL_EEPROM_ADDR = 0;

// ------------------- module state -------------------
static HX711_ADC load_cell(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
static bool s_new_data_ready = false;
static uint32_t s_last_ms = 0;

static const uint32_t STABILIZING_MS = 2000;
static const bool TARE_ON_BOOT = true;
static const uint32_t READ_INTERVAL_MS = 100;

static bool valid_cal(float f) {
  return isfinite(f) && (f != 0.0f);
}

bool loadcell_init() {
  // EEPROM must be begun once on ESP32
  if (!EEPROM.begin(EEPROM_SIZE)) {
    Serial.println("EEPROM.begin failed!");
    return false;
  }

  load_cell.begin();
  load_cell.start(STABILIZING_MS, TARE_ON_BOOT);

  if (load_cell.getTareTimeoutFlag() || load_cell.getSignalTimeoutFlag()) {
    Serial.println("HX711 timeout: check wiring/pins");
    return false;
  }

  float cal = 1.0f;
  EEPROM.get(CAL_EEPROM_ADDR, cal);

  if (!valid_cal(cal)) {
    cal = 1.0f;
    Serial.println("No valid cal factor in EEPROM; using 1.0");
  } else {
    Serial.print("Loaded cal factor from EEPROM: ");
    Serial.println(cal, 6);
  }

  load_cell.setCalFactor(cal);

  // ensure dataset is ready
  while (!load_cell.update()) delay(1);

  s_new_data_ready = false;
  s_last_ms = millis();
  return true;
}

bool loadcell_read(float &out_value, uint32_t now_ms) {
  if (load_cell.update()) s_new_data_ready = true;

  if (s_new_data_ready && (now_ms - s_last_ms >= READ_INTERVAL_MS)) {
    out_value = load_cell.getData();
    s_new_data_ready = false;
    s_last_ms = now_ms;
    Serial.print("Load cell: ");
    Serial.println(out_value, 3);
    return true;
  }
  return false;
}

bool loadcell_calibrate(Stream &io) {
  io.println("***");
  io.println("Start calibration:");
  io.println("Remove any load.");
  io.println("Send 't' to tare.");

  bool resume = false;
  while (!resume) {
    load_cell.update();

    if (io.available() > 0) {
      char inByte = (char)io.read();
      if (inByte == 't') load_cell.tareNoDelay();
    }

    if (load_cell.getTareStatus() == true) {
      io.println("Tare complete");
      resume = true;
    }
    delay(1);
  }

  io.println("Place known mass, then type its weight (e.g. 100.0) and press enter:");

  float known_mass = 0.0f;
  resume = false;
  while (!resume) {
    load_cell.update();
    if (io.available() > 0) {
      known_mass = io.parseFloat();
      if (known_mass != 0.0f) {
        io.print("Known mass: ");
        io.println(known_mass, 3);
        resume = true;
      }
    }
    delay(1);
  }

  load_cell.refreshDataSet();
  float newCal = load_cell.getNewCalibration(known_mass);

  io.print("New calibration value: ");
  io.println(newCal, 6);

  if (!valid_cal(newCal)) {
    io.println("Calibration failed (invalid newCal). Ensure readings change with weight.");
    return false;
  }

  io.print("Save to EEPROM address ");
  io.print(CAL_EEPROM_ADDR);
  io.println("? y/n");

  resume = false;
  while (!resume) {
    if (io.available() > 0) {
      char inByte = (char)io.read();
      if (inByte == 'y') {
        EEPROM.put(CAL_EEPROM_ADDR, newCal);
        EEPROM.commit();
        io.println("Saved.");
        resume = true;
      } else if (inByte == 'n') {
        io.println("Not saved.");
        resume = true;
      }
    }
    delay(1);
  }

  // apply immediately either way
  load_cell.setCalFactor(newCal);

  io.println("Calibration done.");
  io.println("***");
  return true;
}
