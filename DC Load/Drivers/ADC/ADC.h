/*
 * ADC.h
 *
 * Created: 23.05.2019 20:34:26
 *  Author: Hotwater
 */ 

#include <avr/io.h>

#ifndef ADC_H_
#define ADC_H_

/* Defines */
#define ADC_VREF_mV 5000U
#define ADC_SAMPLE_NUMBER 16U
#define ADC_RESOLUTION_STATES 1024U
#define ADC_OFFSET_mV 34U

/* Function prototypes */
void ADC_Init();
uint32_t ADC_Get_Voltage(uint8_t channel);

#endif /* ADC_H_ */