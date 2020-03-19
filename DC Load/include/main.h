/*
 * main.h
 *
 * Created: 02.06.2019 18:40:59
 *  Author: Hotwater
 */ 




#ifndef MAIN_H_
#define MAIN_H_

typedef struct
{
	uint16_t milliseconds;
	uint8_t seconds;
	uint8_t minutes;
	uint8_t hours;
}time_t;

#include "DCLoad.h"
#include "MCP4725.h"
#include "HMI.h"
#include "menu.h"


#include "Drivers/I2C/I2C.h"
#include "Drivers/ADC/ADC.h"
#include "Drivers/GPIO/GPIO.h"
#include "Drivers/UART/UART.h"
#include "Drivers/TC16/TC16.h"

#define TIME_NOMATCH 0
#define TIME_MATCH 1

#define TIME_INCREMENT_100MILLIS 1
#define TIME_INCREMENT_SECONDS 2
#define TIME_INCREMENT_MINUTES 3

void User_DAC_I2C_Init();
void User_DAC_I2C_Write(const uint8_t deviceAddress, const uint8_t registerAddress, const uint8_t data);
uint8_t User_DAC_I2C_Read(const uint8_t deviceAddress, const uint8_t registerAddress);

void User_ADC_Init();
uint32_t User_ADC_Read(uint8_t channel);

void User_Button_Init();
uint8_t User_Button_Read(uint8_t button);

void Serial_Send_Parameters(load_params_t *loadData, time_t *timeData);
void Serial_Send_Text(char *textPtr);
void Time_Increment(time_t *timePtr, uint8_t increment);
void Time_Decrement(time_t *timePtr, uint8_t decrement);
uint8_t Time_Compare(time_t *time1, time_t *time2);
void Time_Reset(time_t *timePtr);

#endif /* MAIN_H_ */