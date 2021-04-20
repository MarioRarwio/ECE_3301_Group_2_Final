#include <nRF24L01.h>
//#include <printf.h>
#include <RF24.h>
#include <RF24_config.h>
#include "TxControl.h"
#include "Peripherals.h"

// SimpleTx - the master or the transmitter
#define CE_PIN   9
#define CSN_PIN 10

void send();
void updateMessage();

const byte slaveAddress[5] = {'R','x','A','A','A'};
char attempt = 1;
RF24 radio(CE_PIN, CSN_PIN); // Create a Radio
byte dataToSend[4]; // this must match dataReceived in the RX
unsigned long currentMillis;
unsigned long prevMillis;
unsigned long txIntervalMillis = 10; // send 10 per second
TxControl Tx;

void setup() {

    Serial.begin(9600);
    Serial.println("SimpleTx Starting");
    radio.begin();
    radio.setDataRate( RF24_250KBPS );
    radio.setRetries(3,5); // delay, count
    radio.openWritingPipe(slaveAddress);
    Tx.init();
}

//====================

void loop() {
    currentMillis = millis();
    if (currentMillis - prevMillis >= txIntervalMillis) {
        send();
        prevMillis = millis();
    }
}

//====================

void send() {

    bool rslt;
    Tx.Read_J();
    Tx.Read_Pots();
    dataToSend[0] = Tx.Get_jX();
    dataToSend[1] = Tx.Get_jY();
    dataToSend[2] = Tx.Get_Pot1();
    dataToSend[3] = Tx.Get_Pot2();
    rslt = radio.write( &dataToSend, sizeof(dataToSend) );
        // Always use sizeof() as it gives the size as the number of bytes.
        // For example if dataToSend was an int sizeof() would correctly return 2

    Serial.print("Data Sent ");
    
    if (rslt) {
        Serial.print("  Acknowledge received");
        updateMessage();
        attempt = 1;
        
    }
    else {
        Serial.println("  Tx failed");
        attempt++;
    }
}

//================

void updateMessage() {
        // so you can see that new data is being sent
        Serial.println("\tAttempt: " + String(attempt));
        Tx.Print_Data();
}
