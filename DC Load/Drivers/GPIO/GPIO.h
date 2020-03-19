/*
 * GPIO.h
 *
 * Created: 30.05.2019 17:08:43
 *  Author: Hotwater
 */ 

#include <avr/io.h>
#include <stdbool.h>

#ifndef GPIO_H_
#define GPIO_H_


#define GPIO_DIR_IN	0
#define GPIO_DIR_OUT 1

#define GPIO_STATE_LOW 0
#define GPIO_STATE_HIGH 1

#define GPIO_PULL_OFF 0
#define GPIO_PULL_ON 1

typedef struct
{
	uint8_t *port;
	uint8_t  *ddr;
	uint8_t pins;
	bool direction;
	bool pullup;
	
}gpio_config_t;

void GPIO_Init(gpio_config_t *configPtr);
void GPIO_Write(uint8_t *portx, uint8_t pin, bool state);
void GPIO_Toggle(uint8_t *portx, uint8_t pin);
uint8_t GPIO_Read(uint8_t *pinx, uint8_t pin);




#endif /* GPIO_H_ */