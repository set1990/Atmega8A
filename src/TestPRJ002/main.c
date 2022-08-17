/*
 * main.c
 *
 *  Created on: 9 sie 2022
 *      Author: Borsuk
 */
#include <avr/io.h>
#include <util/delay.h>

#include "lib_enc28j60/network.h"
#include "lib_enc28j60/udp.h"

unsigned char UdpOnIncomingDatagram(const UdpDatagram datagram, const unsigned char *data, unsigned short dataLength)
{
	const unsigned char PC_Ip[4] = {192, 168, 1, 151};
	const unsigned char data_out[] = "HELO3";
	UdpSendDataTmpPort(PC_Ip, 5001, data_out, sizeof(data_out));
	return 0;
}

int main(void)
{
	NetInit();
	while(1)
	{
		NetHandleNetwork();
	}
}
