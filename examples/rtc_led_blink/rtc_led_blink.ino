#include <DS1307Emulator.h>

/*
 * DS1307 Emulator blink
 * 
 * Created on 19 April 2017 
 * by Enrico Sanino
 * 
 * This example code is part of the public domain
 */


void setup()
{
   
  DS1307Emulator.init(13);
  DS1307Emulator.bufferUserData();
  DS1307Emulator.writeToRTC(0x00); // set address 0
  DS1307Emulator.writeToRTC(0x00); // start the RTC issuing the clock run command at address 0
  DS1307Emulator.setUserData();
  DS1307Emulator.bufferUserData();
  DS1307Emulator.writeToRTC(0x07); // set address 7, the conf register
  DS1307Emulator.writeToRTC(0x10); // set output pin to toggle at 1Hz
  DS1307Emulator.setUserData();
}

void loop()
{

  
}
