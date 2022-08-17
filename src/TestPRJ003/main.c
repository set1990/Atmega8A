/*
 * main.c
 *
 *  Created on: 9 sie 2022
 *      Author: Borsuk
 */
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define tics_per_1us 12
#define tics_per_1ms (tics_per_1us*1000)

ISR(TIMER0_OVF_vect)
{
	PORTD |= (1<<PD0);
	TCNT1 = 0x00;
	TCCR1B=(1<<CS10);
	sei();
	while((tics_per_1ms*2.5)>TCNT1);
	PORTD ^= (1<<PD0);
	TIMSK=(1<<TOIE0);				// maskowanie przerwań T0
	TCCR0=((1<<CS02)|(1<<CS00)); 	// wybór źródła taktowania, prescaler 1024
	TCNT0=0x15;
	sei();
}

int main(void)
{

DDRD=0xff;
PORTD=0x00;

// konfiguracja timera0 8-bitowego
TIMSK=(1<<TOIE0);				// maskowanie przerwań T0
TCCR0=((1<<CS02)|(1<<CS00)); 	// wybór źródła taktowania, prescaler 1024
TCNT0=0x15;
sei();

	while(1)
	{
		_delay_ms(500);
    }
}
