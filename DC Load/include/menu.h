/*
 * menu.h
 *
 * Created: 02.06.2019 18:22:59
 *  Author: Hotwater
 */ 

#include <avr/io.h>

#include "main.h"

#ifndef MENU_H_
#define MENU_H_


void Menu_Change_Function(uint8_t *modePtr);
void Menu_Increment(load_settings_t *loadSettingsPtr, time_t *timePtr, uint8_t *modePtr);
void Menu_Decrement(load_settings_t *loadSettingsPtr, time_t *timePtr, uint8_t *modePtr);
void Menu_Display_Cursor(uint8_t *modePtr);

#endif /* MENU_H_ */