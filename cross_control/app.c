#include "app.h"
#define DANGER 0
#define V0 0
#define SPEED_TOLERANCE     10
#define CCSON  (u8*)"CCS ON "
#define CCSOFF (u8*)"CCS OFF"
#define FUEL     PUSH_BUTTON_0
#define BRAKE    PUSH_BUTTON_1
#define CCS      PUSH_BUTTON_2
#define GB       PUSH_BUTTON_3
#define GB_WITHOUT_INT      0

#if (V0)
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
//u16 car_danger = 0; 
DangerFlag_t DangerFlag;    
#endif

/*global var for gear */
u8 gear_ArrState[3]={'N','D','R'};
static u8 gearState = 'N';
static u8 BASState = 0;
u8 CCS_u8State = 0;
u8 speed_u8Capture=0;
static u8 speedLimiter_Status = 0;
static u8 speedlimit = 100;
u8 speedflag = 0;
u8 change_u8Flag=0;
static u8 screen_u8Page =1;

u16 dist=0;
u8 Dist_inc=0;
/*time*/
u8 hours=10;
u8 mins=30;
u8 sec=0;

/*setup*/
void setup (void)
{
    gearState = gear_ArrState[0];
    GIE_voidEnable();/*enable gie*/
    /*exti1 init*/
    M_EXTINT_Void_EXTINTInit(INT1_CHANNEL);
    M_EXTINT_Void_SetCallBack(INT1_CHANNEL,GB_Shift);
    
    H_Lcd_Void_LCDInit();
    H_Buzzer_Void_BuzzerInit();
    M_ADC_Void_ADCInit();
    Relay_voidInit();
    H_LED_Void_LedInit(LED_YEL);
    H_LED_Void_LedInit(LED_GRN);
    H_LED_Void_LedInit(LED_RED);
    H_KeyPad_Void_KeyPadInit();

    /*timer init */
        /*timer init */
    M_Timer_Void_TimerInit();
    M_Timer_Void_TimerSetTime(1000);
    M_Timer_Void_SetCallBack(timer_sec);
    M_Timer_Void_TimerStart(TIMER0_CHANNEL);
    /*init lcd*/
    H_Lcd_Void_LCDGoTo(0,0);
    H_Lcd_Void_LCDWriteString("******* GB:  *******");
    H_Lcd_Void_LCDGoTo(0,11);
    H_Lcd_Void_LCDWriteCharacter(gearState);
    H_Lcd_Void_LCDGoTo(1,0);
    H_Lcd_Void_LCDWriteString("*                  *");
    H_Lcd_Void_LCDGoTo(3,0);
    H_Lcd_Void_LCDWriteString("*******( /4)********");

    #if (V0)
    DangerFlag.AllFlags=0;
    //H_LED_Void_LedInit(LED_BLU);
    //h_pushbutton_void_pushbuttoninit(fuel);
    //h_pushbutton_void_pushbuttoninit(brake);
    //h_pushbutton_void_pushbuttoninit(ccs);
    //h_PushButton_Void_PushButtonInit(GB);
    #endif
}
/*loop*/
int main (void)
{
    setup();
	while(1)
	{
		#if (V0)
		DangerSate();
		#endif
        LCD_pageFlip();
		Brake_Padel();
        BAS();
        LCD_Write_Speed();
        speed_limiter();
        CCS_States();
        CCS_operating();
        Void_Write_Time();
        distanceIncrement();
        distanceDisp();
	}
}

#if(V0)
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
#endif

void distanceIncrement(void)
{
    if(gearState=='D')
    {
        u16 speed = ADC_u16Mapping_mm();
        if(speed == 0 )
        {
            Dist_inc = 0;
        }
        else if((speed >0)&&(speed <40))
        {
            Dist_inc = 1;
        }
        else if(speed >= 40 && speed < 80)
        {
            Dist_inc = 2;
        }
        else if(speed >= 80 && speed < 120)
        {
            Dist_inc = 3;
        }
        else if(speed >= 120 && speed < 160)
        {
            Dist_inc = 4;
        }
        else if(speed >= 160 && speed <= 200)
        {
            Dist_inc = 5;
        }
    }
    else
    {
        Dist_inc =0;
    }
}

void distanceDisp(void)
{
    if(screen_u8Page == 4)
    {
        H_Lcd_Void_LCDGoTo(2,1);
        H_Lcd_Void_LCDWriteString("Distance=");
        H_Lcd_Void_LCDGoTo(2,10);
        H_Lcd_Void_LCDWriteNumber(dist);
    }
}

void timer_sec(void)
{
    if(speedflag)
    {
        Relay_voidFlash();
    }
    else
    {
        Relay_voidOff();
    }
    dist +=Dist_inc;
    /*for dm*/
    change_u8Flag++;
    DM();
    /*time update*/
    Update_Time();
}

void LCD_pageFlip(void)
{
    static u8 flag = 1;/*for initial state of proj*/
    if(H_KeyPad_U8_KeyPadRead(3)== 'R')
    {
        if(screen_u8Page == 4)
        {screen_u8Page =1;}
        else
        {screen_u8Page++;}
        flag= 1;
        while(H_KeyPad_U8_KeyPadRead(3)== 'R');
    }
    if(H_KeyPad_U8_KeyPadRead(3)== 'L')
    {
        if(screen_u8Page == 1)
        {screen_u8Page = 4;}
        else
        {screen_u8Page--;}
        flag =1;
        while(H_KeyPad_U8_KeyPadRead(3)== 'L');
    }
    if(flag == 1)
    {
        H_Lcd_Void_LCDGoTo(1,0);
        H_Lcd_Void_LCDWriteString("*                  *"); 
        H_Lcd_Void_LCDGoTo(2,0);
        H_Lcd_Void_LCDWriteString("*                  *"); 
        /*in gear function*/
        H_Lcd_Void_LCDGoTo(3,8);
        H_Lcd_Void_LCDWriteNumber(screen_u8Page);
        flag =0;
    }
}

void BAS(void)
{
    if(H_KeyPad_U8_KeyPadRead(0)== 'S')
    {
        if(gearState == 'D')
        {
            BASState= !BASState;
            while (H_KeyPad_U8_KeyPadRead(0)=='S');

        }
        else
        {
            while (H_KeyPad_U8_KeyPadRead(0)=='S')
            {
                H_LED_Void_LedOn(LED_YEL);
            }
            H_LED_Void_LedOff(LED_YEL);
        }
    }
    if(gearState != 'D')
    {

        BASState=0;
    }
  
    if((screen_u8Page == 3))
    {
        H_Lcd_Void_LCDGoTo(2,5);
        H_Lcd_Void_LCDWriteString("BAS:");
        H_Lcd_Void_LCDGoTo(2,11);
        if(BASState==1)
        {H_Lcd_Void_LCDWriteString("ON ");}
        else 
        {H_Lcd_Void_LCDWriteString("OFF");}

    }
}

void Brake_Padel(void)
{
    /*if((H_PushButton_U8_PushButtonRead(BRAKE)==PUSH_BUTTON_PRESSED)||(DangerFlag.AllFlags > 2))*/
    if(H_KeyPad_U8_KeyPadRead(0)== 'B')
    {
        H_LED_Void_LedOn(LED_RED);
    }
    else
    {
        H_LED_Void_LedOff(LED_RED);
    }
}

void CCS_operating(void)
{
    u8 flag=1;
    if(CCS_u8State==1)
    {
        u8 local_u8Speed=0;
        local_u8Speed=ADC_u16Mapping_mm();
        if((local_u8Speed > (speed_u8Capture + SPEED_TOLERANCE)) ||(local_u8Speed < (speed_u8Capture - SPEED_TOLERANCE)) )
        {
            CCS_u8State=0;
            flag =1;
        }
    }
    if(screen_u8Page==2)
    {
        if(flag)
        {
            flag=0;
            H_Lcd_Void_LCDGoTo(2,4);
            H_Lcd_Void_LCDWriteString("CCS : ");
            H_Lcd_Void_LCDGoTo(2,11);
            if(CCS_u8State)
            {H_Lcd_Void_LCDWriteString("ON ");}
            else
            {H_Lcd_Void_LCDWriteString("OFF");}
            
        }
    }
    
}

void CCS_States(void)
{
    u8 flag =1;
    if(H_KeyPad_U8_KeyPadRead(1)=='O')
    {
        
        _delay_ms(10);
        H_Buzzer_Void_BuzzerOnce();
        /*change the CCS state if the state of GB is : D */
        if(gearState==gear_ArrState[1])
        {
            /*tog the state*/
            CCS_u8State ^= 1;
            if(CCS_u8State==1)
            {       
                speed_u8Capture = ADC_u16Mapping_mm();
                flag =1;
            }
            /*freez the state untill the user release the button */
            while (H_KeyPad_U8_KeyPadRead(1)=='O');   
        }
        else
        {
            /*if the CCS button pressed and the GB state not D : produce alarm */
            while (H_KeyPad_U8_KeyPadRead(1)=='O')
                {
                    H_LED_Void_LedOn(LED_YEL);
                }
                H_LED_Void_LedOff(LED_YEL);
        }
    }
    if(screen_u8Page==2)
    {
        if(flag)
        {
            flag=0;
            H_Lcd_Void_LCDGoTo(2,4);
            H_Lcd_Void_LCDWriteString("CCS : ");
            H_Lcd_Void_LCDGoTo(2,11);
            if(CCS_u8State)
            {H_Lcd_Void_LCDWriteString("ON ");}
            else
            {H_Lcd_Void_LCDWriteString("OFF");}
            
        }
    }

}

#if(!GB_WITHOUT_INT)

void GB_Shift(void)
    {
    static u8 counter = 0;
    u16 local_u16CNT =0;
    if(H_KeyPad_U8_KeyPadRead(0)== 'B')
    {
        H_Buzzer_Void_BuzzerOnce();
        /*we have 3 state of iterate through them using counter*/
        if(counter==2)
            {counter=0;}
        else
        {counter++;}
        /*save the gear state in a global var */
        gearState = gear_ArrState[counter];
        H_Lcd_Void_LCDGoTo(0,11);
        H_Lcd_Void_LCDWriteCharacter(gearState);
        /*if GB state != D turn off the CCS */
        if(gearState != gear_ArrState[1])
        {
            CCS_u8State=0;
        }
        while ((H_KeyPad_U8_KeyPadRead(0)== 'B')&&local_u16CNT <3000)
        {local_u16CNT++;H_LED_Void_LedOn(LED_RED);}

            
    }
    else 
    {
        while (H_KeyPad_U8_KeyPadRead(0)== 'I')
        {
            H_LED_Void_LedOn(LED_YEL);
        }
        H_LED_Void_LedOff(LED_YEL);

    }       
}
#endif


#if(GB_WITHOUT_INT)
void GB_Shift(void){
    static u8 counter = 0;
    if(H_KeyPad_U8_KeyPadRead(0)== 'C')
    {
        H_Buzzer_Void_BuzzerOnce();
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
            CCS_u8State=0;
        }
        while (H_KeyPad_U8_KeyPadRead(0)=='C');
            
    }
    else if(H_KeyPad_U8_KeyPadRead(0)== 'G')
    {
        while (H_KeyPad_U8_KeyPadRead(0)== 'G')
        {
            H_LED_Void_LedOn(LED_YEL);
        }
        H_LED_Void_LedOff(LED_YEL);

    }       
}
#endif

#if (V1)
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

void speed_limiter(void)
{
    u8 flag=1;
    u16 speed = ADC_u16Mapping_mm();
    /*system on off*/
    if(H_KeyPad_U8_KeyPadRead(2) == 'P')
    {
        if(gearState == 'D')
        {
            speedLimiter_Status = !speedLimiter_Status; // Activate speed limiter
            while(H_KeyPad_U8_KeyPadRead(2) == 'P');
        }
        else
        {
            while (H_KeyPad_U8_KeyPadRead(2)=='P')
            {
                H_LED_Void_LedOn(LED_YEL);
            }
            H_LED_Void_LedOff(LED_YEL);
        }
        
    }
    /*up the speed limiter*/
    if(H_KeyPad_U8_KeyPadRead(2) == 'U')
    {
        if(speedlimit <200)
        {
            speedlimit += 5;
            _delay_ms(10);
            flag=1;
        }
    }
    /*down the speed limiter*/
    if(H_KeyPad_U8_KeyPadRead(2) == 'D'){
        if(speedlimit > 0)
        {
            speedlimit -= 5;
            _delay_ms(10);
            flag=1;
        }
    }
    
    if(gearState != 'D')
    {
        speedLimiter_Status=0;
        flag =1;
    }
    if(speedLimiter_Status == 1){
        
        if (speed >= speedlimit)
        {
            speedflag = 1;
        }
        else
        {
            speedflag =0;
        }
    }
    else
    {
        speedflag =0;
        M_DIO_Void_SetPinValue(RELAY_PIN,0);
    }
    
    /*display */
    if(screen_u8Page == 1)
    {
        if(flag)
        {
            H_Lcd_Void_LCDGoTo(2,2);
            H_Lcd_Void_LCDWriteString("SPL:");
            if(speedlimit<100)
            {
                
                if(speedlimit< 10)
                {
                    H_Lcd_Void_LCDGoTo(2,8);
			        H_Lcd_Void_LCDWriteString("  ");
                }
                else
                {
                    H_Lcd_Void_LCDGoTo(2,9);
                    H_Lcd_Void_LCDWriteString(" ");
                }
                
            }
            H_Lcd_Void_LCDGoTo(2,7);
            H_Lcd_Void_LCDWriteNumber(speedlimit);
             
        }
        H_Lcd_Void_LCDGoTo(2,15);
        if(speedLimiter_Status == 1)
        {
            H_Lcd_Void_LCDWriteString("ON ");
        }
        else
        {
            H_Lcd_Void_LCDWriteString("OFF");
        }
    }
}

void LCD_Write_Speed(void)
{
    
    u16 speed = ADC_u16Mapping_mm();
    if(screen_u8Page !=4)
    {
        H_Lcd_Void_LCDGoTo(1,3);
        H_Lcd_Void_LCDWriteString("SPEED: ");
        if(gearState == 'D')
        { 
            H_Lcd_Void_LCDGoTo(1,11);
            H_Lcd_Void_LCDWriteNumber(speed);
            /*to enhance display*/
            if(speed<100)
            {
                if(speed< 10)
                {
                    H_Lcd_Void_LCDGoTo(1,12);
                    H_Lcd_Void_LCDWriteString("  ");
                }
                else
                {
                    H_Lcd_Void_LCDGoTo(1,13);
                    H_Lcd_Void_LCDWriteString(" ");
                } 
            }
        }
        else
        {
            H_Lcd_Void_LCDGoTo(1,11);
            H_Lcd_Void_LCDWriteString("---");
        }

    }
       

}

u16 ADC_u16Mapping_mm(void)
{
    static u16 Speed_u16OLD=0;
    u32 Local_u16GetADC = 0;
	/*read the ADC value*/
	Local_u16GetADC = M_ADC_U16_ADCRead(ADC_CHANNEL_0);
    /*map it from 0>> 1023 to 0 >> 2000km ( 1000) to aviod float operation  */
	u32 speed_val=((Local_u16GetADC * 4000 )/ 1023) ;
	if(speed_val>=200)
    {
		speed_val=200;
	}
    if( abs(speed_val - Speed_u16OLD)>SPEED_TOLERANCE )
    {
        change_u8Flag =0;
    }
    Speed_u16OLD = speed_val;
	return speed_val;
}

void DM(void)
{
    /*if driver sleep : green led*/
    if ((change_u8Flag>5)&&(gearState == 'D'))
    {
        H_LED_Void_LedOn(LED_GRN);
    }
    else
    {
        H_LED_Void_LedOff(LED_GRN);
    }
}

void Update_Time(void)
{
	if(sec<59){
		sec++;
	}
	else{
		sec=0;
		mins++;
	}
	if(mins==60){
		hours++;
		mins=0;
	}
	else
	{
		//
	}
	if(hours==24){
		hours=0;
	}
	else{
		//
	}
}

void Void_Write_Time(void)
{
    if(screen_u8Page == 4)
    {
        H_Lcd_Void_LCDGoTo(1,1);
        H_Lcd_Void_LCDWriteString("18/3/24");
        H_Lcd_Void_LCDGoTo(1,10);
        H_Lcd_Void_LCDWriteNumber(hours);
        H_Lcd_Void_LCDGoTo(1,12);
        H_Lcd_Void_LCDWriteString(":");
        H_Lcd_Void_LCDWriteNumber(mins);
        H_Lcd_Void_LCDGoTo(1,15);
        H_Lcd_Void_LCDWriteString(":");
        H_Lcd_Void_LCDWriteNumber(sec);    
    }
	
}
