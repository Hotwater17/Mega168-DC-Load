/*
 * GPIO.c
 *
 * Created: 30.05.2019 17:08:36
 *  Author: Hotwater
 */ 

#include "GPIO.h"

/*
 *	@brief Initializes gpio pin(s)
 *	@param [*configPtr]: pointer to configuration structure
 */
void GPIO_Init(gpio_config_t *configPtr)
{
	configPtr->ddr = ((configPtr->direction) << (configPtr->pins));
	configPtr->port = ((configPtr->pullup) << (configPtr->pins));
}

/*
 *	@brief Writes state to pin(s) in specific port
 *	@param [port]: port which selected pin is a part of
 *	@param [pin]: pin to write
 *	@param [state]: state to write
 */
void GPIO_Write(uint8_t *portx, uint8_t pin, bool state)
{
	*portx = (state<<pin);
}

/*
 *	@brief Toggle pin(s) in specific port
 *	@param [port]: port which selected pin is a part of
 *	@param [pin]: pin to toggle
 */
void GPIO_Toggle(uint8_t *portx, uint8_t pin)
{
	*portx |= (1<<pin);
}

/*
 *	@brief Reads state from pin in specific port
 *	@param [port]: port which selected pin is a part of
 *	@param [pin]: pin to read from
 *	@return: state of pin 
 */
uint8_t GPIO_Read(uint8_t *pinx, uint8_t pin)
{
	return (*(pinx) & (1<<pin));
}