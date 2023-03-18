/*
 * main.c
 *
 *  Created on: 9 sie 2022
 *      Author: Borsuk
 */


/*******************************************************************************/
#include <avr/io.h>
#include <util/delay.h>
#include "i2c.h"

#define pcf8574_adres     0x40       // adres ekspandera

int main(void){

 uint8_t num, i = 0x00;
 uint8_t lcd[]={          // tablica cyfr
     0x77,        // 0
     0x14,        // 1
     0xb3,        // 2
     0xb6,        // 3
     0xd4,        // 4
     0xe6,        // 5
     0xe7,        // 6
     0x34,        // 7
     0xf7,         // 8
     0xf6          // 9
     };
 i2c_init();

 while(1){

  i2c_start(pcf8574_adres);
  for(i = 0; i<10; i++){            // wyświetla kolejne cyfry co 300ms
   i2c_write(lcd[i]);
   _delay_ms(300);
  }
  i2c_stop();
 }
}
