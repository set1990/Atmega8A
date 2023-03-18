/*
 * steps.c
 *
 *  Created on: 12 sie 2022
 *      Author: Borsuk
 */
#include "steps.h"
#include "serwo.h"
#include "adc.h"
#include "eth.h"
#define PLUS20P(v)		(v+((v*2)/10))
#define MINUS20P(v)		(v-((v*2)/10))
#define PAUZE_I			100

uint8_t step_end = 0;
uint8_t step_current = 0;
uint8_t step_run_flag = PAUSE_STEPS;
steps_q steps_program[MAX_STEPS] = {0};

void Steps_run()
{
	step_run_flag=RUN_STEPS;
}

void Steps_pause()
{
	step_run_flag=PAUSE_STEPS;
}

void Steps_add(uint8_t num, uint16_t value)
{
	if(step_end<MAX_STEPS)
	{
		steps_program[step_end].num   = num;
		steps_program[step_end].value = value;
		step_end++;
	}
}

void Steps_clear()
{
	step_end = 0;
	step_current = 0;
	step_run_flag = PAUSE_STEPS;
}

const unsigned char* Steps_list()
{
	return (unsigned char*)(steps_program);
}

char step_over(uint16_t value)
{
	static char special_step_count = 0;
	static char special_step_flag = 0;
	char falg = 0;
	if(steps_program[step_current].num < 5)
	{
		value = ((value*7)/10) + 175;
		if((ADC_value[steps_program[step_current].num]>=MINUS20P(value))
			&&(ADC_value[steps_program[step_current].num]<=PLUS20P(value)))
				falg=1;
	}
	else
	{
		if (special_step_flag==0)
		{
			special_step_flag = 1;
			special_step_count = 0;
		}
		else
		{
			if(special_step_count >= 250)
			{
				falg = 1;
				special_step_flag = 0;
			}
		}
		special_step_count++;
	}
	return falg;
}

void Steps_Handle()
{
	static uint16_t ADC_prev;
	static uint16_t pauza=0;
	static uint8_t oneflag=0;
	if(PAUSE_STEPS==step_run_flag) return;
	if(oneflag==0)
	{
		Serwo_set_value(steps_program[step_current].num, steps_program[step_current].value);
		oneflag=1;
	}
	if(pauza<PAUZE_I)
	{
		pauza++;
		return;
	}
	pauza = 0;
	if(ADC_fresh[steps_program[step_current].num]==0)return;
	ADC_fresh[steps_program[step_current].num]=0;
	if((ADC_prev/10)==(ADC_value[steps_program[step_current].num]/10))
	{
		if(step_over(steps_program[step_current].value)) //Warunek na zmianę kroku na następny
		{
			if(step_current==step_end)
			{
				step_run_flag=PAUSE_STEPS;
				step_current=0;
			}
			else
			{
				step_current++;
				oneflag=0;
			}
		}
	}
	ADC_prev = (ADC_value[steps_program[step_current].num]);
}
