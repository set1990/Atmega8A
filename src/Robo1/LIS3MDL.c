#include "i2c.h"
#include "LIS3MDL.h"
#include "eth.h"

#define LIS3MDL_ADDRESS		0x1E
#define WHO_AM_I		 	0x0F
#define CTRL_REG1		 	0x20
#define OM1				 	0x06
#define OM0				 	0x05
#define DO2					0x04
#define CTRL_REG3		 	0x22
#define MD1				 	0x01
#define MD0				 	0x00
#define CTRL_REG4		 	0x23
#define OZ1				 	0x03
#define OZ0				 	0x02
#define STATUS_REG			0x27
#define ZYXOR				0x07
#define ZYXDA				0x03
#define OUT_X_L				0x28

#define UltraHighPerformanceXY()\
{\
	Data_out_i2c = CTRL_REG1;\
	I2C_back = Data_Write;\
	I2C_start(LIS3MDL_ADDRESS, WRITE_DETA_I2C);\
	data_to_write = (1<<OM1)|(1<<OM0)|(1<<DO2);\
}

#define UltraHighPerformanceZ()\
{\
	Data_out_i2c = CTRL_REG4;\
	I2C_back = Data_Write;\
	I2C_start(LIS3MDL_ADDRESS, WRITE_DETA_I2C);\
	data_to_write = (1<<OZ1)|(1<<OZ0);\
}

#define Mesurment_Triger()\
{\
	Data_out_i2c = CTRL_REG3;\
	I2C_back = Data_Write;\
	I2C_start(LIS3MDL_ADDRESS, WRITE_DETA_I2C);\
	data_to_write = (1<<MD0);\
}

#define Check_new_value()\
{\
	Data_out_i2c = STATUS_REG;\
	I2C_back = Data_Read_Triger;\
	I2C_start(LIS3MDL_ADDRESS, WRITE_DETA_I2C);\
}

#define Read_new_value(nr)\
{\
	Data_out_i2c = OUT_X_L+nr-1;\
	I2C_back = Data_Read_Triger;\
	I2C_start(LIS3MDL_ADDRESS, WRITE_DETA_I2C);\
}

#define Check_WHO_AM_I()\
{\
	Data_out_i2c = WHO_AM_I;\
	I2C_back = Data_Read_Triger;\
	I2C_start(LIS3MDL_ADDRESS, WRITE_DETA_I2C);\
}

typedef enum
{
	InitUHPXY,
	InitUHPZ,
	I_AM_REQ,
	I_AM_WAIT,
	I_AM_RESP,
	TRIGER_WAIT,
	TRIGER_REQ,
	NEW_VALUE_REQ,
	NEW_VALUE_WAIT,
	NEW_VALUE_RESP,
	OUT_X_L_REQ,
	OUT_X_L_WAIT,
	OUT_X_L_RESP,
	OUT_X_H_REQ,
	OUT_X_H_WAIT,
	OUT_X_H_RESP,
	OUT_Y_L_REQ,
	OUT_Y_L_WAIT,
	OUT_Y_L_RESP,
	OUT_Y_H_REQ,
	OUT_Y_H_WAIT,
	OUT_Y_H_RESP,
	OUT_Z_L_REQ,
	OUT_Z_L_WAIT,
	OUT_Z_L_RESP,
	OUT_Z_H_REQ,
	OUT_Z_H_WAIT,
	OUT_Z_H_RESP
} LIS3MDL_state;
LIS3MDL_state LIS3MDL_state_machin = InitUHPXY;

char LIS3MDL_date[8] = {0};
static char data_to_write = 0;

static void Data_Read_Triger()
{
	I2C_rep_start(LIS3MDL_ADDRESS, READ_DETA_I2C);
	switch(LIS3MDL_state_machin)
	{
		case I_AM_WAIT:
			LIS3MDL_state_machin = I_AM_RESP;
			break;

		case NEW_VALUE_WAIT:
			LIS3MDL_state_machin = NEW_VALUE_RESP;
			break;

		case OUT_X_L_WAIT:
			LIS3MDL_state_machin = OUT_X_L_RESP;
			break;

		case OUT_X_H_WAIT:
			LIS3MDL_state_machin = OUT_X_H_RESP;
			break;

		case OUT_Y_L_WAIT:
			LIS3MDL_state_machin = OUT_Y_L_RESP;
			break;

		case OUT_Y_H_WAIT:
			LIS3MDL_state_machin = OUT_Y_H_RESP;
			break;

		case OUT_Z_L_WAIT:
			LIS3MDL_state_machin = OUT_Z_L_RESP;
			break;

		case OUT_Z_H_WAIT:
			LIS3MDL_state_machin = OUT_Z_H_RESP;
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

void LIS3MDL_Handle()
{
	if(Transmision_Flag_i2c==0)
	{
		switch(LIS3MDL_state_machin)
		{
			case InitUHPXY:
				LIS3MDL_state_machin = InitUHPZ;
				UltraHighPerformanceXY();
				break;

			case InitUHPZ:
				LIS3MDL_state_machin = I_AM_REQ;
				UltraHighPerformanceZ();
				break;

			case I_AM_REQ:
				LIS3MDL_state_machin = I_AM_WAIT;
				Check_WHO_AM_I();
				break;

			case I_AM_RESP:
				LIS3MDL_date[pI_AM] = Data_in_i2c;
				LIS3MDL_state_machin = TRIGER_WAIT;
				break;

			case TRIGER_REQ:
				LIS3MDL_state_machin = NEW_VALUE_REQ;
				Mesurment_Triger();
				break;

			case NEW_VALUE_REQ:
				LIS3MDL_state_machin = NEW_VALUE_WAIT;
				Check_new_value();
				break;

			case NEW_VALUE_RESP:
				if(Data_in_i2c&(1<<ZYXDA))
					 LIS3MDL_state_machin = OUT_X_L_REQ;
				else LIS3MDL_state_machin = NEW_VALUE_REQ;
				break;

			case OUT_X_L_REQ:
				LIS3MDL_state_machin = OUT_X_L_WAIT;
				Read_new_value(pOUT_X_L);
				break;

			case OUT_X_L_RESP:
				LIS3MDL_date[pOUT_X_L] = Data_in_i2c;
				LIS3MDL_state_machin = OUT_X_H_REQ;
				break;

			case OUT_X_H_REQ:
				LIS3MDL_state_machin = OUT_X_H_WAIT;
				Read_new_value(pOUT_X_H);
				break;

			case OUT_X_H_RESP:
				LIS3MDL_date[pOUT_X_H] = Data_in_i2c;
				LIS3MDL_state_machin = OUT_Y_L_REQ;
				break;

			case OUT_Y_L_REQ:
				LIS3MDL_state_machin = OUT_Y_L_WAIT;
				Read_new_value(pOUT_Y_L);
				break;

			case OUT_Y_L_RESP:
				LIS3MDL_date[pOUT_Y_L] = Data_in_i2c;
				LIS3MDL_state_machin = OUT_Y_H_REQ;
				break;

			case OUT_Y_H_REQ:
				LIS3MDL_state_machin = OUT_Y_H_WAIT;
				Read_new_value(pOUT_Y_H);
				break;

			case OUT_Y_H_RESP:
				LIS3MDL_date[pOUT_Y_H] = Data_in_i2c;
				LIS3MDL_state_machin = OUT_Z_L_REQ;
				break;

			case OUT_Z_L_REQ:
				LIS3MDL_state_machin = OUT_Z_L_WAIT;
				Read_new_value(pOUT_Z_L);
				break;

			case OUT_Z_L_RESP:
				LIS3MDL_date[pOUT_Z_L] = Data_in_i2c;
				LIS3MDL_state_machin = OUT_Z_H_REQ;
				break;

			case OUT_Z_H_REQ:
				LIS3MDL_state_machin = OUT_Z_H_WAIT;
				Read_new_value(pOUT_Z_H);
				break;

			case OUT_Z_H_RESP:
				LIS3MDL_date[pOUT_Z_H] = Data_in_i2c;
				LIS3MDL_state_machin = TRIGER_WAIT;
				LIS3MDL_date[pNEW_VAL] = 1;
				break;

			default:
				break;
		}
	}
}

void LIS3MDL_Triger()
{
	LIS3MDL_state_machin = TRIGER_REQ;
}
