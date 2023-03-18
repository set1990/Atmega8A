/*
 * serwo.h
 *
 *  Created on: 10 sie 2022
 *      Author: Borsuk
 *
 *	!!!IMPROTANT!!
 *  tics_per_1us 12
 *  tics_per_1ms (tics_per_1us*1000)
 */

#ifndef SERWO_H_
#define SERWO_H_
#include <stdint.h>

#define MAX_VALUE			1000
#define ENABLE				1
#define DISABLE				0

void Serwo_ON_OFF(uint8_t num, uint8_t state);
void Serwo_init(void);
void Serwo_set_value(uint8_t num, uint16_t value);

#endif /* SERWO_H_ */
