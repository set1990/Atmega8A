/*
 * eth.c
 *
 *  Created on: 10 sie 2022
 *      Author: Borsuk
 */
#include <string.h>

#include "serwo.h"
#include "eth.h"
#include "adc.h"
#include "steps.h"
#include "LIS3MDL.h"

#define ORDER_MAX 	  	0x10
#define ARG_MAX		  	ORDER_MAX-0x01
#define ADC_READ	  	0x01
#define SET_NEW_VLUE  	0x02
#define ENABLE_SERWO	0x03
#define DISABLE_SERWO	0x04
#define RUN_PROGRAM		0x05
#define ADD_STEP		0x06
#define CLEAR_STEPS		0x07
#define LIST_STEPS		0x08
#define LIS3MDL_TR		0x09
#define LIS3MDL_READ	0x0A

#define GET_SERWO_NUM(ARG)	 (ARG[0])
#define GET_SERWO_VALUE(ARG) ((ARG[1]<<8)|ARG[2])

typedef struct{
	unsigned char type;
	unsigned char arg[ARG_MAX];
} order;

typedef union{
	unsigned char data[ORDER_MAX];
	order		  order;
} datain;

datain EthData = {0};

char NewOrderFlag=0;

unsigned char UdpOnIncomingDatagram(const UdpDatagram datagram, const unsigned char *data, unsigned short dataLength)
{
	const unsigned char data_out_OK[] = "OK";
	const unsigned char data_out_FAIL[] = "FAIL";
	if(ORDER_MAX>=dataLength)
	{
		memcpy(EthData.data, data, dataLength);
		memcpy(&datagramin, &datagram, sizeof(UdpDatagram));
		UdpSendDataTmpPort(datagram.ip, datagram.port, data_out_OK, sizeof(data_out_OK));
		NewOrderFlag=1;
	}
	else UdpSendDataTmpPort(datagram.ip, datagram.port, data_out_FAIL, sizeof(data_out_FAIL));
	return 0;
}

void Eth_order_Handle()
{
	if(NewOrderFlag)
	{
		switch(EthData.order.type)
		{
			case SET_NEW_VLUE:
				Serwo_set_value(GET_SERWO_NUM(EthData.order.arg),GET_SERWO_VALUE(EthData.order.arg));
				NewOrderFlag=0;
				break;

			case ADC_READ:
				UdpSendDataTmpPort(datagramin.ip, datagramin.port, (const unsigned char *)(&ADC_value), sizeof(ADC_value));
				NewOrderFlag=0;
				break;

			case ENABLE_SERWO:
				Serwo_ON_OFF(GET_SERWO_NUM(EthData.order.arg), ENABLE);
				NewOrderFlag=0;
				break;

			case DISABLE_SERWO:
				Serwo_ON_OFF(GET_SERWO_NUM(EthData.order.arg), DISABLE);
				Steps_pause();
				NewOrderFlag=0;
				break;

			case RUN_PROGRAM:
				Steps_run();
				NewOrderFlag=0;
				break;

			case ADD_STEP:
				Steps_add(GET_SERWO_NUM(EthData.order.arg),GET_SERWO_VALUE(EthData.order.arg));
				NewOrderFlag=0;
				break;

			case CLEAR_STEPS:
				Steps_clear();
				NewOrderFlag=0;
				break;

			case LIST_STEPS:
				UdpSendDataTmpPort(datagramin.ip, datagramin.port, Steps_list(), sizeof(steps_q)*MAX_STEPS);
				NewOrderFlag=0;
				break;

			case LIS3MDL_TR:
				LIS3MDL_Triger();
				NewOrderFlag=0;
				break;

			case LIS3MDL_READ:
				UdpSendDataTmpPort(datagramin.ip, datagramin.port, (const unsigned char *)(&LIS3MDL_date), sizeof(LIS3MDL_date));
				LIS3MDL_date[pNEW_VAL] = 0;
				NewOrderFlag=0;
				break;

			default:
				NewOrderFlag=0;
				break;
		}
	}

}
