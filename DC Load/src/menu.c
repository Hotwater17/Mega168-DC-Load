/*
 * menu.c
 *
 * Created: 02.06.2019 18:23:06
 *  Author: Hotwater
 */ 

#include "menu.h"

/*
 *	@brief Switches to next parameter of load in menu
 *	@param [*modePtr] Pointer to mode variable
 */
void Menu_Change_Function(uint8_t *modePtr)
{
	++(*modePtr);
	if(*modePtr > 6) *modePtr = 0;
	
}

/*
 *	@brief Increments selected parameter of load in menu
 *	@param [*modePtr] Pointer to mode variable
 *	@param [*loadSettingsPtr] Pointer to load settings structure
 *	@param [*timePtr] Pointer to time structure
 */
void Menu_Increment(load_settings_t *loadSettingsPtr, time_t *timePtr, uint8_t *modePtr)
{
	switch(*modePtr)
	{
		case 0:
		{
			if(loadSettingsPtr->setCurrent < 250)
			{
				loadSettingsPtr->setCurrent += 10;
			}
			break;
		}
		case 1:
		{
			if(loadSettingsPtr->setMinVoltage < 12000)
			{
				loadSettingsPtr->setMinVoltage += 100;
			}
			break;
		}
		case 2:
		{
			if(loadSettingsPtr->setPower < 5000)
			{
				loadSettingsPtr->setPower += 100;
			}
			break;

		}
		case 3:
		{
			if(loadSettingsPtr->setMaxTemperature < 100)
			{
				loadSettingsPtr->setMaxTemperature++;
			}
			break;
		}
		case 4:
		{
			Time_Increment(timePtr, TIME_INCREMENT_MINUTES);
			break;
		}
		case 5:
		{
			Time_Increment(timePtr, TIME_INCREMENT_SECONDS);
			break;
		}
		case 6:
		{
			loadSettingsPtr->mode = LOAD_MODE_CC;
		}
		default: break;
	}
}

/*
 *	@brief Decrements selected parameter of load in menu
 *	@param [*modePtr] Pointer to mode variable
 *	@param [*loadSettingsPtr] Pointer to load settings structure
 *	@param [*timePtr] Pointer to time structure
 */
void Menu_Decrement(load_settings_t *loadSettingsPtr, time_t *timePtr, uint8_t *modePtr)
{
	switch(*modePtr)
	{
		case 0:
		{
			if(loadSettingsPtr->setCurrent > 10)
			{
				loadSettingsPtr->setCurrent -= 10;
			}
			break;
		}
		case 1:
		{
			if(loadSettingsPtr->setMinVoltage > 100)
			{
				loadSettingsPtr->setMinVoltage -= 100;
			}
			break;
		}
		case 2:
		{
			if(loadSettingsPtr->setPower > 100)
			{
				loadSettingsPtr->setPower -= 100;
			}
			break;
		}
		case 3:
		{
			if(loadSettingsPtr->setMaxTemperature > 30)
			{
				loadSettingsPtr->setMaxTemperature--;
			}
			break;
		}
		case 4:
		{
			Time_Decrement(timePtr, TIME_INCREMENT_MINUTES);
			break;
		}
		case 5:
		{
			Time_Decrement(timePtr, TIME_INCREMENT_SECONDS);
			break;
		}
		case 6:
		{
			loadSettingsPtr->mode = LOAD_MODE_DISABLED;
		}
		default: break;
	}
}

/*
 *	@brief Displays cursor under currently selected load parameter
 *	@param [*modePtr] Pointer to mode variable
 */
void Menu_Display_Cursor(uint8_t *modePtr)
{
	switch(*modePtr)
	{
		case 0:
		{
			HMI_Display_Cursor(0,0);
			break;
		}
		case 1:
		{
			HMI_Display_Cursor(8,0);
			break;
		}
		case 2:
		{
			HMI_Display_Cursor(0,1);
			break;
		}
		case 3:
		{
			HMI_Display_Cursor(7,1);
			break;
		}
		case 4:
		{
			HMI_Display_Cursor(7,1);
			break;			
		}
		case 5:
		{
			HMI_Display_Cursor(10,1);
			break;
		}
		case 6:
		{
			HMI_Display_Cursor(13,1);
			break;
		}
		default: break;
		
	}
}



