//Include Libraries
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include "Peripherals.h"
#include "RxControl.h"

//Pins
#define dc      6         //PWM
#define serv1   5         //PWM
#define forward 3         //Digital
#define reverse 2         //Digital
#define CE      9
#define CSN     8
#define TRIG_PIN 7
#define ECHO_PIN  4
//Prototypes
void GetData();
void ShowData();
//create an RF24 object
RF24 radio(CE, CSN);                    // CE, CSN

const byte address[6] = "00001";        //address through which
//two modules communicate.
RxControl Rx;                           //Receiver Object
bool newData = false;
int currentTime = 0;
int timeRecieved = 0;
const int timeOut = 20000;
byte dataReceived[32];                   // this must match dataToSend
// in the TX

//const int TRIG_PIN = 7; // digital pin 7
//const int ECHO_PIN = 4; // digital pin 4
const unsigned int MAX_DIST = 23200;
unsigned long t1;
unsigned long t2;
unsigned long pulse_width;
float cm;
float inches;

void setup()
{
  //while (!Serial);
  Serial.begin(9600);
  radio.begin();
  radio.openReadingPipe(0, address);    //set the address
  radio.startListening();               //Set module as receiver
  Rx.init(dc, serv1, forward, reverse);
  Serial.println("I am receiver");
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
}



void loop()
{
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  while ( digitalRead(ECHO_PIN) == 0 )
  {
    Serial.println("Echo...0");
  }

  t1 = micros();
  while ( digitalRead(ECHO_PIN) == 1)
  {
    Serial.println("Echo....1");
  }
  t2 = micros();
  pulse_width = t2 - t1;

  cm = pulse_width / 58.0;
  inches = pulse_width / 148.0;

  Serial.print("Distance " + String(inches) + "\t");
  //delay(60); //this delay is unneeded now
  //  Read the data if available in buffer
  if (radio.available())
  {
    //    byte text[32] = {0};
    //    radio.read(&text, sizeof(text));
    //    Serial.println(text[1]);
    GetData();
    Rx.Act_Servo();
    Rx.Act_DC();
  }
}

void GetData()
{
  currentTime = millis();
  if ( radio.available() ) {
    radio.read( &dataReceived, sizeof(dataReceived) );
    newData = true;
    timeRecieved = millis();
    Rx.Set_Data(dataReceived);
    ShowData();
  }
  else
  {
    newData = false;
  }
  if ((currentTime - timeRecieved) > timeOut)
  {
    Rx.Reset();
    Serial.println("No Connection");
  }
}
void ShowData()
{
  //        Serial.print("Data received: ");
  Rx.Print_Data();
  newData = false;
}
