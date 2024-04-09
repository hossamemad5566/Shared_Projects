#include "DIO_Interface.h"
#include "Relay.h"
#include <util/delay.h>

void Relay_voidInit(void)
{
    M_DIO_Void_SetPinDirection(RELAY_PIN,OUTPUT);
}

void Relay_voidFlash(void)
{
    M_DIO_Void_SetPinValue(RELAY_PIN,HIGH);
    _delay_ms(1000);
    M_DIO_Void_SetPinValue(RELAY_PIN,LOW);
    _delay_ms(1000);
}