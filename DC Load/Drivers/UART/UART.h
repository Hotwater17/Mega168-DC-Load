/*
 * UART.h
 *
 * Created: 23.05.2019 20:36:47
 *  Author: Hotwater
 */ 

#include <avr/io.h>
#include <stdlib.h>
#include <stdbool.h>

#ifndef UART_H_
#define UART_H_

#define BAUD_REG_9600 51
#define BAUD_REG_19200 25

//NOT RECOMMENDED
#define BAUD_REG_57600 8
//NOT RECOMMENDED
#define BAUD_REG_115200 3



void Uart_Init(uint16_t baud);
void Uart_Send_Byte(uint8_t data);
void Uart_Send_Text(char *dataPtr);
void Uart_Send_Int32(int32_t data);

uint8_t Uart_Is_Data_Received();
uint8_t Uart_Receive_Byte();

uint16_t Uart_Receive_Text(char *destinationPtr);






#endif /* UART_H_ */