#ifndef STEPS_H_
#define STEPS_H_

#include <stdint.h>
#include <stddef.h>

#define MAX_STEPS	40
#define RUN_STEPS	1
#define PAUSE_STEPS 0

typedef struct{
	uint8_t num;
	uint16_t value;
}steps_q;

void Steps_run(void);
void Steps_add(uint8_t num, uint16_t value);
void Steps_clear(void);
void Steps_pause(void);
void Steps_Handle(void);
const unsigned char* Steps_list(uint8_t* size_list_out);

#endif /* STEPS_H_ */
