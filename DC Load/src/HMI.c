/*
 * HMI.c
 *
 * Created: 13.04.2019 23:17:44
 *  Author: Hotwater
 */ 

#include "HMI.h"



/*
 *	@brief Initializes HMI module
 *	@param [*params] pointer to HMI configuration structure
 */
void HMI_Initialize(hmi_config_t *params)
{
	hmi_actual_config.buttonInitFunction = params->buttonInitFunction;
	hmi_actual_config.buttonReadFunction = params->buttonReadFunction;
	
	hmi_actual_config.buttonInitFunction();
	
	LCD_Initalize();
}

/*
 *	@brief Displays current on LCD
 *	@param [currentInMilliamperes] current to display in mA
 */
void HMI_Display_Current(uint32_t currentInMilliamperes)
{
	LCD_GoTo(0, 0);
	HMI_Display_Value(currentInMilliamperes);
	HMI_Display_Text("mA");
}

/*
 *	@brief Displays voltage on LCD
 *	@param [voltageInMillivolts] voltage to display in mV
 */
void HMI_Display_Voltage(uint32_t voltageInMillivolts)
{
	LCD_GoTo(8, 0);
	HMI_Display_Value(voltageInMillivolts);
	HMI_Display_Text("mV");
}

/*
 *	@brief Displays power on LCD
 *	@param [powerInMilliwatts] power to display in mW
 */
void HMI_Display_Power(uint32_t powerInMilliwatts)
{
	LCD_GoTo(0, 1);
	HMI_Display_Value(powerInMilliwatts);
	HMI_Display_Text("mW");	
}

/*
 *	@brief Displays temperature on LCD
 *	@param [temperatureInCelcius] temperature to display in C
 */
void HMI_Display_Temperature(uint32_t temperatureInCelcius)
{
	LCD_GoTo(7, 1);
	HMI_Display_Value(temperatureInCelcius);
	HMI_Display_Text("C");	
}

/*
 *	@brief Displays time on LCD
 *	@param [*timePtr] pointere to time structure to display
 */
void HMI_Display_Time(time_t *timePtr)
{
	HMI_Display_Locate(7,1);
	HMI_Display_Value(timePtr->minutes);
	HMI_Display_Text(":");
	if((timePtr->seconds) < 10) 
	{
		HMI_Display_Value(0);
	}
	HMI_Display_Value(timePtr->seconds);	
}

/*
 *	@brief Shows cursor on display in specific place
 *	@param [x] column of display
 *	@param [y] row of display
 */
void HMI_Display_Cursor(uint8_t x, uint8_t y)
{
	LCD_GoTo(x, y);
	LCD_WriteCommand(0x0E);
}

/*
 *	@brief Hides cursor on display
 */
void HMI_Hide_Cursor()
{
	LCD_WriteCommand(0x0C);
}
/*
 *	@brief Moves cursor to given coordinates on display
 *	@param [x] column of display
 *	@param [y] row of display
 */
void HMI_Display_Locate(uint8_t x, uint8_t y)
{
	LCD_GoTo(x, y);
}

/*
 *	@brief Clears display
 */
void HMI_Display_Clear()
{
	LCD_Clear();
}

/*
 *	@brief Displays mode of load
 *	@param [currentMode] mode of load to display
 */
void HMI_Display_Mode(uint8_t currentMode)
{
	switch(currentMode)
	{	
		

		case 0U:
		{
			LCD_GoTo(13,1);
			HMI_Display_Text("OFF");
			break;
		}
		case 1U:
		{
			LCD_GoTo(13,1);
			HMI_Display_Text("CC");
			break;
		}
		case 2U:
		{
			LCD_GoTo(13,1);
			HMI_Display_Text("CP");
			break;
		}			
		default:
		{
			LCD_GoTo(13,1);
			HMI_Display_Text("ERR");
			break;
		}	
	}


}

/*
 *	@brief Displays number in current cursor location
 *	@param [value] number to display
 */
void HMI_Display_Value(uint32_t value)
{
	itoa(value, txtBuffer, 10);
	LCD_WriteText(txtBuffer);
}

/*
 *	@brief Displays text in current cursor location
 *	@param [*textPtr] pointer to text to display
 */
void HMI_Display_Text(char *textPtr)
{
	LCD_WriteText(textPtr);
}

/*
 *	@brief Checks if specific button was pressed
 *	@param [buttonNumber] number of button to check
 *	@return state of the button (0 - pressed, 1 - released)
 */
bool HMI_Is_Button_Pressed(uint8_t buttonNumber)
{
	return hmi_actual_config.buttonReadFunction(buttonNumber);
}