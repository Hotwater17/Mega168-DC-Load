/*
 * HMI.h
 *
 * Created: 13.04.2019 23:17:33
 *  Author: Hotwater
 */ 

#include <avr/io.h>
#include <stdbool.h>
#include <stdlib.h>

#include "main.h"
#include "HD44780.h"


#ifndef HMI_H_
#define HMI_H_

/* Defines */ 

#define BUT0	0
#define BUT1	1
#define BUT2	2

#define BUTTON_PUSHED 0
#define BUTTON_RELEASED 1

#define BUT0_DDR	DDRD
#define BUT0_PORT	PORTD
#define BUT0_PIN	PD2

#define BUT1_DDR	DDRD
#define BUT1_PORT	PORTD
#define BUT1_PIN	PD3

#define BUT2_DDR	DDRC
#define BUT2_PORT	PORTC
#define BUT2_PIN	PC3

/* Variables */

typedef struct  
{
	void (*buttonInitFunction)();
	uint8_t (*buttonReadFunction)(uint8_t button);
}hmi_config_t;

hmi_config_t hmi_actual_config;

char txtBuffer[20];

/* Function prototypes */

void HMI_Initialize(hmi_config_t *params);

void HMI_Display_Current(uint32_t currentInMilliamperes);
void HMI_Display_Voltage(uint32_t voltageInMillivolts);
void HMI_Display_Power(uint32_t powerInMilliwatts);
void HMI_Display_Temperature(uint32_t temperatureInCelcius);
void HMI_Display_Cursor(uint8_t x, uint8_t y);
void HMI_Hide_Cursor();
void HMI_Display_Locate(uint8_t x, uint8_t y);
void HMI_Display_Mode(uint8_t currentMode);
void HMI_Display_Time(time_t *timePtr);
void HMI_Display_Value(uint32_t value);
void HMI_Display_Text(char *textPtr);
void HMI_Display_Clear();


bool HMI_Is_Button_Pressed(uint8_t buttonNumber);


#endif /* HMI_H_ */