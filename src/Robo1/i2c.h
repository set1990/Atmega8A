#ifndef I2C_H_
#define I2C_H_

#define READ_DETA_I2C	0x01
#define WRITE_DETA_I2C	0x00
extern char Data_in_i2c;
extern char Data_out_i2c;
extern char Transmision_Flag_i2c;
extern void (*I2C_back)(void);

void I2C_start(char adress, char RW_i2c);
void I2C_rep_start(char adress, char RW_i2c);
void I2C_call(void);
void I2C_read(void);
void I2C_write(void);
void I2C_stop(void);
void I2C_init(void);

#endif /* I2C_H_ */
