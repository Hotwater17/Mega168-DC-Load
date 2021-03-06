/*
 * I2C.c
 *
 * Created: 23.05.2019 20:34:07
 *  Author: g4lvanix
 * https://github.com/g4lvanix
 * Modified by: Hotwater
 */ 

#include <util/twi.h>
#include "I2C.h"


#define F_SCL 100000UL // SCL frequency
#define Prescaler 1
#define TWBR_val ((((F_CPU / F_SCL) / Prescaler) - 16 ) / 2)

/*
 *	@brief Initializes I2C
 */
void i2c_init(void)
{
	SCL_DDR |= (1<<SCL_Pin);
	SDA_DDR |= (1<<SDA_Pin);
	
	TWBR = (uint8_t)TWBR_val;
	//TWBR = 192;
}

/*
 *	@brief sends Start condition and slave address
 *	@param [address]: slave address
 *	@return: Slave acknowledge (0 - NACK, 1 - ACK)
 */
uint8_t i2c_start(uint8_t address)
{
	uint8_t retVal;
	// reset TWI control register
	TWCR = 0;
	// transmit START condition
	TWCR = (1<<TWINT) | (1<<TWSTA) | (1<<TWEN);
	// wait for end of transmission
	while( !(TWCR & (1<<TWINT)) );
	
	// check if the start condition was successfully transmitted
	if((TWSR & 0xF8) != TW_START)
	{ 
		return 1;
		
	}
	
	// load slave address into data register
	TWDR = address;
	// start transmission of address
	TWCR = (1<<TWINT) | (1<<TWEN);
	// wait for end of transmission
	while( !(TWCR & (1<<TWINT)) );
	
	// check if the device has acknowledged the READ / WRITE mode
	uint8_t twst = TW_STATUS & 0xF8;
	if ( (twst != TW_MT_SLA_ACK) && (twst != TW_MR_SLA_ACK) )
	{
		return 1;
	}
	
	return 0;
}

/*
 *	@brief Writes data byte to slave
 *	@param [data]: byte to send
 *	@return: Slave acknowledge (0 - NACK, 1 - ACK)
 */
uint8_t i2c_write(uint8_t data)
{
	// load data into data register
	TWDR = data;
	// start transmission of data
	TWCR = (1<<TWINT) | (1<<TWEN);
	// wait for end of transmission
	while( !(TWCR & (1<<TWINT)) );
	
	if( (TWSR & 0xF8) != TW_MT_DATA_ACK )
	{ 
		return 1;
	}
	
	return 0;
}

/*
 * @brief Reads I2C byte with ACK
 *	@return: received data
 */
uint8_t i2c_read_ack(void)
{
	
	// start TWI module and acknowledge data after reception
	TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWEA);
	// wait for end of transmission
	while( !(TWCR & (1<<TWINT)) );
	// return received data from TWDR
	return TWDR;
}

/*
 * @brief Reads I2C byte with NACK
 *	@return: received data
 */
uint8_t i2c_read_nack(void)
{
	
	// start receiving without acknowledging reception
	TWCR = (1<<TWINT) | (1<<TWEN);
	// wait for end of transmission
	while( !(TWCR & (1<<TWINT)) );
	// return received data from TWDR
	return TWDR;
}

/*
 *	@brief Transmits data to slave 
 *	@param [address]: slave address
 *	@param [*data]: pointer to data to send
 *	@param [length]: length of data buffer
 *	@return: error message
 */
uint8_t i2c_transmit(uint8_t address, uint8_t* data, uint16_t length)
{
	if (i2c_start(address | I2C_WRITE)) return 1;
	
	for (uint16_t i = 0; i < length; i++)
	{
		if (i2c_write(data[i]))
		{
			return 1;
		}
	}
	
	i2c_stop();
	
	return 0;
}

/*
 *	@brief Receives data from slave 
 *	@param [address]: slave address
 *	@param [*data]: pointer to data buffer
 *	@param [length]: length of data buffer
 *	@return: error message
 */
uint8_t i2c_receive(uint8_t address, uint8_t* data, uint16_t length)
{
	if (i2c_start(address | I2C_READ)) return 1;
	
	for (uint16_t i = 0; i < (length-1); i++)
	{
		data[i] = i2c_read_ack();
	}
	data[(length-1)] = i2c_read_nack();
	
	i2c_stop();
	
	return 0;
}

/*
 *	@brief Writes to register with specific address
 *	@param [devaddr]: slave address
 *	@param [regaddr]: register address
 *	@param [*data]: pointer to data to write
 *	@param [length]: length of data buffer
 *	@return error message
 */
uint8_t i2c_writeReg(uint8_t devaddr, uint8_t regaddr, uint8_t* data, uint16_t length)
{
	if (i2c_start(devaddr | 0x00))
	{
		return 1;
	}

	i2c_write(regaddr);

	for (uint16_t i = 0; i < length; i++)
	{
		if (i2c_write(data[i])) 
		{
			return 1;
		}
	}

	i2c_stop();

	return 0;
}

/*
 *	@brief Reads from register with specific address
 *	@param [devaddr]: slave address
 *	@param [regaddr]: register address
 *	@param [*data]: pointer to data buffer
 *	@param [length]: length of data buffer
 *	@return error message
 */
uint8_t i2c_readReg(uint8_t devaddr, uint8_t regaddr, uint8_t* data, uint16_t length)
{
	if (i2c_start(devaddr))
	{
		 return 1;
	}

	i2c_write(regaddr);

	if (i2c_start(devaddr | 0x01))
	{
		 return 1;
	}

	for (uint16_t i = 0; i < (length-1); i++)
	{
		data[i] = i2c_read_ack();
	}
	data[(length-1)] = i2c_read_nack();

	i2c_stop();

	return 0;
}

/*
 *	@brief Sends I2C stop condition
 */
void i2c_stop(void)
{
	// transmit STOP condition
	TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWSTO);
}