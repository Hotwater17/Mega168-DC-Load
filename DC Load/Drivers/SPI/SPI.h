/*
 * SPI.h
 *
 * Created: 23.05.2019 20:36:38
 *  Author: Hotwater
 */ 

#include <avr/io.h>

#ifndef SPI_H_
#define SPI_H_


void SPI_enable();

uint8_t SPI_Exchange_Byte(uint8_t dataToSend);



#endif /* SPI_H_ */