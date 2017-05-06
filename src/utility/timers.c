
/* The MIT License (MIT)
 * Copyright (c) 2015 Enrico Sanino
 * File:   timers.h
 * Author: Enrico Sanino
 *
 * Created on 28 febbraio 2015, 18.42
 */

#include "timers.h"


/* Private variables and prototypes*/

static uint16_t timer1presc = 0; /* parti spento */


static void write16bit(uint16_t input_n, volatile uint8_t* out_h, volatile uint8_t* out_l);
static void read16bit(volatile uint16_t *out_n, uint8_t in_h, uint8_t in_l);


static void write16bit(uint16_t input_n, volatile uint8_t* out_h, volatile uint8_t* out_l)
{
	cli();
	*out_h = (uint8_t)(input_n >> 8);
	*out_l = (uint8_t)(input_n);
	sei();
}

static void read16bit(volatile uint16_t *out_n, uint8_t in_h, uint8_t in_l)
{
	cli();
	*out_n = (uint16_t)in_l;
	*out_n |= ((uint16_t)(in_h))<<8;
	sei();
}





void initTimer1(uint16_t prescaler, _t_edge edge,  _t_timer_mode mode , _t_timer_clock clock, uint8_t timer_int)
{
	//TCCR0A = 0;
	
	//TIMSK0 = 0;
	
	switch (mode)
	{
		case TIMER_MODE_NORMAL: 
		cbi(TCCR1A, WGM10);
		cbi(TCCR1A, WGM11);
		cbi(TCCR1B, WGM12);
		cbi(TCCR1B, WGM13);
		break;
		
		case TIMER_MODE_PWM: /* TBD TOP = OCRA e TOP = ICR */
		/* TOP = 0xFF */
		sbi(TCCR1A, WGM10);
		cbi(TCCR1A, WGM11);
		cbi(TCCR1B, WGM12);
		cbi(TCCR1B, WGM13);
		
		break;
		
		case TIMER_MODE_COMPARE_MATCH:
		cbi(TCCR1A, WGM10);
		cbi(TCCR1A, WGM11);
		sbi(TCCR1B, WGM12);
		cbi(TCCR1B, WGM13);		
		break;
		
		case TIMER_MODE_FAST_PWM:
		/* TOP = 0xff */
		sbi(TCCR1A, WGM10);
		cbi(TCCR1A, WGM11);
		sbi(TCCR1B, WGM12);
		cbi(TCCR1B, WGM13);
		break;
		
		default:
		cbi(TCCR1A, WGM10);
		cbi(TCCR1A, WGM11);
		cbi(TCCR1B, WGM12);
		cbi(TCCR1B, WGM13);
		break;
	}

	
	
	if (timer1presc != 0)
	{
		TCCR1B &= 0xF8; /* Clear CTS0 1 e 2 */
		timer1presc = prescaler; /* e lo usi per riapplicare il prescaler quanto fai lo start */
		switch (prescaler)
		{

			case 1:
			TCCR1B |= 1;
			break;
			
			case 8:
			TCCR1B |= 2;
			break;
			
			case 64:
			TCCR1B |= 3;
			break;
			
			case 256:
			TCCR1B |= 4;
			break;
			
			case 1024:
			TCCR1B |= 5;
			break;
			
			default: /* spento se valore errato */
			TCCR1B |= 0;
			break;
		}
	}
	else
	{
		TCCR1B &= 0xF8; /* Clear CTS0 1 e 2 */
		timer1presc = prescaler; /* e lo usi per riapplicare il prescaler quanto fai lo start */
	}
	
	switch (clock)
	{
		case CLKPIN:
		/* Sovrascrivi prescaler */
		if (edge == TMR_CLK_RISE)
		{
			sbi(TCCR1B, CS02);
			sbi(TCCR1B, CS01);
			cbi(TCCR1B, CS00);
		}
		else /* Fall */
		{
			sbi(TCCR1B, CS02);
			sbi(TCCR1B, CS01);
			sbi(TCCR1B, CS00);
		}
		break;
		
		case CLKOSC:
		/* use prescaler value */
		break;
		
		default:
		/* use prescaler value */
		break;
	}
	
	switch (timer_int)
	{
		case TIMER_INT_ON:
		if (mode == TIMER_MODE_NORMAL)  /* Altrimenti setti interrupt nelle funzioni di compare init o pwm init */
		{
			sbi(TIMSK1, TOIE1);
		}
		break;
		case TIMER_INT_OFF:
		cbi(TIMSK1, TOIE1);
		break;
		default:
		cbi(TIMSK1, TOIE1);
		break;
	}
	
}

/**@brief Chiamabile più volte per inizializzare moduli A e B */
void initCompare1 (uint16_t comp_val, _t_compare_sel comp, uint16_t prescaler, _t_edge edge, _t_timer_clock clock, uint8_t timer_int)
{
	initTimer1(prescaler, edge, TIMER_MODE_COMPARE_MATCH, clock, timer_int);
	switch (comp)
	{
		case COMP_A_TOGGLE:
		sbi(TCCR1A, COM1A0);
		cbi(TCCR1A, COM1A1);
		sbi(DDRB, DDB1);
		write16bit(comp_val, &OCR1AH, &OCR1AL);
		OCR1A = comp_val;
		break;
		
		case COMP_A_CLEAR:
		cbi(TCCR1A, COM1A0);
		sbi(TCCR1A, COM1A1);
		sbi(DDRB, DDB1);
		write16bit(comp_val, &OCR1AH, &OCR1AL);
		break;
		
		case COMP_A_SET:
		sbi(TCCR1A, COM1A0);
		sbi(TCCR1A, COM1A1);
		sbi(DDRB, DDB1);
		write16bit(comp_val, &OCR1AH, &OCR1AL);
		break;
		
		case COMP_B_TOGGLE:
		sbi(TCCR1A, COM1B0);
		cbi(TCCR1A, COM1B1);
		sbi(DDRB, DDB2);
		write16bit(comp_val, &OCR1BH, &OCR1BL);
		break;
		
		case COMP_B_CLEAR:
		cbi(TCCR1A, COM1B0);
		sbi(TCCR1A, COM1B1);
		sbi(DDRB, DDB2);
		write16bit(comp_val, &OCR1BH, &OCR1BL);
		break;
		
		case COMP_B_SET:
		sbi(TCCR1A, COM1B0);
		sbi(TCCR1A, COM1B1);
		sbi(DDRB, DDB2);
		write16bit(comp_val, &OCR1BH, &OCR1BL);
		break;
		
		case COMP_A_DISCONNECT:
		cbi(TCCR1A, COM1A0);
		cbi(TCCR1A, COM1A1);
		write16bit(comp_val, &OCR1AH, &OCR1AL);
		break;
		
		case COMP_B_DISCONNECT:
		cbi(TCCR1A, COM1B0);
		cbi(TCCR1A, COM1B1);
		write16bit(comp_val, &OCR1BH, &OCR1BL);
		break;
		
		default:
		/* lascia OCnx sconnesso come lo è da initTimer0*/
		cbi(TCCR1A, COM1B0);
		cbi(TCCR1A, COM1B1);
		cbi(TCCR1A, COM1A0);
		cbi(TCCR1A, COM1A1);
		break;
	}
	
	switch (timer_int)
	{
		case TIMER_INT_ON:
		/* Non devo chiamare questa funzione se uso il normale */
		if ((comp == COMP_A_CLEAR) || (comp == COMP_A_SET) ||(comp == COMP_A_TOGGLE)||(comp == COMP_A_DISCONNECT))
		{
			sbi(TIMSK1, OCIE1A);
		}
		if ((comp == COMP_B_CLEAR) || (comp == COMP_B_SET) ||(comp == COMP_B_TOGGLE)||(comp == COMP_B_DISCONNECT))
		{
			sbi(TIMSK1, OCIE1B);
		}
		break;
		
		case TIMER_INT_OFF:
		if ((comp == COMP_A_CLEAR) || (comp == COMP_A_SET) ||(comp == COMP_A_TOGGLE)||(comp == COMP_A_DISCONNECT))
		{
			cbi(TIMSK1, OCIE1A);
		}
		if ((comp == COMP_B_CLEAR) || (comp == COMP_B_SET) ||(comp == COMP_B_TOGGLE)||(comp == COMP_B_DISCONNECT))
		{
			cbi(TIMSK1, OCIE1B);
		}
		break;
		
		default:
		cbi(TIMSK1, OCIE1A);
		cbi(TIMSK1, OCIE1B);
		break;
	}
	
}

void writeTimer1(uint16_t val)
{
	write16bit(val, &TCNT1H, &TCNT1L);
}

void startTimer1()
{
	TCCR1B &= 0xF8; /* Clear CTS0 1 e 2 */
	switch (timer1presc)
	{
		case 1:
		TCCR1B |= 1;
		break;
		
		case 8:
		TCCR1B |= 2;
		break;
		
		case 64:
		TCCR1B |= 3;
		break;
		
		case 256:
		TCCR1B |= 4;
		break;
		
		case 1024:
		TCCR1B |= 5;
		break;
		
		default: /* spento se valore errato */
		TCCR1B |= 0;
		break;
	}
}

void stopTimer1()
{
	TCCR1B &= 0xF8; /* Clear CTS0 1 e 2 */
}