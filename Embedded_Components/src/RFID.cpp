#include "rfid.hpp"

// Global variable
uint64_t ID_Number = 0;

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
        char buffer[64];

        Serial.println("TAG INFO:");

        sprintf(buffer, "\tCountry: %03u", reading.country);
        Serial.println(buffer);

        if (reading.isData)   Serial.println("\tTag type: data");
        if (reading.isAnimal) Serial.println("\tTag type: animal");

        ID_Number = reading.id;
        sprintf(buffer, "\tID: %llu", ID_Number);
        Serial.println(buffer);
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
