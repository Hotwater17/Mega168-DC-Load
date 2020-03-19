/*
 * MCP4725.c
 *
 * Created: 14.04.2019 23:23:57
 *  Author: Hotwater
 */ 

#include "MCP4725.h"

/*
 *	@brief Initializes MCP4725 device and sets functions to communicate with DAC via I2C
 *	@param *writeFunction I2C 1 byte write function pointer with arguments: uint8_t deviceAddress, uint8_t registerAddress, uint8_t data
 *	@param *readFunction I2C read function pointer returning uint8_t data with arguments: uint8_t deviceAddres, uint8_t registerAddres
 *	@param *initFunction I2C initialize function pointer
 */
void MCP4725_Initialize(dac_config_t *params)
{

	dac_actual_config.dacInitFunction = params->dacInitFunction;
	dac_actual_config.dacReadFunction = params->dacReadFunction;
	dac_actual_config.dacWriteFunction = params->dacWriteFunction;
	
	dac_actual_config.dacInitFunction();
}

/*
 *	@brief Sets output voltage on DAC in fast mode
 *	@param [voltageInMillivolts] Voltage to be set on output expressed in millivolts
 */
void MCP4725_Set_Voltage_Fast(const uint16_t voltageInMillivolts)
{
	uint32_t dacCode = 0;
	uint16_t dacCmd = 0;
	uint16_t dacData = 0;
	
	//Check if voltage is in correct range(lesser than 5V), if not, set maximum DAC code.
	
	if(voltageInMillivolts < DAC_MAX_VOLTAGE)
	{
		//Conversion to uV 
		dacCode = (((uint32_t)(voltageInMillivolts)*mV_TO_uV)/DAC_LSB_TO_uV);
	}
	else
	{
		dacCode = DAC_MAX_CODE;
	}
	

	
	dacCmd |= (DAC_MODE_FAST << DAC_MODE_FAST_POS);
	dacCmd |= (DAC_SLEEP_AWAKE << DAC_SLEEP_FAST_POS);
	dacCmd |= (dacCode >> DAC_FAST_MS_DATA_POS);
	dacData |= (dacCode &  0x000000FF);
	
	dac_actual_config.dacWriteFunction((DAC_ADDRESS | DAC_CMD_WRITE), dacCmd, dacData);
	
	
}

/*
 *	@brief Sets output voltage on DAC in normal mode
 *	@param [voltageInMillivolts] Voltage to be set on output expressed in millivolts
 */
void MCP4275_Set_Voltage_Normal(const uint16_t voltageInMillivolts)
{
	
}