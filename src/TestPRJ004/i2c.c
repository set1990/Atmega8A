/*
 * i2c.c
 *
 *  Created on: 28 lut 2023
 *      Author: Borsuk
 */


#include <avr/io.h>
#include <compat/twi.h>
#include "i2c.h"

/*** Inicjalizacja magistarli I2C ***/
void i2c_init(void){

 TWSR = 0;    // wartosć preskalera 1
 TWBR = 0x10;   // ustawianie wartosci rejestru TWBR
}

/*** Start magistrali, ustawienie adresu i kierunku transmisji ***/
unsigned char i2c_start(unsigned char adres){

 uint8_t twst;

 // send START condition
 TWCR = (1<<TWINT) | (1<<TWSTA) | (1<<TWEN);

 // wait until transmission completed
 while(!(TWCR & (1<<TWINT)));

 // check value of TWI Status Register. Mask prescaler bits.
 twst = TW_STATUS & 0xF8;
 if ( (twst != TW_START) && (twst != TW_REP_START)) return 1;

 // send device address
 TWDR = adres;
 TWCR = (1<<TWINT) | (1<<TWEN);

 // wail until transmission completed and ACK/NACK has been received
 while(!(TWCR & (1<<TWINT)));

 // check value of TWI Status Register. Mask prescaler bits.
 twst = TW_STATUS & 0xF8;
 if ( (twst != TW_MT_SLA_ACK) && (twst != TW_MR_SLA_ACK) ) return 1;

 return 0;
}

/*** Wysyła bajt na magistrale I2C ***/
unsigned char i2c_write( unsigned char data ){

    uint8_t   twst;

 // send data to the previously addressed device
 TWDR = data;
 TWCR = (1<<TWINT) | (1<<TWEN);

 // wait until transmission completed
 while(!(TWCR & (1<<TWINT)));

 // check value of TWI Status Register. Mask prescaler bits
 twst = TW_STATUS & 0xF8;
 if( twst != TW_MT_DATA_ACK) return 1;
 return 0;
}
/*** Koniec transmisji I2C ***/
void i2c_stop(void){

    /* send stop condition */
 TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWSTO);

 // wait until stop condition is executed and bus released
 while(TWCR & (1<<TWSTO));
}

/*************************************************************************
 Read one byte from the I2C device, request more data from device

 Return:  byte read from I2C device
*************************************************************************/
unsigned char i2c_readAck(void)
{
 TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWEA);
 while(!(TWCR & (1<<TWINT)));

    return TWDR;
}

/*************************************************************************
 Read one byte from the I2C device, read is followed by a stop condition

 Return:  byte read from I2C device
*************************************************************************/
unsigned char i2c_readNak(void)
{
 TWCR = (1<<TWINT) | (1<<TWEN);
 while(!(TWCR & (1<<TWINT)));

    return TWDR;
}

/*******************************************************************************/
