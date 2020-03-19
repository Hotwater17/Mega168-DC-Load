#include <avr/io.h>
#include <util/delay.h>
#include <stdbool.h>

#include "main.h"




int main(void)
{
	

	hmi_config_t hmi_init_config;
	
	hmi_init_config.buttonInitFunction = User_Button_Init;
	hmi_init_config.buttonReadFunction = User_Button_Read;
	
	dac_config_t dac_init_config;
	
	dac_init_config.dacInitFunction = User_DAC_I2C_Init;
	dac_init_config.dacReadFunction = User_DAC_I2C_Read;
	dac_init_config.dacWriteFunction = User_DAC_I2C_Write;
	
	load_config_t  load_init_config;
	
	load_init_config.adcInitFunction = User_ADC_Init;
	load_init_config.adcReadFunction = User_ADC_Read;
	load_init_config.dacWriteFunction = MCP4725_Set_Voltage_Fast;
	load_init_config.currentChannel = LOAD_CURRENT_CHANNEL;
	load_init_config.voltageChannel = LOAD_VOLTAGE_CHANNEL;
	load_init_config.temperatureChannel = LOAD_TEMPERATURE_CHANNEL;
	
	load_params_t loadData;
	load_settings_t loadSetValues;
	
	time_t globalTime;
	time_t loadActionTime;
	
	globalTime.milliseconds = 0;
	globalTime.seconds = 0;
	globalTime.minutes = 0;
	globalTime.hours = 0;
	
	loadActionTime.milliseconds = 0;
	loadActionTime.seconds = 30;
	loadActionTime.minutes = 0;
	loadActionTime.hours = 0;
	
	uint32_t voltageToWrite = 100;
	
	uint8_t menuMode = 0;
	
	
	Uart_Init(BAUD_REG_9600);
	
	TC16_Init(TC16_CONFIG_100MS);
	
	HMI_Initialize(&hmi_init_config);
	MCP4725_Initialize(&dac_init_config);
	Load_Initialize(&load_init_config);

	HMI_Display_Text("DC LOAD");	
	_delay_ms(500);
	
	loadSetValues.setMaxTemperature = 40;
	loadSetValues.setCurrent= 100;
	loadSetValues.setPower = 1000;
	loadSetValues.setMinVoltage = 500;
	loadSetValues.mode = LOAD_MODE_DISABLED;
	LCD_Clear();
	
	
	
	while (1) 
	{

		Load_Run(&loadData, &loadSetValues);
	
		//HMI_Is_Button_Pressed(BUT0);
		

		if(HMI_Is_Button_Pressed(BUT0) == BUTTON_PUSHED)
		{


			Menu_Decrement(&loadSetValues, &loadActionTime, &menuMode);	
			if(voltageToWrite > 100) voltageToWrite -= 100;
			
			_delay_ms(100); 
		}
		if(HMI_Is_Button_Pressed(BUT1) == BUTTON_PUSHED)
		{


			Menu_Increment(&loadSetValues, &loadActionTime, &menuMode);
			if(voltageToWrite < 5000) voltageToWrite += 100;

			
			_delay_ms(100);
		}
				
		if(HMI_Is_Button_Pressed(BUT2) == BUTTON_PUSHED)
		{
			if(loadSetValues.mode == LOAD_MODE_DISABLED)
			{
				Menu_Change_Function(&menuMode);
			}

			_delay_ms(100);
		}	
		
		//MCP4725_Set_Voltage_Fast(voltageToWrite);	
		//LCD_GoTo(0,0);
		//HMI_Display_Value(voltageToWrite);

		
		if(TC16_Poll() == TC16_POLL_READY)
		{
			HMI_Display_Clear();
			

				
		if(loadSetValues.mode == LOAD_MODE_DISABLED)
		{
			HMI_Display_Current(loadSetValues.setCurrent);
			HMI_Display_Power(loadSetValues.setPower);
			HMI_Display_Voltage(loadSetValues.setMinVoltage);
			HMI_Display_Temperature(loadSetValues.setMaxTemperature);
			HMI_Display_Mode(loadSetValues.mode);
			
			if((menuMode == 4) || (menuMode == 5))
			{
				HMI_Display_Time(&loadActionTime)	;
			}
			Menu_Display_Cursor(&menuMode);
			Time_Reset(&globalTime);
		
		}
		else if(loadSetValues.mode== LOAD_MODE_CC)
		{
			
			HMI_Hide_Cursor();	
			HMI_Display_Current(loadData.current);
			HMI_Display_Power(loadData.power);
			HMI_Display_Voltage(loadData.voltage);
			Serial_Send_Parameters(&loadData, &globalTime);	
			HMI_Display_Mode(loadSetValues.mode);	
			
			Time_Increment(&globalTime, TIME_INCREMENT_100MILLIS);
			if(((globalTime.seconds)%3) == 0)
			{
				HMI_Display_Temperature(loadData.temperature);
			}
			else
			{
				HMI_Display_Time(&globalTime);
			}
			
			
			if(Time_Compare(&globalTime, &loadActionTime) == TIME_MATCH)
			{
				loadSetValues.mode = LOAD_MODE_DISABLED;
				Serial_Send_Text(" Koniec pomiaru \r \n");
				Time_Reset(&globalTime);
			}
			
		}
		
		
			
		}		
	}
}

/*
 *	@brief DAC intereface initialization function for MCP4725 library
 */
void User_DAC_I2C_Init()
{
	i2c_init();
}
/*
 *	@brief DAC write function for MCP4725 library
 */
void User_DAC_I2C_Write(const uint8_t deviceAddress, const uint8_t registerAddress, const uint8_t data)
{
	uint8_t dataBuffer = data;
	i2c_writeReg(deviceAddress, registerAddress, &dataBuffer, 1);
}

/*
 *	@brief DAC read function for MCP4725 library
 */
uint8_t User_DAC_I2C_Read(const uint8_t deviceAddress, const uint8_t registerAddress)
{
	uint8_t dataBuffer = 0;
	i2c_readReg(dataBuffer, registerAddress, &dataBuffer, 1);
	return dataBuffer;
}

/*
 *	@brief ADC initialization function for DCLoad library
 */
void User_ADC_Init()
{
	ADC_Init();
}

/*
 *	@brief channel ADC read function for DCLoad library
 */
uint32_t User_ADC_Read(uint8_t channel)
{
	return ADC_Get_Voltage(channel);
}

/*
 *	@brief Button initialization function for HMI library
 */
void User_Button_Init()
{
	
	DDRD &= ~(1<<PD2) & ~(1<<PD3);
	DDRC &= ~(1<<PC3);
	PORTD |= (1<<PD2) | (1<<PD3);
	PORTC |= (1<<PC3);
	
	/*
	gpio_config_t but0_config;
	but0_config.direction = GPIO_DIR_IN;
	but0_config.pins = PD2;
	but0_config.pullup = GPIO_PULL_ON;
	but0_config.port = PORTD;
	but0_config.ddr = DDRD;
	
	GPIO_Init(&but0_config);
	
	gpio_config_t but1_config;
	but1_config.direction = GPIO_DIR_IN;
	but1_config.pins = PD3;
	but1_config.pullup = GPIO_PULL_ON;
	but1_config.port = PORTD;
	but1_config.ddr = DDRD;	
	
	GPIO_Init(&but1_config);
	
	gpio_config_t but2_config;
	but2_config.direction = GPIO_DIR_IN;
	but2_config.pins = PC3;
	but2_config.pullup = GPIO_PULL_ON;
	but2_config.port = PORTC;
	but2_config.ddr = DDRC;	
	
	GPIO_Init(&but2_config);
	*/
}

/*
 *	@brief Button read function for HMI library
 */
uint8_t User_Button_Read(uint8_t button)
{

	uint8_t state = BUTTON_RELEASED;
	/*
	if((button == BUT0_PIN) || (button == BUT1_PIN))
	{
		state = (PIND & (1<<button));
	}
	else if(button == BUT2_PIN)
	{
		state = (PINC & (1<<button));
	}*/
	switch(button)
	{
		case BUT0:
		{
			state = (PIND & (1<<BUT0_PIN));
			break;
		}
		case BUT1:
		{
			state = (PIND & (1<<BUT1_PIN));
			break;
		}
		case BUT2:
		{
			state = (PINC & (1<<BUT2_PIN));
			break;
		}	
		default: state = BUTTON_RELEASED;	
	}
	

	
	return state;

} 

/*
 *	@brief Sends out time, temperature, power, current and voltage readings via serial port
 */
void Serial_Send_Parameters(load_params_t *loadData, time_t *timeData)
{
	
	Uart_Send_Int32(timeData->hours);
	Uart_Send_Text(":");
	
	Uart_Send_Int32(timeData->minutes);
	Uart_Send_Text(":");
	
	Uart_Send_Int32(timeData->seconds);
	Uart_Send_Text(":");
	
	Uart_Send_Int32(timeData->milliseconds);
	Uart_Send_Text(",");	
		
	Uart_Send_Int32(loadData->temperature);
	Uart_Send_Text(",");
		
	Uart_Send_Int32(loadData->power);
	Uart_Send_Text(",");
		
	Uart_Send_Int32(loadData->current);
	Uart_Send_Text(",");
		
	Uart_Send_Int32(loadData->voltage);
	Uart_Send_Text("\r \n");	
}

/*
 *	@brief Sends text via serial interface
 */
void Serial_Send_Text(char *textPtr)
{
	Uart_Send_Text(textPtr);
}

/*
 *	@brief Increments time structure by certain value
 */
void Time_Increment(time_t *timePtr, uint8_t increment)
{
	switch(increment)
	{
		case TIME_INCREMENT_100MILLIS:
		{
			timePtr->milliseconds += 100;
			if(timePtr->milliseconds == 1000)
			{
				timePtr->milliseconds = 0;
				timePtr->seconds++;
				if(timePtr->seconds == 60)
				{
					timePtr->seconds = 0;
					timePtr->minutes++;
					if(timePtr->minutes == 60)
					{
						timePtr->minutes = 0;
						timePtr->hours++;
					}
				}
			}			
			break;			
		}
		case TIME_INCREMENT_SECONDS:
		{
			timePtr->seconds++;
			if(timePtr->seconds == 60)
			{
				timePtr->seconds = 0;
				timePtr->minutes++;
				if(timePtr->minutes == 60)
				{
					timePtr->minutes = 0;
					timePtr->hours++;
				}
			}			
			break;
		}
		case TIME_INCREMENT_MINUTES:
		{
			timePtr->minutes++;
			if(timePtr->minutes == 60)
			{
				timePtr->minutes = 0;
				timePtr->hours++;
			}			
			break;
		}
		default: break;
	}
}

/*
 *	@brief Decrements time structure by certain value
 */
void Time_Decrement(time_t *timePtr, uint8_t decrement)
{
	
	//TODO: this
	switch(decrement)
	{
		case TIME_INCREMENT_100MILLIS:
		{
			if(timePtr->milliseconds == 0)
			{
				timePtr->milliseconds = 999;
				if(timePtr->seconds == 0)
				{
					timePtr->seconds = 59;
					if(timePtr->minutes == 0)
					{
						timePtr->minutes = 59;
						if(timePtr->hours == 0)
						{
							timePtr->hours = 23;
						}
						else
						{
							timePtr->hours--;
						}
					}
					else
					{
						timePtr->minutes--;
					}
				}
				else
				{
					timePtr->seconds--;
				}
			}
			else
			{
				timePtr->milliseconds--;
			}
			break;
		}
		case TIME_INCREMENT_SECONDS:
		{
			
			if(timePtr->seconds == 0)
			{
				timePtr->seconds = 59;
				if(timePtr->minutes == 0)
				{
					timePtr->minutes = 59;
					if(timePtr->hours == 0)
					{
						timePtr->hours = 23;
					}
					else
					{
						timePtr->hours--;
					}
				}
				else
				{
					timePtr->minutes--;
				}
			}
			else
			{
				timePtr->seconds--;
			}
		

			break;
		}
		case TIME_INCREMENT_MINUTES:
		{

			if(timePtr->minutes == 0)
			{
				timePtr->minutes = 59;
				if(timePtr->hours == 0)
				{
					timePtr->hours = 23;
				}
				else
				{
					timePtr->hours--;
				}
			}
			else
			{
				timePtr->minutes--;
			}
		


			break;			
		}
		default: break;
	}
	
}

/*
 *	@brief Compares two time structures
 */
uint8_t Time_Compare(time_t* time1, time_t *time2)
{
	uint8_t result = TIME_NOMATCH;
	if(time1->hours == time2->hours)
	{
		if(time1->minutes == time2->minutes)
		{
			if(time1->seconds == time2->seconds)
			{
				result = TIME_MATCH;
			}
		}
	}
	
	return result;
}

/*
 *	@brief Resets time structure
 */
void Time_Reset(time_t *timePtr)
{
	timePtr->hours = 0;
	timePtr->minutes = 0;
	timePtr->seconds = 0;
	timePtr->milliseconds = 0;
}







/*
	DONE:
	-Comment the functions
	-Write a report(basically includes those two points below)
	-Draw system architecture
	-Draw out abstraction layer diagram
	-Add start and stop command
	-Add timer to count time of experiment and display it
	-Write LCD menu for displaying and setting values
	-Send readouts via serial port
	-Create User Functions files.
	-Add GPIO drivers
	-Implement HMI functions(display and buttons).
	-Implement Load functions(besides RUN)
	-Design system requirements
	-Create DAC IC library.
	-Import LCD library (Mirek or Radzio).
	-Add function pointers to HMI module
 */

/*
   OMMITED:
   -Read buttons with GPIO drivers(Sort of done but not exactly)
   -Import SD Card library (Petit Fat FS) - no point with this hardware
 */