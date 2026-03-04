#include "rfid.hpp"

// Global variable
volatile uint64_t ID_Number = 0;
static volatile bool rfidNewReading = false;

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
        Serial.print("RFID read: ");
        Serial.println(ID_Number);
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


uint64_t rfidGetReading()
{
    rfidNewReading = false;   // consume the reading
    return ID_Number;
}

