#ifndef TxControl_h //Checks if main has already defined header file
#define TxControl_h
#include "Peripherals.h"
#include <Arduino.h>
class TxControl
{
    Peripherals data;

  public:
    void init();
    //NRFL01+ can only send a byte at a time
    // So we have to map values to 0-255 values

    void Read_J();                 // Read from A0 & A1
    void Read_Pots();              // Read from A2 & A3
    byte Get_Pot1();               // Get value from Pot1 (blue)
    //byte Get_Pot2();               // Get value from Pot2 (red)
    byte Get_jX();               // Get value from joystick X dir
    byte Get_jY();                // Get value from joystick Y dir
    void Print_Data();
    Peripherals Get_Data();
};

#endif
