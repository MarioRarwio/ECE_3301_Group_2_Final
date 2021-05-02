#ifndef Peripherals_h //Checks if main has already defined header file
#define Peripherals_h
#include <Arduino.h>

struct Peripherals
{
  byte jX;            // joystick X val
  byte jY;            // joystick Y val
  byte pot1;          // Pot 1 (blue)
  //byte pot2;          // Pot 2 (red)

};
#endif
