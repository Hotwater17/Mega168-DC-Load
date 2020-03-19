/*
 * ADC.c
 *
 * Created: 23.05.2019 20:34:33
 *  Author: Hotwater
 */ 

#include "ADC.h"

/*
 *	@brief Initializes ADC module
 */
void ADC_Init()
{
	PRR &= ~(1<<PRADC);
	DDRC &= ~(0x07);
	
	ADMUX = (1<<REFS0);
	ADCSRA = (1<<ADEN) | (1<<ADPS2) | (1<<ADPS0);
	ADCSRA |= (1<<ADSC);
}

/*
 *	@brief Reads specific ADC channel with averaging (16 samples per measurement)
 *	@param [channel]: channel to measure: 0-7
 *	@return voltage in millivolts
 */
uint32_t ADC_Get_Voltage(uint8_t channel)
{
	uint8_t sampleNumber = 0;
	uint32_t sampleAccumulator = 0;
	uint32_t voltage = 0;
	
	ADMUX = ((ADMUX & 0xF0) | (channel & 0x0F)); 
	for(sampleNumber = 0; sampleNumber < ADC_SAMPLE_NUMBER; sampleNumber++)
	{
		ADCSRA |= (1<<ADSC);
		while(!(ADCSRA & (1<<ADIF)))
		{
			//wait for conversion to finish
		}
		
		sampleAccumulator += ADC;
	}

	
	sampleAccumulator = (sampleAccumulator >> 4); //Divide by 16
	voltage = ((sampleAccumulator * ADC_VREF_mV)/ADC_RESOLUTION_STATES);
	return voltage;
	
	
}