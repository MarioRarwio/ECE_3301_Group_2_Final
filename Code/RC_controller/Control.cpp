#include "Control.h"
void Control::init()
    {
      pinMode(A0, INPUT);
      pinMode(A1, INPUT);
      pinMode(A2, INPUT);
      pinMode(A3, INPUT);
      data.jX = 127;
      data.jY = 127;
      data.pot1 = 1;
      data.pot2 = 1;
    }
    //NRFL01+ can only send a byte at a time
    // So we have to map values to 0-255 values

    void Control::Read_J()                 // Read from A0 & A1
    {
      data.jY = map(analogRead(A0), 0, 1023, 0, 255);
      data.jX = map(analogRead(A1), 0, 1023, 0, 255);
//      Serial.print("X: " + String(analogRead(A1)));
//      Serial.print("\t" + String(data.jX)+ "\t");
//      Serial.print("Y: " + String(analogRead(A0)));
//      Serial.println("\t" + String(data.jY));
      delay(500);
      
    }

    void Control::Read_Pots()              // Read from A2 & A3
    {
      data.pot1 = map(analogRead(A2), 0, 1023, 255, 0);
      data.pot2 = map(analogRead(A3), 0, 1023, 255, 0);
      Serial.print("Blue: " + String(analogRead(A2)));
      Serial.print("\t" + String(data.pot1)+ "\t");
      Serial.print("Red: " + String(analogRead(A3)));
      Serial.println("\t" + String(data.pot2));
    }

    byte Control::Get_Pot1()               // Get value from Pot1 (blue)
    {
      return data.pot1;
    }
    byte Control::Get_Pot2()               // Get value from Pot2 (red)
    {
      return data.pot2;
    }

    byte Control::Get_jX()                // Get value from joystick X dir
    {
      return data.jX;
    }
    byte Control::Get_jY()                // Get value from joystick Y dir
    {
      return data.jY;
    }
    Peripherals Control::Get_Data()
    {
      return data;
    }
