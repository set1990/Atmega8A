#include <avr/io.h>
#include <compat/twi.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include "i2c.h"

char Data_in_i2c  = 0;
char Data_out_i2c = 0;
char Transmision_Flag_i2c   = 0;
char Transmision_adress_i2c = 0;
char Transmision_RW_i2c		= 0; // 0-W, 1-R
char Continue_Flag_i2c      = 0;

void (*I2C_back)(void) = NULL;

void I2C_init(void)
{
	TWSR = 0;
	TWBR = 0x10;
	TWCR |= (1<<TWIE);
	sei();
}

void I2C_rep_start(char adress, char RW_i2c)
{
	Transmision_adress_i2c = adress;
	Transmision_RW_i2c = RW_i2c;
	TWDR = (Transmision_adress_i2c<<1) | Transmision_RW_i2c;
    TWCR |= (1<<TWINT) | (1<<TWEN) |( 1<<TWSTA);
}

void I2C_start(char adress, char RW_i2c)
{
	Transmision_Flag_i2c = 1;
	Transmision_adress_i2c = adress;
	Transmision_RW_i2c = RW_i2c;
	TWCR &= ~((1<<TWSTO));
	TWCR |= (1<<TWINT) | (1<<TWEN) |( 1<<TWSTA);
}

void I2C_call()
{
    TWDR = (Transmision_adress_i2c<<1) | Transmision_RW_i2c;
    TWCR &= ~((1<<TWSTA));
    TWCR |= (1<<TWINT) | (1<<TWEN);
}

void I2C_read()
{
	Data_in_i2c  = TWDR;
}

void I2C_write()
{
	TWDR = Data_out_i2c;
	TWCR |= (1<<TWINT) | (1<<TWEN);
}

void I2C_stop()
{
	TWCR |= (1<<TWINT) | (1<<TWEN) | (1<<TWSTO);
	Transmision_Flag_i2c = 0;
}

void I2C_next()
{
	TWCR |= (1<<TWINT) | (1<<TWEN) | (1<<TWSTO);
	Continue_Flag_i2c = 0;
}

ISR(TWI_vect)
{
	static char writed_flag = 0;
	switch(TWSR & 0xF8)
	{
    	case TW_START:  // start transmitted
    		writed_flag = 0;
    		I2C_call();
    		break;

    	case TW_MT_SLA_ACK: // SLA+W transmitted, ACK received
    		I2C_write();
    		break;

    	case TW_MT_DATA_ACK:
    		if(writed_flag == 1)
    		{
    			I2C_stop();
    			writed_flag = 0;
    		}
    		else if(I2C_back!=NULL)
    		{
    			writed_flag = 1;
    			I2C_back();
    		}
    		else I2C_stop();
    		break;

    	case TW_MR_SLA_ACK: // SLA+R transmitted, ACK received
    	case TW_REP_START:
    		TWCR &= ~((1<<TWSTA) | (1<<TWSTO));
    		TWCR |= (1<<TWINT) | (1<<TWEN);
    		break;

    	case TW_MR_DATA_ACK: // data received, ACK returned
    	case TW_MR_DATA_NACK:
    		I2C_read();
    		if(Continue_Flag_i2c == 0) I2C_stop();
    		break;
  }
}


