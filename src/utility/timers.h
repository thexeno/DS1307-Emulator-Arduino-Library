/* The MIT License (MIT)
 * Copyright (c) 2015 Enrico Sanino
 * File:   timers.h
 * Author: Enrico Sanino
 *
 * Created on 28 febbraio 2015, 18.42
 */

#ifndef TIMERS_H
#define	TIMERS_H

#include "avr/io.h"
#include "avr/interrupt.h"

#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))

typedef enum {CLKPIN, CLKOSC, CLKINST} _t_timer_clock;
enum {TIMER_INT_OFF, TIMER_INT_ON};

typedef enum {TMR_CLK_RISE, TMR_CLK_FALL} _t_edge;
typedef enum
{
	COMP_A_TOGGLE,
	COMP_A_CLEAR,
	COMP_A_SET,
	COMP_B_TOGGLE,
	COMP_B_CLEAR,
	COMP_B_SET,
	COMP_A_DISCONNECT,
	COMP_B_DISCONNECT
}	_t_compare_sel;
	

typedef enum 
{
	TIMER_MODE_NORMAL,
	TIMER_MODE_PWM,
	TIMER_MODE_COMPARE_MATCH,
	TIMER_MODE_FAST_PWM
} _t_timer_mode;




void initTimer1(uint16_t prescaler, _t_edge edge,  _t_timer_mode mode , _t_timer_clock clock, uint8_t timer_int );
void startTimer1();
void stopTimer1();

void initCompare1 (uint16_t comp_val, _t_compare_sel comp, uint16_t prescaler, _t_edge edge,   _t_timer_clock clock, uint8_t timer_int);
void writeTimer1(uint16_t value);



#endif	/* TIMERS_H */

