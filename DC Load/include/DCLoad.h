/*
 * DCLoad.h
 *
 * Created: 13.04.2019 23:02:45
 *  Author: Hotwater
 */ 

#include <avr/io.h>


#ifndef DCLOAD_H_
#define DCLOAD_H_

#define LOAD_MODE_DISABLED 0 
#define LOAD_MODE_CC 1
#define LOAD_MODE_CP 2

#define LOAD_CURRENT_HYSTERESIS 1U

#define LOAD_SHUNT_OHMS 6
#define LOAD_MILLIVOLTS_PER_CELCIUS 10U
#define LOAD_DIVISION_RATIO 11U

#define LOAD_TEMPERATURE_CHANNEL 0
#define LOAD_CURRENT_CHANNEL 1
#define LOAD_VOLTAGE_CHANNEL 2

#define REGULATE



/* Function pointers */


/* Variables */

typedef struct
{
	uint32_t (*adcReadFunction)(const uint8_t channel);
	void (*adcInitFunction)();
	void (*dacWriteFunction)(const uint16_t voltageInMillivolts);
	
	uint8_t currentChannel;
	uint8_t voltageChannel;
	uint8_t temperatureChannel;
	
}load_config_t;

typedef struct  
{
	uint32_t current;
	uint32_t voltage;
	uint32_t temperature;
	uint32_t power;
}load_params_t;

typedef struct
{
	uint32_t setMaxTemperature;
	uint32_t setCurrent;
	uint32_t setPower;
	uint32_t setMinVoltage;
	uint8_t mode;
}load_settings_t;

load_config_t load_actual_config;

uint32_t dacVoltage;




/* Function prototypes */
void Load_Initialize(load_config_t *params);
void Load_Run(load_params_t *paramsPtr, load_settings_t *settingsPtr);


uint32_t Load_Get_Current();
uint32_t Load_Get_Voltage();
uint32_t Load_Get_Temperature();
uint32_t Load_Get_Power();
	





#endif /* DCLOAD_H_ */