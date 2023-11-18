#include <avr/io.h>
#include <util/delay.h>
#include "lib_enc28j60/network.h"
#include "serwo.h"
#include "eth.h"
#include "adc.h"
#include "steps.h"
#include "i2c.h"
#include "LIS3MDL.h"
#include "LSM6DS33.h"

int main(void)
{
	NetInit();
	Serwo_init();
	I2C_init();

	while(1)
	{
		NetHandleNetwork();
		Eth_order_Handle();
		ADC_Handle();
		Steps_Handle();
		LIS3MDL_Handle();
		LSM6DS33_Handle();
		_delay_ms(10);
    }
}
