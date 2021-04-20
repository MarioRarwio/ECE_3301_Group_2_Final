
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include "Control.h"
#include "Peripherals.h"

#define Pot1 A0
#define Pot2 A1
#define jStickX A2
#define jStickY A3
        

RF24 radio(9,8);               //nRF24L01 (CE,CSN)
Control Tx;                       //Create var for controller
const byte address[6] = "00001";  //Address

void setup() 
{
  Serial.begin(9600);
  Tx.init();
  radio.begin();                  
  radio.openWritingPipe(address);
  radio.setAutoAck(false);
  //radio.stopListening();
  radio.setDataRate(RF24_250KBPS);
  radio.setPALevel(RF24_PA_LOW);
  Serial.println("Size: " + String(sizeof(Tx.Get_Data())));
}

void loop() 
{
  //Read analog inputs
  Tx.Read_J();
  Tx.Read_Pots();
  radio.write(&Tx.Get_Data(), sizeof(Tx.Get_Data()));  
}
