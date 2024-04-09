#include "app.h"
#define DANGER 0
#define CCSON  (u8*)"CCS ON "
#define CCSOFF (u8*)"CCS OFF"
#define FUEL     PUSH_BUTTON_0
#define BRAKE    PUSH_BUTTON_1
#define CCS      PUSH_BUTTON_2
#define GB       PUSH_BUTTON_3
#if (DANGER)
typedef union 
{
    struct
    {
        u8 Dist___10 :1;
        u8 Dist_8_10 :1;
        u8 Dist_6_8  :1;
        u8 Dist_4_6  :1;
        u8 Dist_2_4  :1;
        u8 Dist_0_2  :1;     
    };
    u8 AllFlags;
}DangerFlag_t;
  
DangerFlag_t DangerFlag;    
#endif
/*global var for gear */
u8 gear_ArrState[3]={'N','D','R'};
static u8 gearState = 0;
u16 car_danger = 0;
/*setup*/
void setup (void)
{
    gearState = gear_ArrState[0];
    //GIE_voidEnable();/*enable gie*/
    H_Lcd_Void_LCDInit();
    H_Buzzer_Void_BuzzerInit();
    M_ADC_Void_ADCInit();
    H_LED_Void_LedInit(LED_YEL);
    H_LED_Void_LedInit(LED_BLU);
    H_LED_Void_LedInit(LED_GRN);
    H_LED_Void_LedInit(LED_RED);
    H_PushButton_Void_PushButtonInit(FUEL);
    H_PushButton_Void_PushButtonInit(BRAKE);
    H_PushButton_Void_PushButtonInit(CCS);
    H_PushButton_Void_PushButtonInit(GB);
    Relay_voidInit();

    #if (DANGER)
    DangerFlag.AllFlags=0;
    #endif
    H_Lcd_Void_LCDGoTo(0,2);
    H_Lcd_Void_LCDWriteString((u8*)"CCS OFF ");
    H_Lcd_Void_LCDGoTo(1,2);
    H_Lcd_Void_LCDWriteString((u8*)"R N D : N");
}
/*loop*/
int main (void)
{
    setup();
        while(1)
        {
            #if (DANGER)
            DangerSate();
            #endif
            Fuel_Padel();
            Brake_Padel();
            if(!H_PushButton_U8_PushButtonRead(CCS)){CCS_States();}
            if(!H_PushButton_U8_PushButtonRead(GB)){GB_Shift();}

        }
}

void Fuel_Padel(void)
{
    /*if((H_PushButton_U8_PushButtonRead(FUEL)==PUSH_BUTTON_PRESSED)&&(DangerFlag.AllFlags <= 2))*/
    if((H_PushButton_U8_PushButtonRead(FUEL)==PUSH_BUTTON_PRESSED))
    {
        H_LED_Void_LedOn(LED_BLU);
        CCS_OFF();
    }
    else
    {
        H_LED_Void_LedOff(LED_BLU);
    }

}

void Brake_Padel(void){
    /*if((H_PushButton_U8_PushButtonRead(BRAKE)==PUSH_BUTTON_PRESSED)||(DangerFlag.AllFlags > 2))*/
    if(H_PushButton_U8_PushButtonRead(BRAKE)==PUSH_BUTTON_PRESSED)
    {
        H_LED_Void_LedOn(LED_RED);
    }
    else
    {
        H_LED_Void_LedOff(LED_RED);
    }
}

void CCS_ON(void)
{
    H_LED_Void_LedOn(LED_GRN);
    H_Lcd_Void_LCDGoTo(0,2);
    H_Lcd_Void_LCDWriteString(CCSON);
}

void CCS_OFF(void)
{
    H_LED_Void_LedOff(LED_GRN);
    H_Lcd_Void_LCDGoTo(0,2);
    H_Lcd_Void_LCDWriteString(CCSOFF);
}

void CCS_States(void){

    
    static u8 CCS_State = 0;
    
    H_Buzzer_Void_BuzzerOnce();
    /*change the CCS state if the state of GB is : D */
    if(gearState==gear_ArrState[1])
    {
        /*tog the state*/
        CCS_State ^= 1;
        /*if((CCS_State==1)&&(DangerFlag.AllFlags<=2))*/
        if(CCS_State==1)
        {       
            CCS_ON();
        }
        else
        {
            CCS_OFF();
        }
        /*freez the state untill the user release the button */
        while (!H_PushButton_U8_PushButtonRead(CCS));   
    }
    else
    {
        /*if the CCS button pressed and the GB state not D : produce alarm */
        while (!H_PushButton_U8_PushButtonRead(CCS))
            {
                H_LED_Void_LedOn(LED_YEL);
            }
            H_LED_Void_LedOff(LED_YEL);
    }

}

void GB_Shift(void){
    static u8 counter = 0;
    /*buzz with press*/
    H_Buzzer_Void_BuzzerOnce();
    if((!H_PushButton_U8_PushButtonRead(GB)))
    {
        /*change the gearbox state only if the brake is pressed */
        if(!H_PushButton_U8_PushButtonRead(BRAKE))
        {
            /*we have 3 state of iterate through them using counter*/
            if(counter==2)
                {counter=0;}
            else
            {counter++;}
            /*save the gear state in a global var */
            gearState = gear_ArrState[counter];
            H_Lcd_Void_LCDGoTo(1,10);
            H_Lcd_Void_LCDWriteCharacter(gearState);
            /*if GB state != D turn off the CCS */
            if(gearState != gear_ArrState[1])
            {
                CCS_OFF();
            }
            while (!H_PushButton_U8_PushButtonRead(GB));
              
        }
        else
        {
            /*if the GB button pressed and the BRAKE not pressed : produce alarm */

            while (!H_PushButton_U8_PushButtonRead(GB))
            {
                H_LED_Void_LedOn(LED_YEL);
            }
            H_LED_Void_LedOff(LED_YEL);
        }
       
    }
    
}

u16 ADC_u16Mapping_mm(void)
{
    u16 Local_u16GetADC =0;
    /*read the ADC value*/
    Local_u16GetADC = M_ADC_U16_ADCRead(ADC_CHANNEL_0);
    /*map it from 0>> 1023 to 0 >> 10 m (* 1000) to aviod float operation  */
    return (Local_u16GetADC * 10 / 1023);
}

#if (DANGER)
void DangerSate(void)
{
    u8 Local_u8Reading;
    /*test if we in D mode */
    if(gearState == gear_ArrState[1])
    {
        /*read the adc*/
        Local_u8Reading = ADC_u16Mapping_mm();
        DangerFlag.AllFlags =  0;
		if(Local_u8Reading>=10)
		{
			DangerFlag.Dist___10 = 1;
			H_Lcd_Void_LCDGoTo(2,0);
			H_Lcd_Void_LCDWriteString("           ");
            Brake_Padel();
		}
		else if((Local_u8Reading < 10)&&(Local_u8Reading >2) )
        {

			H_Lcd_Void_LCDGoTo(2,0);
            H_Lcd_Void_LCDWriteString("BAS =");
            H_Lcd_Void_LCDGoTo(2,8);
            H_Lcd_Void_LCDWriteNumber(Local_u8Reading);
            H_Lcd_Void_LCDGoTo(3,0);
            H_Lcd_Void_LCDWriteString("      ");
            /*test the ranges and set the flags*/

            if((Local_u8Reading<10)&&(Local_u8Reading>=8))
            {
                DangerFlag.Dist_8_10 = 1;
                H_LED_Void_LedOff(LED_YEL);
                H_LED_Void_LedOff(LED_RED);


            }
            else if((Local_u8Reading<8)&&(Local_u8Reading>=6))
            {
                DangerFlag.Dist_6_8 =  1;
                CCS_OFF();

            }
            else if((Local_u8Reading<6)&&(Local_u8Reading>=4))
            {
                DangerFlag.Dist_4_6 =  1;
                CCS_OFF();  H_LED_Void_LedOn(LED_YEL); 
            }
            else if((Local_u8Reading<4)&&(Local_u8Reading>2))
            {
                DangerFlag.Dist_2_4 =  1;
                CCS_OFF();  H_LED_Void_LedBlink(LED_YEL);   
            }
        }
        else if((Local_u8Reading<=2)&&(Local_u8Reading>=0))
        {
            DangerFlag.Dist_0_2 =  1;
            H_LED_Void_LedOff(LED_YEL);
            H_LED_Void_LedOff(LED_RED);
            CCS_OFF();  
            Relay_voidFlash();
            H_Lcd_Void_LCDGoTo(2,0);
            H_Lcd_Void_LCDWriteString("BAS =");
            H_Lcd_Void_LCDGoTo(2,8);
			H_Lcd_Void_LCDWriteNumber(Local_u8Reading);
            H_Lcd_Void_LCDGoTo(3,0);
            H_Lcd_Void_LCDWriteString("ACC!!!");
        }
    }
    else
    {
        H_Lcd_Void_LCDGoTo(2,0);
        H_Lcd_Void_LCDWriteString("           ");
    }
    
}
#endif

