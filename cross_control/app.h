/*
 * app.h
 *
 *  Created on: Mar 4, 2024
 *      Author: Anwar
 */

#ifndef APP_H_
#define APP_H_

#include "BitMath.h"
#include "STD.h"
#include "math.h"

#include <avr/io.h>
#include <util/delay.h>


#include "DIO_Interface.h"
#include "LED_Interface.h"
#include "LCD_Interface.h"
#include "EXTINT_Interface.h"
#include "GIE_Interface.h"
#include "PushButton_Interface.h"
#include "Buzzer_Interface.h"
#include "ADC_Interface.h"
#include "Relay.h"
#include "Keypad_Interface.h"
#include "Timer_Interface.h"


void timer_sec(void);

void Fuel_Padel(void);

void Brake_Padel(void);

void CCS_operating(void);

void CCS_States(void);

void GB_Shift(void);

u16 ADC_u16Mapping_mm(void);

void DangerSate(void);

void BAS(void);

void LCD_Write_Speed(void);

void speed_limiter(void);

void LCD_pageFlip(void);

void DM(void);

void distanceIncrement(void);

void distanceDisp(void);

void Update_Time(void);

void Void_Write_Time(void);

void Void_Write_Date_Lcd(void);

#endif /* APP_H_ */
