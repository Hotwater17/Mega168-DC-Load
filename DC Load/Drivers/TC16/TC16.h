/*
 * TC16.h
 *
 * Created: 01.06.2019 18:01:39
 *  Author: Hotwater
 */ 

#include <avr/io.h>
#include <util/delay.h>

#ifndef TC16_H_
#define TC16_H_

#define TC16_CONFIG_1MS 1
#define TC16_CONFIG_10MS 2
#define TC16_CONFIG_100MS 3
#define TC16_CONFIG_1000MS 4

#define TC16_POLL_RUNNING 0
#define TC16_POLL_READY 1

void TC16_Init(uint8_t period);

uint8_t TC16_Poll();


#endif /* TC16_H_ */