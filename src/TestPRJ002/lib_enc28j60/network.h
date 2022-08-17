
#ifndef NETWORK
#define NETWORK

#include "udp.h"

#define NET_MIN_DINAMIC_PORT 49152
#define NET_MAX_PORT 65535

#if defined(TCP) || defined(UDP)
extern unsigned short connectPortRotaiting;
#endif

#define NET_HANDLE_RESULT_OK       0
#define NET_HANDLE_RESULT_DROP     1
#define NET_HANDLE_RESULT_REJECT   2

void NetInit();
void NetHandleNetwork();
void NetHandleIncomingPacket(unsigned short length);
unsigned char *NetGetBuffer();
#endif
