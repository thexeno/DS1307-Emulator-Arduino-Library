# DS1307 Emulator Arduino Library V1.0
*Licensing information is attached on the header of each file.*

The DS1307 Emulator is, as stated by its name, a mere emulation of the omonym chip. This library lets your Arduino behave like such a chip, without actually having a DS1307 around. 

## Compatibility
Since it is really keeping the time, this library is pre-tuned (and since it is open source can be retuned) to work with a quartz crystal of the system clock, running at 16MHz. For this reason can run on any Arduino that uses an external crystal at 16 MHz. The microcontroller supported is the one from the Atmega family (like Arduino Uno, Leonardo, Nano, Micro, Mini and other Atmega derived at 16MHz crystal, like Arduino PRO).

It requires to have installed the [HardWire](http://www.arduinolibraries.info/libraries/hard-wire) library.

### Resources
When initialized with the I2C bus usage, this library occupy the:
 - Timer1
 - I2C bus/pins
 - RTC pin
 - Any possible resource used by the Wire library, since it is used to communicate through the I2C bus
 
Disconnecting the RTC logic from the bus, allow to free up the I2C resoources, like SDA and SCL pins, or use the I2C bus for other purposes or role in the bus (i.e. like using the Arduino running the emulator as I2C master for other purposes). It can subsequently reconnected to the RTC logic (see *Initialization* below).

## How to use it
To use the library it is needed at least to call the initialization. It could run with just a single line of code.

### Initialization

  - **DS1307Emulator.init(*pin number*)** -> initialize the whole emulator, wiping all the non-volatile data and the timekeeping registers. Initialize also the I2C bus. *This step is the minimum required to get the library up and running, therefore is the only function really needed.*
  - **DS1307Emulator.busDisconnect()** -> detach the RTC from the I2C bus without alterating the current RTC functionality. Useful if the the bus shall be used/shared with other tasks on the same sketch. After this call, the emulator will work only inside the sketch and you can do whateve you want with the I2C bus, which will be compatible with the [Wire](https://www.arduino.cc/en/Reference/Wire) lib.
  - **DS1307Emulator.busConnect()** -> attach the RTC to the I2C bus without alterating the RTC functionality.
  
Why the *pin number*? The DS1307 has an output pin, called pin number in the APIs. Let's say we have an Arduino Uno and we want to use the default LED in the port 13. The sketcher shall import the library and call the init in the *setup()*, with the pin number 13. Then, if the emulator is connected to the I2C bus, any master can interact with the Arduino. The only thing required is to call the *init(pin number)* first.

## Some use cases and the remaining functions of the library explained

It is assumed that the RTC is initialized and connected to the I2C bus - i.e. standard initialization.

**Use case: Physical master interfaced with the board running the emulator**

  1. Be sure that the emulator software is connected to the bus. If not, issue the DS1307Emulator.busConnect().
  2. Use a master (very nice would be a second Arduino running an RTC library, or a Raspberry Pi) to talk with the Arduino running the DS1307 emulator, AKA the slave. If the master have the right RTC software (which could be any already written to talk with the original DS1307), it will issue the commands for which this emulator (slave) will answer and behave accordingly.
  
**Use case: On-board sketch interfacing with the emulator**

Function to used in sequence to *write to the emulator* using the sketch:

  1. **DS1307Emulator.bufferUserData()** -> save the current time to a temporary buffer to avoid time wrapping issues (as the DS1307 chip does).
  2. **DS1307Emulator.writeToRTC(*address*)** -> write the current *address* data. In this step, is the RTC internal address set to address value. 
  3. Issuing again the step 2 will write RTC data from the previously set address - it will auto-increment the internal address from the one set in step 2. Repeat for as many bytes shall be written.
  4. **DS1307Emulator.setUserData()** -> apply the adjusted configuration to the RTC registers and close the writing sequence.
 
Function to used in sequence to *read from the emulator* using the sketch:

  1. **DS1307Emulator.bufferUserData()** -> save the current time to a temporary     buffer to avoid time wrapping issues (as the DS1307 chip does).
  2. **DS1307Emulator.writeToRTC(*address*)** -> write the current address byte. *Note that the step 2 is not mandatory, provided that the address currently set is the right one.*
  3. **(*char*) DS1307Emulator.readUserData()** -> read a char from the current address set in the previous point - it will auto-increment the internal address from the one set in step 2. Repeat for as many bytes shall be read.
  4. There is no need to close a reading sequence.

In other words, the sketch shall follow the same algoritm the bus master would do. For the moment, read the [DS1307 Datasheet](https://datasheets.maximintegrated.com/en/ds/DS1307.pdf) for informations like what data is present at what address, what to write at what address to stop and start the clock and so on.

Enjoy!

  
