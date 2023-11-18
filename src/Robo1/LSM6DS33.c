#include "i2c.h"
#include "LSM6DS33.h"
#include "eth.h"

#define LSM6DS33_ADDRESS	0x6B
#define	WHO_AM_I			0x0F
#define CTRL1_XL			0x10
#define CTRL2_G				0x11
#define CTRL3_C				0x12
#define OUT_X_L_G			0x22
#define ODR_XL1				0x05
#define ODR_XL0				0x04
#define FS_XL1				0x03
#define FS_XL0				0x02
#define ODR_G2				0x06
#define FS_G0				0x02
#define FS_G1				0x03
#define BDU					0x06
#define IF_INC				0x02

#define UltraHighPerformance_ACC()\
{\
	Data_out_i2c = CTRL1_XL;\
	I2C_back = Data_Write;\
	I2C_start(LSM6DS33_ADDRESS, WRITE_DETA_I2C);\
	data_to_write = (1<<ODR_XL1)|(1<<ODR_XL0);\
}

#define UltraHighPerformance_G()\
{\
	Data_out_i2c = CTRL2_G;\
	I2C_back = Data_Write;\
	I2C_start(LSM6DS33_ADDRESS, WRITE_DETA_I2C);\
	data_to_write = (1<<ODR_G2)|(1<<FS_G0)|(1<<FS_G1);\
}

#define ACC_G_common()\
{\
	Data_out_i2c = CTRL3_C;\
	I2C_back = Data_Write;\
	I2C_start(LSM6DS33_ADDRESS, WRITE_DETA_I2C);\
	data_to_write = (1<<BDU);\
}


#define Read_new_value(nr)\
{\
	Data_out_i2c = OUT_X_L_G+nr-1;\
	I2C_back = Data_Read_Triger;\
	I2C_start(LSM6DS33_ADDRESS, WRITE_DETA_I2C);\
}

#define Check_WHO_AM_I()\
{\
	Data_out_i2c = WHO_AM_I;\
	I2C_back = Data_Read_Triger;\
	I2C_start(LSM6DS33_ADDRESS, WRITE_DETA_I2C);\
}

typedef enum
{
	InitUHPACC,
	InitUHPG,
	I_AM_REQ,
	I_AM_WAIT,
	I_AM_RESP,
	TRIGER_WAIT,
	OUT_REQ,
	OUT_WAIT,
	OUT_RESP
} LSM6DS33_state;
LSM6DS33_state LSM6DS33_state_machin = InitUHPACC;
char LSM6DS33_data_nr = 0;

char LSM6DS33_date[14] = {0};
static char data_to_write = 0;

static void Data_Read_Triger()
{
	I2C_rep_start(LSM6DS33_ADDRESS, READ_DETA_I2C);
	switch(LSM6DS33_state_machin)
	{
		case I_AM_WAIT:
			LSM6DS33_state_machin = I_AM_RESP;
			break;

		case OUT_WAIT:
			LSM6DS33_state_machin = OUT_RESP;
			break;

		default:
			break;
	}
}

static void Data_Write()
{
	Data_out_i2c = data_to_write;
	I2C_write();
}

void LSM6DS33_Handle()
{
	if(Transmision_Flag_i2c==0)
	{
		switch(LSM6DS33_state_machin)
		{
			case InitUHPACC:
				LSM6DS33_state_machin = InitUHPG;
				UltraHighPerformance_ACC();
				break;

			case InitUHPG:
				LSM6DS33_state_machin = I_AM_REQ;
				UltraHighPerformance_G();
				break;

			case I_AM_REQ:
				LSM6DS33_state_machin = I_AM_WAIT;
				Check_WHO_AM_I();
				break;

			case I_AM_RESP:
				LSM6DS33_date[pI_AM_D] = Data_in_i2c;
				LSM6DS33_state_machin = TRIGER_WAIT;
				break;

			case OUT_REQ:
				LSM6DS33_state_machin = OUT_WAIT;
				Read_new_value(pOUT_X_L_G + LSM6DS33_data_nr);
				break;

			case OUT_RESP:
				LSM6DS33_date[pOUT_X_L_G + LSM6DS33_data_nr] = Data_in_i2c;
				if(LSM6DS33_data_nr<11)
				{
					LSM6DS33_data_nr++;
					LSM6DS33_state_machin = OUT_REQ;
				}
				else
				{
					LSM6DS33_data_nr = 0;
					LSM6DS33_state_machin = TRIGER_WAIT;
					LSM6DS33_date[pNEW_VAL_D] = 1;
				}
				break;

			default:
				break;
		}
	}
}

void LSM6DS33_Triger()
{
	LSM6DS33_state_machin = OUT_REQ;
}
