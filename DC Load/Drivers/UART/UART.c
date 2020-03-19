/*
 * UART.c
 *
 * Created: 23.05.2019 20:36:56
 *  Author: Hotwater
 */ 

#include "UART.h"

/*
 *	@brief Initializes UART, 8 data bits, 1 stop, no parity
 *	@param [baud] UBRR register contents
 */
void Uart_Init(uint16_t baud)
{
	
	DDRD &= ~(1<<PD0);
	DDRD |= (1<<PD1);
	PORTD &= ~(1<<PD0) & ~(1<<PD1);
		
	UBRR0H = 0;
	UBRR0L = baud;
	UCSR0C |= (1<<UCSZ01) | (1<<UCSZ00);
	UCSR0B |= (1<<RXEN0) | (1<<TXEN0);

}

/*
 *	@brief Sends byte via UART
 *	@param [data] byte to send
 */
void Uart_Send_Byte(uint8_t data)
{
	while(!(UCSR0A & (1 << UDRE0)))
	{	
		
	}
	UDR0 = data;
}

/*
 *	@brief Sends text via UART
 *	@param [*dataPtr] pointer to text array to send
 */
void Uart_Send_Text(char *dataPtr)
{
	uint16_t dataCnt = 0;
	uint16_t size = sizeof(dataPtr);
	while(dataPtr[dataCnt] != 0)
	{
		Uart_Send_Byte(dataPtr[dataCnt]);
		dataCnt++;
		
	}
}

/*
 *	@brief Sends int32 number via UART
 *	@param [data] number to send
 */
void Uart_Send_Int32(int32_t data)
{
	char textBuffer[20];
	for (uint8_t bufferCnt = 0; bufferCnt < 20; bufferCnt++)
	{
		textBuffer[bufferCnt] = 0;
	}
	itoa(data, textBuffer, 10);
	Uart_Send_Text(textBuffer);
}

/*
 *	@brief Receives byte via UART
 *	@return received byte
 */
uint8_t Uart_Receive_Byte()
{
	while(!Uart_Is_Data_Received())
	{
		
	}
	return UDR0;
}

/*
 *	@brief Receives text via UART
 *	@param [*destinationPtr] pointer to array in which received text will be stored
 *	@return received text length
 */
uint16_t Uart_Receive_Text(char *destinationPtr)
{
	uint16_t sizeCnt;
	uint8_t receivedByte;
	
	for(sizeCnt = 0; sizeCnt < sizeof(destinationPtr); sizeCnt++)
	{
		destinationPtr[sizeCnt] = 0;
	}
	
	sizeCnt = 0;
	
	do 
	{
		receivedByte = Uart_Receive_Byte();
		destinationPtr[sizeCnt] = receivedByte;
		sizeCnt++;
		if(sizeCnt == sizeof(destinationPtr)) break;
	} while (receivedByte != 0);

	
	return sizeCnt;
}

/*
 *	@brief Checks if new data is received via UART
 *	@return Readiness of new data (0 - no new data, 1 - new data awaits in data buffer)
 */
uint8_t Uart_Is_Data_Received()
{
	return (UCSR0A & (1 << RXC0));
}