/*
 * LIS3MDL.h
 *
 *  Created on: 24 sty 2023
 *      Author: Borsuk
 */

#ifndef LIS3MDL_H_
#define LIS3MDL_H_

typedef enum
{
	pI_AM = 0,
	pOUT_X_L,
	pOUT_X_H,
	pOUT_Y_L,
	pOUT_Y_H,
	pOUT_Z_L,
	pOUT_Z_H,
	pNEW_VAL,
} LIS3MDL_position;

extern char LIS3MDL_date[8];

void LIS3MDL_Handle(void);
void LIS3MDL_Triger(void);

#endif /* LIS3MDL_H_ */
