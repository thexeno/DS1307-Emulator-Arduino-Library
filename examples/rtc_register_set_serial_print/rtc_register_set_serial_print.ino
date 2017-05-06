/* DS1307 RTC Emulator functionality test, with serial commands and interaction. 
 *
 * Here is initialized and started the RTC emulation. 
 * With a serial terminal, is possible to send the following commands, followed by a "\n" character (usually already enabled on the Arduino terminal):
 *          "h" to increment the hours
 *          "m" to increment minutes
 *          "D" to increment days
 *          "d" to increment date
 *          "M" to increment months
 *          "y" to increment years
 *          "c"+"0xXX" to enable the writing in the control register with the "0xXX" exadecimal value. This requires to send a number and not a character, 
 *          so a terminal like Termite can do the job.
 *          "1" to reconnect on the I2C bus
 *          "2" to disconnect from the I2C bus
 *          "3" to issue a soft initialization, RTC pin 13
 *          "4" to issue a complete re-initialization, RTC pin 13
 *          "p" to print the RTC values
 * 
 * Created on 19 April 2017 
 * by Enrico Sanino
 * 
 * This example code is part of the public domain
 */


#include <DS1307Emulator.h>


uint8_t dec2bcd(uint8_t data);
uint8_t bcd2dec(uint8_t data);

uint8_t hour_address = 0x02;
uint8_t minute_address = 0x01;
uint8_t second_address = 0x00;

void setup()
{
  // put your setup code here, to run once:
  
  Serial.begin(9600);
  DS1307Emulator.init(13);
  DS1307Emulator.bufferUserData();
  DS1307Emulator.writeToRTC(0x00); // set address 0
  DS1307Emulator.writeToRTC(0x00); // start the RTC issuing the clock run command at address 0
  DS1307Emulator.setUserData();
}

void loop()
{
  char data = 0;
  // To be used with the "\n" in the terminal to work better
  if (Serial.available() >= 2)
  {
    uint8_t temp = 0;
    data = Serial.read();
    if (data == 'h')
    {
      DS1307Emulator.bufferUserData();
      DS1307Emulator.writeToRTC(hour_address);
      temp = DS1307Emulator.readUserData();
      temp = bcd2dec(temp);
      temp++;
      temp = dec2bcd(temp);
      DS1307Emulator.bufferUserData();
      DS1307Emulator.writeToRTC(hour_address);
      DS1307Emulator.writeToRTC(temp);
      DS1307Emulator.setUserData();
    }
    else if (data == 'm')
    {
      DS1307Emulator.bufferUserData();
      DS1307Emulator.writeToRTC(minute_address);
      temp = DS1307Emulator.readUserData();
      temp = bcd2dec(temp);
      temp++;
      temp = dec2bcd(temp);
      DS1307Emulator.bufferUserData();
      DS1307Emulator.writeToRTC(minute_address);
      DS1307Emulator.writeToRTC(temp);
      DS1307Emulator.setUserData();
    }
    else if (data == 'D')
    {
      // DAY
      DS1307Emulator.bufferUserData();
      DS1307Emulator.writeToRTC(3);
      temp = DS1307Emulator.readUserData();
      temp = bcd2dec(temp);
      temp++;
      temp = dec2bcd(temp);
      DS1307Emulator.bufferUserData();
      DS1307Emulator.writeToRTC(3);
      DS1307Emulator.writeToRTC(temp);
      DS1307Emulator.setUserData();
    }
    else if (data == 'd')
    {
      // date
      DS1307Emulator.bufferUserData();
      DS1307Emulator.writeToRTC(4);
      temp = DS1307Emulator.readUserData();
      temp = bcd2dec(temp);
      temp++;
      temp = dec2bcd(temp);
      DS1307Emulator.bufferUserData();
      DS1307Emulator.writeToRTC(4);
      DS1307Emulator.writeToRTC(temp);
      DS1307Emulator.setUserData();
    }
    else if (data == 'M')
    {
      // Month
      DS1307Emulator.bufferUserData();
      DS1307Emulator.writeToRTC(5);
      temp = DS1307Emulator.readUserData();
      temp = bcd2dec(temp);
      temp++;
      temp = dec2bcd(temp);
      DS1307Emulator.bufferUserData();
      DS1307Emulator.writeToRTC(5);
      DS1307Emulator.writeToRTC(temp);
      DS1307Emulator.setUserData();
    }
    else if (data == 'y')
    {
      // guess what
      DS1307Emulator.bufferUserData();
      DS1307Emulator.writeToRTC(6);
      temp = DS1307Emulator.readUserData();
      temp = bcd2dec(temp);
      temp++;
      temp = dec2bcd(temp);
      DS1307Emulator.bufferUserData();
      DS1307Emulator.writeToRTC(6);
      DS1307Emulator.writeToRTC(temp);
      DS1307Emulator.setUserData();
    }
    else if (data == 'c')
    {
      // control register, from terminal issue c+0xXX. So a char and a number representing the control register configuration.
      // Cannot be done from the Arduino terminal (allows only characters), but very easily from others, like Termite terminal
      DS1307Emulator.bufferUserData();
      DS1307Emulator.writeToRTC(7);
      if (Serial.available())
      {
        DS1307Emulator.writeToRTC(Serial.read());
      }
      DS1307Emulator.setUserData();
    }
    else if (data == '1')
    {
      DS1307Emulator.busConnect(); 
    }
    else if (data == '2')
    {
      DS1307Emulator.busDisconnect(); 
    }
    else if (data == '3')
    {
      DS1307Emulator.softInit(13); 
    }
    else if (data == '4')
    {
      DS1307Emulator.init(13); 
    }
    else if (data == 'p')
    {
      DS1307Emulator.bufferUserData();
      DS1307Emulator.writeToRTC(second_address);
      Serial.print("Second: ");
      Serial.print(DS1307Emulator.readUserData(), HEX);
      Serial.println();
      Serial.print("Minute: ");
      Serial.print(DS1307Emulator.readUserData(), HEX);
      Serial.println();
      Serial.print("Hour: ");
      Serial.print(DS1307Emulator.readUserData(), HEX);
      Serial.println();
      Serial.print("Day: ");
      Serial.print(DS1307Emulator.readUserData(), HEX);
      Serial.println();
      Serial.print("Date: ");
      Serial.print(DS1307Emulator.readUserData(), HEX);
      Serial.println();
      Serial.print("Month: ");
      Serial.print(DS1307Emulator.readUserData(), HEX);
      Serial.println();
      Serial.print("Year: ");
      Serial.print(DS1307Emulator.readUserData(), HEX);
      Serial.println();
      Serial.print("Ctrl: ");
      Serial.print(DS1307Emulator.readUserData(), HEX);
      Serial.println();
    }
  }
}

uint8_t dec2bcd(uint8_t data)
{
  return ((data/10 * 16) + (data % 10));
}


uint8_t bcd2dec(uint8_t data)
{
  return ((data/16 * 10) + (data % 16));
}
