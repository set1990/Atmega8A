#ifndef CONFIG_H
#define CONFIG_H

#include <avr/io.h>
#include <util/delay.h>

// define SPI pins connected to enc28j60 SI, SO, SCK pins
// SPI pins can changes on different AVR processors look into chip datasheet where SPI pins are
// this example is for AVR atmega328p
#define ENC28J60_DDR DDRB
#define ENC28J60_PORT PORTB
#define ENC28J60_SI_PIN_DDR DDB3
#define ENC28J60_SI_PIN PORTB3
#define ENC28J60_SO_PIN_DDR DDB4
#define ENC28J60_SCK_PIN_DDR DDB5
#define ENC28J60_SCK_PIN PORTB5

// define rest digitals I/O pins connected to enc28j60 RESET, INT, CS pins
// this pins have to be in same DDR and PORT as SPI pins
#define ENC28J60_RESET_PIN_DDR DDB0
#define ENC28J60_RESET_PIN PORTB0
#define ENC28J60_INT_PIN_DDR DDB1
#define ENC28J60_INT_PIN PORTB1
#define ENC28J60_CS_PIN_DDR DDB2
#define ENC28J60_CS_PIN PORTB2

// define IP, MAC address for you AVR
#define NET_IP 192, 168, 1, 150
#define NET_MAC 0x15, 0x8, 0x45, 0x89, 0x69, 0x99
// define RAM buffer size for network packets
// this define maximum size of incoming packet,
// define maximum size of sended packet,
// and define tcp max segment size
// bigger value allow send and receive bigger packet (UDP datagram) and better performance for TCP
// lower value save RAM, you can create bigger buffer than AVR RAM or to big than no space for application :-)
// minimum recommended value is 600 bytes, in IPV4 standard minimum IP datagram size is 576 bytes
// you can use smaller buffer, if you sure than you do not receive bigger packets
#define NET_BUFFER_SIZE 600
// define size of arp cache array, default value 5
// arp requests for ip are cached in RAM
// defined zero size turn off cache and code for cache is not included in compilation
// bigger value allow save more ip in cache is good if your AVR communicate with more hosts
// lower value is good for more dynamic network in smaller cache will often rewrite unused ip in cache
#define NET_ARP_CACHE_SIZE 5


#endif
