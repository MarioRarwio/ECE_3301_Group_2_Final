//Include Libraries
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include "TxControl.h"
#include "Peripherals.h"
#include <Arduino.h>

//Prototypes
void send();

RF24 radio(9, 8);  // CE, CSN             //create an RF24 object
TxControl Tx;                             //Create Control object
const byte address[6] = "00001";          //address through which two modules communicate.
int currentMillis = 0;
int prevMillis = 0;
const int txIntervalMillis = 100;
bool rslt = false;

void setup()
{
  Serial.begin(9600);
  radio.begin();

  //set the address
  radio.openWritingPipe(address);

  //Set module as transmitter
  radio.stopListening();
  //Initialize pins
  Tx.init();
  Serial.println("I am sender");
}
void loop()
{
  currentMillis = millis();
  if (currentMillis - prevMillis >= txIntervalMillis)
  {
//    Serial.println("sending...");
    send();
    rslt = true;
    prevMillis = millis();
  }
  else
  {
    rslt = false;
  }
  if (rslt == true)
  {
//    Serial.println("Data Sent");
  }
}

void send()
{
  byte dataToSend[32] = {0};
  Tx.Read_J();
  Tx.Read_Pots();
  Tx.Read_Switch();
  byte text[32] = {};
  dataToSend[0] = Tx.Get_jX();
  dataToSend[1] = Tx.Get_jY();
  dataToSend[2] = Tx.Get_Pot1();
  //dataToSend[3] = Tx.Get_Pot2();
  dataToSend[3] = Tx.Get_Switch();
  //Send message to Receiver
//  rslt = radio.write(&text, sizeof(text));
  rslt = radio.write(&dataToSend,sizeof(dataToSend));
  Tx.Print_Data();
  
}
