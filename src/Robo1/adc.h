/*
 * adc.h
 *
 *  Created on: 11 sie 2022
 *      Author: Borsuk
 */

#ifndef ADC_H_
#define ADC_H_

#define ADC_MAX		7

uint16_t ADC_value[ADC_MAX];
uint8_t ADC_fresh[ADC_MAX];

void ADC_Handle(void);

#endif /* ADC_H_ */
