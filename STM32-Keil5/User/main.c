#include "stm32f10x.h"                  // Device header
#include "OLED.h"
#include "GPIO.h"
#include "USART.h"
#include "Delay.h"
#include "User.h"
#include "GUI.h"
#include "Keyboard.h"
#include "Timer.h"
#include <string.h>
#include <stdlib.h>

//uint8_t Serial_RX, Serial_RXFlag;
char List_Origin[50], count = 0;
int List[11] = {0};
int i, j, OLED_HandlerFlag = 0, keynum = 0, CycleEnable = 1, Admin_Flag = 0, Sleep_Flag = 0;
long long int a;
US_S User[10];

int main()
{
	OLED_Init();//∆Ù”√OLED
	GPIO_Set();//∆Ù”√GPIO
	Delay_ms(100);
	Serial_Init();//∆Ù”√¥Æø⁄
	User_Init();
	Timer_Init();
	
	GPIO_SetBits(GPIOC, GPIO_Pin_13);
	int opt = 1;
	for(i = 0; i < 10; i++)
	{
		User[i].error = 0;
	}
	while(1)
	{
		CycleEnable = 1;
		OLED_String(1, 1, 4, 7, 8, 9, 10);//ª∂”≠ π”√
		OLED_ShowString(1, 9, "ATM!");
		OLED_String(3, 7, 2, 11, 12);//µ«¬º
		OLED_String(4, 7, 2, 13, 14);//◊¢≤·
		for(i = 1; i <= 2; i++)			
		{
			if(i == opt)
			{
				OLED_ShowString(i+2, 3, "->");
			}
			else
			{
				OLED_ShowString(i+2, 3, "  ");
			}
				
		}
		if(keynum)
		{
			if(keynum == 3)
			{
				opt -= 1;
				if(opt == 0)
				{
					opt = 2;
				}
			}
			else if(keynum == 9)
			{
				opt += 1;
				if(opt == 3)
				{
					opt = 1;
				}
			}
			else if(keynum == 6 || keynum == 12)
			{
				if(opt == 1)
				{
					login();
				}
				
				else if(opt == 2)
				{
					Reg();
				}
			}
			keynum = 0;
		}
	}
}

