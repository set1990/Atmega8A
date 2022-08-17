/*
 * serwo.c
 *
 *  Created on: 10 sie 2022
 *      Author: Borsuk
 *
 *	!!!IMPROTANT!!
 *  tics_per_1us 12
 *  tics_per_1ms (tics_per_1us*1000)
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#include "serwo.h"

#define MIN_REAL_VALUE		9600
#define MULTI				17 //24
#define SERWO_QUANTITY		0x02
#define SET_PINS(PINS) 		PORTD |= (PINS)
#define CLEAN_PIN(PIN) 		PORTD &= (~(1<<PIN))
#define CLAEN_PORT_VALUE 	(PORTD == 0x00)
#define SET_TIMER1()		TCNT1 = 0x00;\
							TCCR1B = (1<<CS10);\
							sei()
#define SET_TIMER0()	 	TIMSK = (1<<TOIE0);\
							TCCR0 = ((1<<CS02)|(1<<CS00));\
							TCNT0=0x16;\
							sei()

uint16_t serwo_value[SERWO_QUANTITY] = {0x00};
uint8_t serwo_state=DISABLE;

void Serwo_init()
{
	DDRD = 0xff;
	PORTD = 0x00;
	SET_TIMER0();
}

void Serwo_ON_OFF(uint8_t state)
{
	serwo_state = state;
}

void Serwo_set_value(uint8_t num, uint16_t value)
{
    if(value>MAX_VALUE) value = MAX_VALUE;
	serwo_value[num] = MIN_REAL_VALUE + (value*MULTI);
}

ISR(TIMER0_OVF_vect)
{
	if(ENABLE==serwo_state)
	{
		SET_TIMER0();
		SET_PINS((1<<PD1)|(1<<PD0));
		SET_TIMER1();
		while(1)
		{
			if((serwo_value[PD0])<TCNT1)
			{
				CLEAN_PIN(PD0);
			}
			if((serwo_value[PD1])<TCNT1)
			{
				CLEAN_PIN(PD1);
			}
			if(CLAEN_PORT_VALUE) break;
		}
	}

}
