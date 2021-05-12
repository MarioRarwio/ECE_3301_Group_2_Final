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
#define trigPin 7
#define echoPin  4
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
int cTime = 0;
int rTime = 0;
int timeRecieved = 0;
const int timeOut = 20000;
byte dataReceived[32];                   // this must match dataToSend
// in the TX
long duration, cm = 100, inches = 100;


//const int TRIG_PIN = 7; // digital pin 7
//const int ECHO_PIN = 4; // digital pin 4
//const unsigned int MAX_DIST = 23200;
//unsigned long t1;
//unsigned long t2;
//unsigned long pulse_width;
//float cm;
//float inches;

void setup()
{
  //while (!Serial);
  Serial.begin(9600);
  radio.begin();
  radio.openReadingPipe(0, address);    //set the address
  radio.startListening();               //Set module as receiver
  Rx.init(dc, serv1, forward, reverse);
  Serial.println("I am receiver");
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}



void loop()
{
  cTime = millis();
  if (cTime - rTime >= 350)
  {
    rTime = millis();
    digitalWrite(trigPin, LOW);
    delay(6);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    pinMode(echoPin, INPUT);
    duration = pulseIn(echoPin, HIGH);
    cm = (duration / 2) / 29.1;
    inches = (duration / 2) / 74;
    Serial.println(inches);
  }
  if (inches <= 3)
  {
    Rx.Reverse();
    
  }
  else if (radio.available())
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
