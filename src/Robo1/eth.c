#include <string.h>

#include "serwo.h"
#include "eth.h"
#include "adc.h"
#include "steps.h"
#include "LIS3MDL.h"
#include "LSM6DS33.h"

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
#define PING			0x0B
#define LSM6DS33_TR		0x0C
#define LSM6DS33_READ	0x0D

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
const unsigned char data_out_FAIL[] = "FAIL";
const unsigned char data_out_OK[] = "OK";

unsigned char UdpOnIncomingDatagram(const UdpDatagram datagram, const unsigned char *data, unsigned short dataLength)
{
	if(ORDER_MAX>=dataLength)
	{
		memcpy(EthData.data, data, dataLength);
		memcpy(&datagramin, &datagram, sizeof(UdpDatagram));
		if(LSM6DS33_READ >= EthData.order.type) NewOrderFlag=1;
		else UdpSendData(datagram.ip, datagramin.remotePort, datagram.port, data_out_FAIL, sizeof(data_out_FAIL));
	}
	else UdpSendData(datagram.ip, datagramin.remotePort, datagram.port, data_out_FAIL, sizeof(data_out_FAIL));
	return 0;
}

void Eth_order_Handle()
{
	uint8_t step_cnt = 0;
	const unsigned char* list;
	if(NewOrderFlag)
	{
		switch(EthData.order.type)
		{
			case SET_NEW_VLUE:
				Serwo_set_value(GET_SERWO_NUM(EthData.order.arg),GET_SERWO_VALUE(EthData.order.arg));
				NewOrderFlag=0;
				UdpSendData(datagramin.ip, datagramin.remotePort, datagramin.port, data_out_OK, sizeof(data_out_OK));
				break;

			case ADC_READ:
				UdpSendData(datagramin.ip, datagramin.remotePort, datagramin.port, (const unsigned char *)(&ADC_value), sizeof(ADC_value));
				NewOrderFlag=0;
				break;

			case ENABLE_SERWO:
				Serwo_ON_OFF(GET_SERWO_NUM(EthData.order.arg), ENABLE);
				NewOrderFlag=0;
				UdpSendData(datagramin.ip, datagramin.remotePort, datagramin.port, data_out_OK, sizeof(data_out_OK));
				break;

			case DISABLE_SERWO:
				Serwo_ON_OFF(GET_SERWO_NUM(EthData.order.arg), DISABLE);
				Steps_pause();
				NewOrderFlag=0;
				UdpSendData(datagramin.ip, datagramin.remotePort, datagramin.port, data_out_OK, sizeof(data_out_OK));
				break;

			case RUN_PROGRAM:
				Steps_run();
				NewOrderFlag=0;
				UdpSendData(datagramin.ip, datagramin.remotePort, datagramin.port, data_out_OK, sizeof(data_out_OK));
				break;

			case ADD_STEP:
				Steps_add(GET_SERWO_NUM(EthData.order.arg),GET_SERWO_VALUE(EthData.order.arg));
				NewOrderFlag=0;
				UdpSendData(datagramin.ip, datagramin.remotePort, datagramin.port, data_out_OK, sizeof(data_out_OK));
				break;

			case CLEAR_STEPS:
				Steps_clear();
				NewOrderFlag=0;
				UdpSendData(datagramin.ip, datagramin.remotePort, datagramin.port, data_out_OK, sizeof(data_out_OK));
				break;

			case LIST_STEPS:
				list = Steps_list(&step_cnt);
				UdpSendData(datagramin.ip, datagramin.remotePort, datagramin.port, list, sizeof(steps_q)*((step_cnt !=0) ? step_cnt : 1));
				NewOrderFlag=0;
				break;

			case LIS3MDL_TR:
				LIS3MDL_Triger();
				NewOrderFlag=0;
				UdpSendData(datagramin.ip, datagramin.remotePort, datagramin.port, data_out_OK, sizeof(data_out_OK));
				break;

			case LIS3MDL_READ:
				UdpSendData(datagramin.ip, datagramin.remotePort, datagramin.port,(const unsigned char *)(&LIS3MDL_date), sizeof(LIS3MDL_date));
				LIS3MDL_date[pNEW_VAL] = 0;
				NewOrderFlag=0;
				break;

			case LSM6DS33_TR:
				LSM6DS33_Triger();
				NewOrderFlag=0;
				UdpSendData(datagramin.ip, datagramin.remotePort, datagramin.port, data_out_OK, sizeof(data_out_OK));
				break;

			case LSM6DS33_READ:
				UdpSendData(datagramin.ip, datagramin.remotePort, datagramin.port,(const unsigned char *)(&LSM6DS33_date), sizeof(LSM6DS33_date));
				LSM6DS33_date[pNEW_VAL_D] = 0;
				NewOrderFlag=0;
				break;

			case PING:
				UdpSendData(datagramin.ip, datagramin.remotePort, datagramin.port, data_out_OK, sizeof(data_out_OK));
				NewOrderFlag=0;
				break;

			default:
				NewOrderFlag=0;
				break;
		}
	}

}
