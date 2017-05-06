/* DS1307 RTC Emulator bare minimum setup
*
*  This is a basic setup for the RTC emulator, with the pin 13 used as eventual clock tick output. 
*  It will be initialized the RTC core in default tick halted mode, waiting for commands from an I2C master
*   
*   Created 19 April 2016
*   by Enrico Sanino
*   
*   This example code is in the public domain.
*/
    
#include <DS1307Emulator.h>
 
void setup()
{
  DS1307Emulator.init(13);
}
 
void loop()
{
 /* Put your sketch code here, it will run in parallel with the RTC emulator */
}
