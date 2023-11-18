#ifndef LSM6DS33_H_
#define LSM6DS33_H_

typedef enum
{
	pI_AM_D = 0,
	pOUT_X_L_G,
	pOUT_X_H_G,
	pOUT_Y_L_G,
	pOUT_Y_H_G,
	pOUT_Z_L_G,
	pOUT_Z_H_G,
	pOUT_X_L_A,
	pOUT_X_H_A,
	pOUT_Y_L_A,
	pOUT_Y_H_A,
	pOUT_Z_L_A,
	pOUT_Z_H_A,
	pNEW_VAL_D,
} LSM6DS33_value;

extern char LSM6DS33_date[14];

void LSM6DS33_Handle(void);
void LSM6DS33_Triger(void);

#endif /* LSM6DS33_H_ */
