/*
 * MCP4725.h
 *
 * Created: 14.04.2019 23:24:05
 *  Author: Hotwater
 */ 

#include <avr/io.h>

#ifndef MCP4725_H_
#define MCP4725_H_



/* Defines */

#define DAC_ADDRESS 0b11000010

#define DAC_CMD_WRITE 0
#define DAC_CMD_READ 1

#define DAC_MODE_FAST 0
#define DAC_MODE_NORMAL_DAC 2
#define DAC_MODE_NORMAL_EEPROM_DAC 3
#define DAC_MODE_NORMAL_POS	5
#define DAC_MODE_FAST_POS 6

#define DAC_SLEEP_AWAKE 0
#define DAC_SLEEP_1k 1
#define DAC_SLEEP_100k 2
#define DAC_SLEEP_500k 3
#define DAC_SLEEP_NORMAL_POS 1
#define DAC_SLEEP_FAST_POS 4

#define DAC_FAST_MS_DATA_POS 8
#define DAC_NORMAL_LS_DATA_POS 4

#define DAC_LSB_TO_uV 1220UL
#define DAC_MAX_CODE 4095
#define DAC_MAX_VOLTAGE 4990
#define mV_TO_uV 1000UL




/* Function pointers */

/* Variables */

typedef struct  
{
	void (*dacWriteFunction)(const uint8_t deviceAddres, const uint8_t registerAddres, const uint8_t data);
	void (*dacInitFunction)();
	uint8_t (*dacReadFunction)(const uint8_t deviceAddres, const uint8_t registerAddres);
}dac_config_t;

dac_config_t dac_actual_config;

/* Function prototypes */

void MCP4725_Initialize(dac_config_t *params);

void MCP4725_Set_Voltage_Fast(const uint16_t voltageInMillivolts);
void MCP4275_Set_Voltage_Normal(const uint16_t voltageInMillivolts);

#endif /* MCP4725_H_ */