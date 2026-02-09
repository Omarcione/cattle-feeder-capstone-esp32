#include "rfid.hpp"

// Global variable
uint64_t ID_Number = 0;

static volatile bool rfidNewReading = false;
static uint64_t last_id;

// Notification handler class
class RfidNotify
{
public:
    static void OnError(Rfid134_Error errorCode)
    {
        Serial.println();
        Serial.print("Com Error ");
        Serial.println(errorCode);
    }

    static void OnPacketRead(const Rfid134Reading& reading)
    {
        ID_Number = reading.id;
        rfidNewReading = true;
    }
};

// Create the RFID object
Rfid134<HardwareSerial, RfidNotify> rfid(Serial2);

// Setup function for RFID
void initRFID()
{
    Serial2.begin(9600, SERIAL_8N2, HardwareSerial_Rx, HardwareSerial_Tx);
    rfid.begin();
}

// Loop update function
void updateRFID()
{
    rfid.loop();
}

bool rfidHasNewReading()
{
    return rfidNewReading;
}


int rfidGetReading()
{
    rfidNewReading = false;   // consume the reading
    return last_id;
}

