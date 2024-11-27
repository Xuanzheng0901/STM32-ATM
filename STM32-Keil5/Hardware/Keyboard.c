#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"

extern int keynum, CycleEnable, Admin_Flag, Sleep_Flag;

void EXTI0_IRQHandler(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, DISABLE);
	Delay_ms(50);	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_ResetBits(GPIOA, GPIO_Pin_0);
	
	if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_4) == 0){Delay_ms(20);while(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_4) == 0)Delay_us(1);keynum = 1;}
	if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_5) == 0){Delay_ms(20);while(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_5) == 0)Delay_us(1);keynum = 11;}
	if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_6) == 0){Delay_ms(20);while(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_6) == 0)Delay_us(1);keynum = 12;}
	if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_7) == 0){Delay_ms(20);while(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_7) == 0)Delay_us(1);keynum = 13;}
	EXTI_ClearITPendingBit(EXTI_Line0);
	
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);	
	TIM2->CNT = 0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}

void EXTI1_IRQHandler(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, DISABLE);	
	Delay_ms(50);
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_ResetBits(GPIOA, GPIO_Pin_1);
	
	if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_4) == 0){Delay_ms(20);while(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_4) == 0)Delay_us(1);keynum = 2;}
	if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_5) == 0){Delay_ms(20);while(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_5) == 0)Delay_us(1);keynum = 5;}
	if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_6) == 0){Delay_ms(20);while(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_6) == 0)Delay_us(1);keynum = 8;}
	if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_7) == 0){Delay_ms(20);while(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_7) == 0)Delay_us(1);keynum = 14;}
	EXTI_ClearITPendingBit(EXTI_Line1);
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);	
	TIM2->CNT = 0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}

void EXTI2_IRQHandler(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, DISABLE);
	Delay_ms(50);	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_ResetBits(GPIOA, GPIO_Pin_2);
	
	if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_4) == 0){Delay_ms(20);while(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_4) == 0)Delay_us(1);keynum = 3;}
	if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_5) == 0){Delay_ms(20);while(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_5) == 0)Delay_us(1);keynum = 6;}
	if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_6) == 0){Delay_ms(20);while(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_6) == 0)Delay_us(1);keynum = 9;}
	if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_7) == 0){Delay_ms(20);while(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_7) == 0)Delay_us(1);keynum = 15;}
	EXTI_ClearITPendingBit(EXTI_Line2);
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);	
	TIM2->CNT = 0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}

void EXTI3_IRQHandler(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, DISABLE);
	Delay_ms(50);	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_ResetBits(GPIOA, GPIO_Pin_3);
	
	if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_4) == 0){Delay_ms(20);while(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_4) == 0)Delay_us(1);keynum = 4;}
	if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_5) == 0){Delay_ms(20);while(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_5) == 0)Delay_us(1);keynum = 7;}
	if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_6) == 0){Delay_ms(20);while(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_6) == 0)Delay_us(1);keynum = 10;}
	if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_7) == 0){Delay_ms(20);while(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_7) == 0)Delay_us(1);keynum = 16;}
	EXTI_ClearITPendingBit(EXTI_Line3);
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);	
	TIM2->CNT = 0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}

void EXTI9_5_IRQHandler(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	Delay_ms(100);	
	if(EXTI_GetITStatus(EXTI_Line6) == SET)
	{
		CycleEnable = 0;
		OLED_Clear();
		OLED_ShowIcon(1, 15, 6);
		EXTI_ClearITPendingBit(EXTI_Line6);
	}
	else if(EXTI_GetITStatus(EXTI_Line7) == SET)
	{
		OLED_Init();
		OLED_ShowIcon(1, 15, 6);
		EXTI_ClearITPendingBit(EXTI_Line7);
	}
	else if(EXTI_GetITStatus(EXTI_Line8) == SET)
	{
		
		Admin_Flag = !Admin_Flag;
		if(Admin_Flag)
		{
			GPIO_ResetBits(GPIOC, GPIO_Pin_13);
		}
		else
		{
			GPIO_SetBits(GPIOC, GPIO_Pin_13);
		}
		EXTI_ClearITPendingBit(EXTI_Line8);
	}
	else if(EXTI_GetITStatus(EXTI_Line9) == SET)
	{
		EXTI_ClearITPendingBit(EXTI_Line9);
	}
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);	
	TIM2->CNT = 0;
}
