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




void Fuel_Padel(void);
void Brake_Padel(void);
void CCS_ON(void);
void CCS_OFF(void);
void CCS_States(void);
void GB_Shift(void);
u16 ADC_u16Mapping_mm(void);
void DangerSate(void);



#endif /* APP_H_ */
