//Include Libraries
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#define analog_ip_VRx  A0//X axis input
#define analog_ip_VRy  A1 //Y axis input 
#define Pot  A2 //50k
#define Mode 4 //pin 4
#define ModeLED 2 //pin 2

//create an RF24 object
RF24 radio(9, 8); //CE, CSN

//adrdress through which two modules communicate
const byte address[6] = {"1Node"};
int currentMillis = 0;
int prevMillis = 0;
const int txIntervalMillis = 100;
bool rslt = false;
byte final_package [32] = {};
byte inputVal_VRx = 0; //variable to store analog input of X axis
byte inputVal_VRy = 0; //variable to store analog input of Y axis
byte inputPot = 0;
byte inputMode = 0; 
void setup() {
  // put your setup code here, to run once:
  radio.begin();
  while (!Serial);
  Serial.begin(9600);
  //Set module as transmitter
  radio.stopListening();

  //Set the Address
  radio.openWritingPipe(address);

  pinMode(analog_ip_VRx, INPUT);
  pinMode(analog_ip_VRy, INPUT);
  pinMode(Pot, INPUT);
  pinMode(Mode, INPUT_PULLUP);
  pinMode(ModeLED,OUTPUT);

}

void loop() {
  // put your main code here, to run repeatedly:
  //Send message to recciever
  currentMillis = millis();
  if (currentMillis - prevMillis >= txIntervalMillis)
  {
    inputVal_VRx = map(analogRead(analog_ip_VRx), 0, 1023, 0 , 255); //Analog values of 0 to 1023?
    inputVal_VRy = map(analogRead(analog_ip_VRy), 0, 1023, 0 , 255);
    inputPot = map(analogRead(Pot), 0, 1023, 0 , 255);
    inputMode = digitalRead(Mode);
    digitalWrite(ModeLED,inputMode); 
    final_package[0] = inputVal_VRx;
    final_package[1] = inputVal_VRy;
    final_package[2] = inputPot;
    final_package[3] = inputMode;
    radio.openWritingPipe(address);
    radio.write(final_package, sizeof(final_package));
    //delay(1000);
  } else
  {
    rslt = false;
  }
  
}
