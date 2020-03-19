/*
 * DCLoad.c
 *
 * Created: 13.04.2019 23:02:31
 *  Author: Hotwater
 */ 

#include <stdlib.h>

#include "DCLoad.h"



/*
 *	@brief Initializes load, sets ADC readout function and channel numbers
 *  @param [*params] Pointer to load configuration parameters and functions
 */
void Load_Initialize(load_config_t *params)
{
	load_actual_config.adcReadFunction = params->adcReadFunction;
	load_actual_config.adcInitFunction = params->adcInitFunction;
	load_actual_config.dacWriteFunction = params->dacWriteFunction;
	
	load_actual_config.currentChannel = params->currentChannel;
	load_actual_config.temperatureChannel = params->temperatureChannel;
	load_actual_config.voltageChannel = params->voltageChannel;
	
	load_actual_config.adcInitFunction();
	
}


/*
 *	@brief Executes a single cycle of load monitoring and regulation
 *	@param [*paramsPtr] Pointer to current load parameters
 *	@param [*settingsPtr] Pointer to load settings
 */
void Load_Run(load_params_t *paramsPtr, load_settings_t *settingsPtr)
{
	int32_t error = 0;
	paramsPtr->current = Load_Get_Current();
	paramsPtr->voltage = Load_Get_Voltage();
	paramsPtr->temperature = Load_Get_Temperature();
	paramsPtr->power = Load_Get_Power();
	


	
	if(((paramsPtr->temperature) > (settingsPtr->setMaxTemperature)) || ((paramsPtr->power) > (settingsPtr->setPower)) || ((paramsPtr->voltage) < (settingsPtr->setMinVoltage)))
	{
		settingsPtr->mode = LOAD_MODE_DISABLED;
	}

	
	if(settingsPtr->mode != LOAD_MODE_DISABLED)
	{
		if(settingsPtr->mode == LOAD_MODE_CC)
		{
			
#ifdef REGULATE			
			error = (settingsPtr->setCurrent - paramsPtr->current);
			
			dacVoltage = ((settingsPtr->setCurrent) ) * LOAD_SHUNT_OHMS;
			if(abs(error) > LOAD_CURRENT_HYSTERESIS)
			{
				if((paramsPtr->current) > (settingsPtr->setCurrent))
				{
					dacVoltage -= (error * LOAD_SHUNT_OHMS);
				}
				else if((paramsPtr->current) < (settingsPtr->setCurrent))
				{
					dacVoltage += (error * LOAD_SHUNT_OHMS);
				}
			}
			load_actual_config.dacWriteFunction(dacVoltage);
#endif
#ifndef REGULATE
		dacVoltage = ((settingsPtr->setCurrent) ) * LOAD_SHUNT_OHMS;
		load_actual_config.dacWriteFunction(dacVoltage);
#endif			
		}
		else if(settingsPtr->mode == LOAD_MODE_CP)
		{
			
		}
		else
		{
			//Error
			settingsPtr->mode = LOAD_MODE_DISABLED;
		}
	}
	else if(settingsPtr->mode == LOAD_MODE_DISABLED)
	{
		load_actual_config.dacWriteFunction(0);
	}	

}


/*
 *	@brief Gets current value at the time of function call.
 *	@return Current expressed in milliamperes. 
 */
uint32_t Load_Get_Current()
{
	uint32_t adcVoltage;
	adcVoltage = load_actual_config.adcReadFunction(load_actual_config.currentChannel);
	return (adcVoltage/LOAD_SHUNT_OHMS);	
}

/*
 *	@brief Gets voltage value at the time of function call.
 *	@return Voltage expressed in millivolts. 
 */
uint32_t Load_Get_Voltage()
{
	uint32_t adcVoltage;
	adcVoltage = load_actual_config.adcReadFunction(load_actual_config.voltageChannel)*LOAD_DIVISION_RATIO;
	return adcVoltage;
}

/*
 *	@brief Gets temperature value at the time of function call.
 *	@return Current expressed in Celcius. 
 */
uint32_t Load_Get_Temperature()
{
	uint32_t adcVoltage;
	adcVoltage = load_actual_config.adcReadFunction(load_actual_config.temperatureChannel);
	return adcVoltage/LOAD_MILLIVOLTS_PER_CELCIUS;
}

/*
 *	@brief Gets power value at the time of function call.
 *	@return Power expressed in milliwatts. 
 */
uint32_t Load_Get_Power()
{
	uint32_t voltage, current;
	uint32_t power;
	voltage = Load_Get_Voltage();
	current = Load_Get_Current();
	power = (current*voltage)/1000;
	return power;
}


