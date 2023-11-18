#ifndef SERWO_H_
#define SERWO_H_
#include <stdint.h>

#define MAX_VALUE			1000
#define ENABLE				1
#define DISABLE				0
#define SERWO_QUANTITY		0x06

void Serwo_ON_OFF(uint8_t num, uint8_t state);
void Serwo_init(void);
void Serwo_set_value(uint8_t num, uint16_t value);
#endif /* SERWO_H_ */
