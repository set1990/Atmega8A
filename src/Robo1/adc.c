/*
 * adc.c
 *
 *  Created on: 11 sie 2022
 *      Author: Borsuk
 */
#include <avr/io.h>
#include <stdint.h>
#include "adc.h"

typedef struct{
	char run:1;
	char finish:1;
	char adc:3;
}conversion;

void ADC_Handle()
{
	static conversion state = {0};
 	if(!state.run)
 	{
 		ADCSRA  = (1<<ADEN)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);
 	 	if(state.adc==ADC_VOID) ADMUX = (1<<REFS0)|(ADC_NEXT);
 	 	else ADMUX = (1<<REFS0)|(state.adc);
 	 	ADCSRA |= (1<<ADSC);	// Start conversion
 	 	state.run = 1;
 	}
 	else
 	{
 		if(!(ADCSRA&(1<<ADSC)))
 	 	{
 			ADC_value[state.adc] = ADCW;
 			ADC_fresh[state.adc] = 1;
 	 		state.run=0;
 	 		state.adc++;
 	 		if(state.adc>=ADC_MAX) state.adc=0;
 	 	}
 	}
 }
