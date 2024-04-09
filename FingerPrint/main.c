#include "main.h"

void setup (void)
{
    H_Lcd_Void_LCDInit();
    H_LED_Void_LedInit(LED_GRN);
    H_LED_Void_LedInit(LED_YEL);
    FP_intit();
    H_PushButton_Void_PushButtonInit(PUSH_BUTTON_0);
    H_PushButton_Void_PushButtonInit(PUSH_BUTTON_1);
    H_Lcd_Void_LCDGoTo(2,0);
	H_Lcd_Void_LCDWriteString("********************");
    H_Lcd_Void_LCDGoTo(3,0);
    H_Lcd_Void_LCDWriteString("1:ADD   2:Match");


    
}

int main(void)
{
	setup();
    u8 local_FPIndex =1;
    u8 local_u8Flag =0;

    while(1)
    {
		if((H_PushButton_U8_PushButtonRead(PUSH_BUTTON_0)==PUSH_BUTTON_PRESSED)&&(local_FPIndex<15))
		{
            FP_setNewFinger(local_FPIndex);
            local_FPIndex++;
        }
		else if(H_PushButton_U8_PushButtonRead(PUSH_BUTTON_1)==PUSH_BUTTON_PRESSED)
		{
            for(u8 i = 1;i<=local_FPIndex;i++)
            {
                if(FP_CheckMatch(i))
                {
                    H_Lcd_Void_LCDGoTo(0,0);
                    H_Lcd_Void_LCDWriteString("    Matched  *_^    ");
                    local_u8Flag = 1;
                    break;
                }
                else 
                {
                	local_u8Flag = 0;
                }
            }
            if(local_u8Flag == 0)
            {
                H_Lcd_Void_LCDGoTo(0,0);
                H_Lcd_Void_LCDWriteString(" Not  Matched  -_- ");
            }
            
            
        }
	}
     
}
    
    

    



