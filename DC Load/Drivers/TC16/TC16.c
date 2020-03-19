/*
 * TC16.c
 *
 * Created: 01.06.2019 18:01:32
 *  Author: Hotwater
 */ 

#include "TC16.h"

/*
 *	@brief Initializes 16-bit Timer/Counter 1 for defined periods
 *	@param [period]: Period of timer ticks
 */
void TC16_Init(uint8_t period)
{
	PRR &= ~(1 << PRTIM1);
	switch (period)
	{
		case TC16_CONFIG_1MS:
		{
			TCCR1B = (1 << WGM12) | (1 << CS11);
			OCR1A = 999;
			break;
		}
		
		case TC16_CONFIG_10MS:
		{
			TCCR1B = (1 << WGM12) | (1 << CS11);
			OCR1A = 9999;
			break;
		}
		
		case TC16_CONFIG_100MS:
		{
			TCCR1B = (1 << WGM12) | (1 << CS11) | (1 << CS10);
			OCR1A = 12499;
			break;
		}
		
		case TC16_CONFIG_1000MS:
		{
			TCCR1B = (1 << WGM12) | (1 << CS12) | (1 << CS11);
			OCR1A = 31249;	
			break;		
		}
		
		default: break;
	}
	
	TIMSK1 |= (1<<OCIE1A);

	
	
}

/*
 *	@brief Checks if timer finished counting 
 *	@return Readiness of timer (0 - counting not finished, 1 - counting finished)
 */
uint8_t TC16_Poll()
{
	uint8_t state = TC16_POLL_RUNNING;

	if(TIFR1 & (1<<OCF1A))
	{
		state = TC16_POLL_READY;
		TIFR1 |= (1<<OCF1A);
	}
	

	return state;
}