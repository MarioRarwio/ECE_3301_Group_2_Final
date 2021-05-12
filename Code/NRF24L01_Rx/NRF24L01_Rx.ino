//Include Libraries
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Servo.h>

#define dc      6
#define serv1   5
#define forward 3
#define reverse 2
#define ce      9
#define csn     8

#define LED     10 //

void RcInit(int, int, int, int);
void SetData(byte[]);
void SetMotors(void);
void Act_DC(void);
void Act_Servo(void);
void SetData(void);

//create an RF24 object
RF24 radio(9, 8);  // CE, CSN

//address through which two modules communicate.
const byte address[6] = "00001";
byte final_package[32] = {}; //variable to store analog input of X axis
const int TRIG_PIN = 7; // digital pin 7
const int ECHO_PIN = 4; // digital pin 4
const unsigned int MAX_DIST = 23200;
unsigned long t1;
unsigned long t2;
unsigned long pulse_width;
float cm;
float inches;

//variables used for servo and motor
Servo servo1;
int conX;
int conY;
int pot1;
int mode; // 1= pot used for speed, 0 = joy stick sole controller of speed
int dirf;
int dirr;
int spos;
int Speed;

bool newData = false;
int currentTime = 0;
int timeRecieved = 0;
const int timeOut = 200000;

bool Reverse;
void ReverseFunc(int);

void setup()
{
  while (!Serial);
  Serial.begin(9600);

  radio.begin();
  //set the address
  radio.openReadingPipe(0, address);
  //Set module as receiver
  radio.startListening();
  servo1.attach(serv1);
  pinMode(TRIG_PIN, OUTPUT);
  digitalWrite(TRIG_PIN, LOW);
  pinMode(ECHO_PIN, INPUT);

  Serial.println("Receiver");
}

void loop()
{
  //Read the data if available in buffer
  currentTime = millis();
  if (Reverse)
  {
    ReverseFunc(2); //a two second reverse
    Reverse = false;
  }
  if (radio.available())
  {


    //char text[32] = {0};
    radio.read(final_package, sizeof(final_package));
    newData = true;
    timeRecieved = millis();

    //    Serial.print("X: " + String(final_package[0]));
    //    Serial.print("\tY: " + String(final_package[1]));
    //    Serial.print("\t Pot: " + String(final_package[2]));
    //    Serial.println("\t Mode: " + String(final_package[3]));

    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG_PIN, LOW);

    while ( digitalRead(ECHO_PIN) == 0 );

    t1 = micros();
    while ( digitalRead(ECHO_PIN) == 1);
    t2 = micros();
    pulse_width = t2 - t1;

    cm = pulse_width / 58.0;
    inches = pulse_width / 148.0;
    //Serial.print("Distance " + String(inches) + "\t");
    //delay(60); //this delay is unneeded now
    if (inches < 3.0)
    {
      digitalWrite(LED, HIGH);
      Reverse = true;
    }
    else
    {
      digitalWrite(LED, LOW);
      SetData(final_package);

      Act_DC();
      Act_Servo();
    }
  } else
  {
    newData = false;
  }
  //  if ((currentTime - timeRecieved) > timeOut)
  //  {
  //    Reset();
  //    Serial.println("No Connection");
  //  }
}


void ReverseFunc(int seconds)
{
  byte reverse_command[32] = {};
  reverse_command[0] = 127;
  reverse_command[1] = 0; //neutral x position
  reverse_command[2] = 255; //using this for speed? max speed 255
  reverse_command[3] = 1;   // set to mode one
  for (int i = 0; i < seconds; i++)
  {
    delay(1000); //one second delay
    //SetData(reverse_command);
    Act_DC();
    Act_Servo();
    Serial.println("Reverse...");
  }
}
void Reset()
{
  final_package[0] = 127;
  final_package[1] = 127;
  final_package[2] = 1;
  final_package[3] = 1;
  dirf = 0;
  dirr = 0;
  Speed = 90;
}

//void RcInit(int motorpin, int servopin, int fwd, int bck)
//{
//  conX = 127;
//  conY = 127;
//  pot1 = 1;
//  dcpin = motorpin;
//  dirf = fwd;
//  dirr = bck;
//}

void SetData(byte in[])
{
  conX = in[0];
  conY = in[1];
  pot1 = in[2];
  mode = in[3];
//  Serial.print("X: " + String(in[0]));
//  Serial.print("\tY: " + String(in[1]));
//  Serial.print("\t Pot: " + String(in[2]));
//  Serial.println("\t Mode: " + String(in[3]));
  if (mode == 1)
  {
    //    Serial.println("No Pot");
    SetMotors();
  }
  else
  {
    //    Serial.println("Pot");
    SetMotorsNoPot();
  }
}

void SetMotors()
{
  Speed = 0;
  if ( (final_package[1] > 90) && (final_package[1] < 140))
  {
    Speed = 0;
    dirf = 0;
    dirr = 0;
  }
  else if (final_package[1] <= 120)
  {
    Speed = pot1;
    dirf = 0;
    dirr = 1;
  }
  else if (final_package[1] >= 140)
  {
    Speed = pot1;
    dirf = 1;
    dirr = 0;
  }

  // Servo
  if ((final_package[0] > 110) && (final_package[0] < 140))
  {
    spos = 90;
  }
  else if (final_package[0] <= 110)
  {
    spos = map(final_package[0], 0, 110, 50, 90);
  }
  else
  {
    spos = map(final_package[0], 140, 255, 90, 120);
  }
  Serial.print("X: " + String(final_package[0]));
  Serial.print("\tY: " + String(final_package[1]));
  Serial.print("\t Pot: " + String(final_package[2]));
  Serial.print("\tDistance " + String(inches) + "\t");
  Serial.println("\t Mode: " + String(final_package[3]));
}

void Act_DC()
{
  Serial.print("f: " + String(dirf));
  Serial.print("\tr: " + String(dirr));
  Serial.println("\tSpd: " + String(Speed));
  digitalWrite(forward, dirf);
  digitalWrite(reverse, dirr);
  analogWrite(dc, Speed);
}

void Act_Servo()
{
//  Serial.println("\tSPos: " + String(spos));
  servo1.write(spos);
}

void SetMotorsNoPot()
{
  if ( (final_package[1] > 90) && (final_package[1] < 140))
  {
    Speed = 0;
    dirf = 0;
    dirr = 0;
  }
  else if (final_package[1] <= 120)
  {
    Speed = map(final_package[1], 0, 120, 255, 0);
    dirf = 1;
    dirr = 0;
  }
  else if (final_package[1] >= 140)
  {
    Speed = map(final_package[1], 140, 255, 0, 255);
    dirf = 0;
    dirr = 1;
  }

  // Servo
  if ((final_package[0] > 110) && (final_package[0] < 140))
  {
    spos = 90;
  }
  else if (final_package[0] <= 110)
  {
    spos = map(final_package[0], 0, 110, 50, 90);
  }
  else
  {
    spos = map(final_package[0], 140, 255, 90, 120);
  }
    Serial.print("X: " + String(final_package[0]));
    Serial.print("\tY: " + String(final_package[1]));
    Serial.print("\t Pot: " + String(final_package[2]));
    Serial.print("\tDistance " + String(inches) + "\t");
    Serial.println("\t Mode: " + String(final_package[3]));
}
