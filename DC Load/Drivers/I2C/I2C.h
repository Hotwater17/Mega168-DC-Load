/*
 * I2C.h
 *
 * Created: 23.05.2019 20:34:15
 *  Author: g4lvanix
 * https://github.com/g4lvanix
 * Modified by: Hotwater
 */ 


#ifndef I2C_H_
#define I2C_H_

/* Definitions */

#define SDA_Pin 4
#define SDA_PORT PORTC
#define SDA_DDR DDRC

#define SCL_Pin 5
#define SCL_PORT PORTC
#define SCL_DDR DDRC

#define I2C_READ 0x01
#define I2C_WRITE 0x00

/* Function prototypes */

void i2c_init(void);
void i2c_stop(void);

uint8_t i2c_start(uint8_t address);
uint8_t i2c_write(uint8_t data);
uint8_t i2c_read_ack(void);
uint8_t i2c_read_nack(void);
uint8_t i2c_transmit(uint8_t address, uint8_t* data, uint16_t length);
uint8_t i2c_receive(uint8_t address, uint8_t* data, uint16_t length);
uint8_t i2c_writeReg(uint8_t devaddr, uint8_t regaddr, uint8_t* data, uint16_t length);
uint8_t i2c_readReg(uint8_t devaddr, uint8_t regaddr, uint8_t* data, uint16_t length);


#endif /* I2C_H_ */