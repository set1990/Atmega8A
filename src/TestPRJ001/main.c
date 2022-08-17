/*
 * main.c
 *
 *  Created on: 6 sie 2022
 *      Author: Borsuk
 */


#include <avr/io.h>
#include <util/delay.h>


int main(void)
{
	DDRD |= (1<<PD2);

	while(1)
	{
		PORTD |= (1<<PD2);
		_delay_ms(1.5);
		PORTD ^= (1<<PD2);
		_delay_ms(100);
	}
}
