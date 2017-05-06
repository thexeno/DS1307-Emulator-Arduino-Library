//The MIT License (MIT)
//
//Copyright (c) 2016 Enrico Sanino
//
//Permission is hereby granted, free of charge, to any person obtaining a copy
//of this software and associated documentation files (the "Software"), to deal
//in the Software without restriction, including without limitation the rights
//to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//copies of the Software, and to permit persons to whom the Software is
//furnished to do so, subject to the following conditions:
//
//The above copyright notice and this permission notice shall be included in all
//copies or substantial portions of the Software.
//
//THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
//SOFTWARE.




#ifndef RTC_PROTOCOL_H_
#define RTC_PROTOCOL_H_

extern "C"
{
	#include "utility/rtc_hal.h"
	#include <string.h>
	#include "avr/interrupt.h"
	#include "avr/io.h"
}


#include <inttypes.h>


#define DS1307_DEVICE_ADDRESS 0x68 /* of DS1307 protocol */






class DS1307emulator
{
	public:
	DS1307emulator();
	void init(uint8_t);
	void softInit(uint8_t);
	static void    tickIncrementISR(void);
	static uint8_t readUserData(void);
	static uint8_t writeToRTC(unsigned char data);
	static void    bufferUserData(void);
	static void    setUserData(void);
	static void    busConnect(void);
	static void    busDisconnect(void);


	private:

	static uint8_t getFromRTC(unsigned char *data);
	static void putYear(uint8_t data);
	static void putMonth(uint8_t data);
	static void putMday(uint8_t data);
	static void putWday(uint8_t data);
	static void putHour(uint8_t data);
	static void putMinute(uint8_t data);
	static void putSecond(uint8_t data);
	static void putNvRam(uint8_t data, uint8_t w_addr);
	static void putControlHandler(uint8_t data);
	static void getControlHandler(uint8_t* val);
	static void getYear(uint8_t* data);
	static void getMonth(uint8_t *data);
	static void getMday(uint8_t *data);
	static void getWday(uint8_t *data);
	static void getHour(uint8_t *data);
	static void getMinute(uint8_t *data);
	static void getSecond(uint8_t *data);
	static void getNvRam(uint8_t *data, uint8_t w_addr);
};

extern DS1307emulator DS1307Emulator;

#endif /* RTC_PROTOCOL_H_ */
