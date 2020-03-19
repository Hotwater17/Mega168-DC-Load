/*
 * SPI.c
 *
 * Created: 23.05.2019 20:36:30
 *  Author: Hotwater
 */ 

#include "SPI.h"

/*
 *	@brief Initialize master SPI, mode 0
 */
void SPI_Init()
{
	SPCR &= ~(1<<CPOL) & ~(1<<CPHA);
	SPCR |= (1<<SPE) | (1<<MSTR);
}

/*
 *	@brief Exchange one byte via SPI
 *	@param [dataToSend] byte which will be send
 *	@return received byte
 */
uint8_t SPI_Exchange_Byte(uint8_t dataToSend)
{
	SPDR = dataToSend;
	
	while(!(SPSR & (1<<SPIF)))
	{
		
	}
	
	return SPDR;
}