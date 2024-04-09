#ifndef HAL_RELAY_H_
#define HAL_RELAY_H_

/*--------------------------------------------------------------------------------------------*/
/*   SELECT MICROCONTROLERS PINS   */
/*--------------------------------------------------------------------------------------------*/
#define RELAY_PIN            PC7_PIN
/*--------------------------------------------------------------------------------------------*/
/*   SELECT PB_DEBOUNCING_TIME IN MILLI SECOND   */
/*--------------------------------------------------------------------------------------------*/
#define PUSH_BUTTON_DEBOUNCING_TIME    50

void Relay_voidInit(void);

void Relay_voidFlash(void);


#endif