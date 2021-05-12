#ifndef RxControl_h
#define RxControl_h

#include "Peripherals.h"
#include <Servo.h>

class RxControl
{
  public:
    
    void init(int,int,int,int,bool MODE = 0);
    void Set_Data(byte[]);
    Peripherals Print_Data();
    void Reset();
    byte Get_JX();
    byte Get_JY();
    byte Get_Switch();
    byte Get_Pot1();
   // byte Get_Pot2();
    void Map_Motors();
    void Act_DC();
    void Act_Servo();
    Peripherals Get_Data();
    
  private:
    Peripherals data;
    Servo serv1;
    int dcPin;
    int sPin;
    bool mode;  
    int jSpeed;
    int sSpeed; 
    int forward;
    int reverse; 
    int dirF;
    int dirR;
    const int led = 10;
};

#endif
