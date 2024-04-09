/***************************************
 * @author : Hossam Emad
 * @date : 22/3/2024
 * @SWC : FingerPrint 
 * @about :
****************************************/
#ifndef _FP_PRIVATE_H_
#define _FP_PRIVATE_H_

void FPReceiveACK(u8 copy_u8code);

void FP_checkintro(void);

void test (void);

#define     CMND_PACK   0x01
#define     DATA_PACK   0x02
#define     ACK_PACK    0x07
#define     END_PACK    0x08

#define BREATHING_       0X01
#define FLASHING_        0X02
#define ON_              0X03
#define OFF_             0X04
#define ON_GRAD_         0X05
#define OFF_GRAD_        0X06

#define RED_             0X01
#define BLUE_            0X02
#define PURPLE_          0X03

#define F_NOT_MATCHED    0X40

#define DEBUGGING       0

#endif
