#include "RxControl.h"
#include "Arduino.h"



void RxControl::init(int dcMotorPin, int ServPin,
                     int fwd, int rvs, bool MODE = 0)
{
  data.jX = 127;
  data.jY = 127;
  data.pot1 = 1;
  //data.pot2 = 1;
  mode = MODE;
  dcPin = dcMotorPin;
  sPin = ServPin;
  forward = fwd;
  reverse = rvs;
  serv1.attach(sPin);
  pinMode(dcPin, OUTPUT);
  pinMode(forward, OUTPUT);
  pinMode(reverse, OUTPUT);
}
void RxControl::Set_Data(byte inData[])
{
  data.jX = inData[0];
  data.jY = inData[1];
  data.pot1 = inData[2];
  data.mode = inData[3];
  //data.pot2 = inData[3];
  Map_Motors();
}
void RxControl::Reset()
{
  data.jX = 127;
  data.jY = 127;
  data.pot1 = 1;
  //data.pot2 = 1;
  dirF = 0;
  dirR = 0;
  sSpeed = 90;
  mode = 0;
}
Peripherals RxControl::Get_Data()
{
  return data;
}
byte RxControl::Get_JX()
{
  return data.jX;
}
byte RxControl::Get_JY()
{
  return data.jY;
}
byte RxControl::Get_Pot1()
{
  return data.pot1;
}
//byte RxControl::Get_Pot2()
//{
//  return data.pot2;
//}
byte RxControl::Get_Switch()
{
  return data.mode;
}
Peripherals RxControl::Print_Data()
{
  //Print data to the Serial Monitor
  Serial.print("jX: ");
  Serial.print(data.jX);
  Serial.print("\tjY: " );
  Serial.print(data.jY);
  Serial.print("\tBlue Pot: ");
  Serial.println(data.pot1);
  Serial.println("Switch: " + String(data.mode));
  //  Serial.print("\tRed Pot: ");
  //  Serial.println(data.pot2);
}
void RxControl::Map_Motors()
{
  if (mode == 0)
  {
    //DC Motor
    //Joystick at center, accounts for dead zone
    if ( (data.jY > 90) && (data.jY < 140))
    {
      jSpeed = 0;
      dirF = 0;
      dirR = 0;
    }
    //If joystick is pushed down
    else if (data.jY <= 120)
    {
      jSpeed = map(data.jY, 0, 90, 255, 0);
      dirF = 0;
      dirR = 1;
    }
    //If joystick is pushed up
    else if (data.jY >= 140)
    {
      jSpeed = map(data.jY, 140, 255, 0, 255);
      dirF = 1;
      dirR = 0;
    }

    //Servo Motor
    if ( (data.jX > 110) && (data.jX < 140))
    {
      sSpeed = 90;
    }
    else if (data.jX <= 110)
    {
      sSpeed = map(data.jX, 0, 110, 70, 90);
      //sSpeed = 90;
    }
    else
    {
      sSpeed = (data.jX, 140, 255, 90, 120);
    }

  }
}
void RxControl::Act_DC()
{
  digitalWrite(forward, dirF);
  digitalWrite(reverse, dirR);
  analogWrite(dcPin, jSpeed);
//  Serial.println("Speed = " + String(jSpeed));
  if (dirF == 1)
  {
    digitalWrite(led,HIGH);
  }
  else {digitalWrite(led,LOW);}

}
void RxControl::Act_Servo()
{
  serv1.write(sSpeed);
//  Serial.println("Servo = " + String(sSpeed));
//  Serial.println("Directions:\tFwd = " + String(dirF) + "\tRvs = " + String(dirR));
}
void RxControl::Reverse()
{
  Serial.println("Reverse!!!");
  dirR = 0;
  dirF = 1;
  jSpeed = 175;
  Act_Servo();
  Act_DC();
  delay(1000);
}
