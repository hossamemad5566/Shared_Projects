/***************************************
 * @author : Hossam Emad
 * @date : 22/3/2024
 * @SWC : FingerPrint 
 * @about :
****************************************/
#ifndef _FP_INTERFACE_H_
#define _FP_INTERFACE_H_
#include "FP_private.h"

#define PID_CMND    CMND_PACK
#define PID_DATA    DATA_PACK
#define PID_ACK     ACK_PACK
#define PID_END     END_PACK
/*LED*/
#define BREATHING       BREATHING_
#define FLASHING        FLASHING_
#define ON              ON_
#define OFF             OFF_
#define ON_GRAD         ON_GRAD_
#define OFF_GRAD        OFF_GRAD_

#define RED             RED_
#define BLUE            BLUE_
#define PURPLE          PURPLE_

#define FP_NOT_MATCHED    F_NOT_MATCHED


#define  CHAR_BUFFER_1  0X01
#define  CHAR_BUFFER_2  0X02


/****************************************
*1- u need to config the uart in 57600 
*2- set stop bits to one  
****************************************/
void FP_intit(void);

/*************************************************************
*1- copy_u8PID : PID_CMND   PID_DATA   PID_ACK    PID_END
*2- Copy_u16PackLength: array size (0>>256) (array + 2"check sum bits")
*3- copy_Pu8Pack : address of the message array
**************************************************************/
u8 FPSend(u8 copy_u8PID,u16 Copy_u16PackLength,u8* copy_Pu8Pack);

/*************************************************************
*1- if return zero >> every thing is okay
**************************************************************/
u8 FingerPS_handShake(void);

u8 FingerPS_genImg(void);
/*******************************************************
 * Copy_u8bufferID : CHAR_BUFFER_1        CHAR_BUFFER_2
*******************************************************/
u8 FingerPS_ConvertImg2CharFile(u8 Copy_u8bufferID);

/*******************************************************
 * Copy_u8bufferID : CHAR_BUFFER_1        CHAR_BUFFER_2
 * Copy_u16PageID   : 0x00 >> 0x15
*******************************************************/
u8 FingerPS_LoadCharFile(u8 Copy_u8bufferID,u16 Copy_u16PageID);

/*******************************************************
 * Copy_u8bufferID : CHAR_BUFFER_1        CHAR_BUFFER_2
 * Copy_u16PageID   : 0x00 >> 0x15
*******************************************************/
u8 FingerPS_StoreTemplate(u8 Copy_u8bufferID,u16 Copy_u16PageID);

/****************************************************************************************
 * copy_u8Control : BREATHING     FLASHING      ON      OFF     ON_GRAD     OFF_GRAD
 * copy_u8Speed :   val between 0x00 to 0xff
 * copy_u8Color:    RED     BLUE        PURPLE
 * copy_u8Cycles:   val between 1 to 255
*****************************************************************************************/
u8 FingerPS_Auraked(u8 copy_u8Control,u8 copy_u8Speed,u8 copy_u8Color,u8 copy_u8Cycles);

u8 FingerPS_genTemplate(void);

/*******************************************************
 * if function return 
 *      0x00              >> finger matched 
 *      FP_NOT_MATCHED >> not mathced
*******************************************************/
u8 FingerPS_Match(void);

/*******************************************************
 * To free the FP flash
*******************************************************/
u8 FP_voidMemoryFree(void);

/*******************************************************
 * Copy_u16StoreLoc : PageID   : 0x00 >> 0x15
*******************************************************/
void FP_setNewFinger(u16 Copy_u16StoreLoc);

/*******************************************************
 * Copy_u16StoreLoc : PageID   : 0x00 >> 0x15
*******************************************************/
u8 FP_CheckMatch(u16 Copy_u16StoreLoc);

#endif
