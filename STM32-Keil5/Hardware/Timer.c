#include "stm32f10x.h"                  // Device header
#include "OLED.h"
#include "USART.h"
#include "Delay.h"

extern int Sleep_Flag, keynum;

void Timer_Init(void)
{
	/*����ʱ��*/
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);			//����TIM2��ʱ��
	
	/*����ʱ��Դ*/
	TIM_InternalClockConfig(TIM2);		//ѡ��TIM2Ϊ�ڲ�ʱ�ӣ��������ô˺�����TIMĬ��ҲΪ�ڲ�ʱ��
	
	/*ʱ����Ԫ��ʼ��*/
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;				//����ṹ�����
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;		//ʱ�ӷ�Ƶ��ѡ�񲻷�Ƶ���˲������������˲���ʱ�ӣ���Ӱ��ʱ����Ԫ����
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;	//������ģʽ��ѡ�����ϼ���
	TIM_TimeBaseInitStructure.TIM_Period = 20000 - 1;				//�������ڣ���ARR��ֵ
	TIM_TimeBaseInitStructure.TIM_Prescaler = 36000 - 1;				//Ԥ��Ƶ������PSC��ֵ
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;			//�ظ����������߼���ʱ���Ż��õ�
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure);				//���ṹ���������TIM_TimeBaseInit������TIM2��ʱ����Ԫ	
	
	/*�ж��������*/
	TIM_ClearFlag(TIM2, TIM_FLAG_Update);						//�����ʱ�����±�־λ
																//TIM_TimeBaseInit����ĩβ���ֶ������˸����¼�
																//��������˱�־λ�������жϺ󣬻����̽���һ���ж�
																//�������������⣬������˱�־λҲ��
	
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);					//����TIM2�ĸ����ж�
	
	/*NVIC�жϷ���*/
	//NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);				//����NVICΪ����2
																//����ռ���ȼ���Χ��0~3����Ӧ���ȼ���Χ��0~3
																//�˷������������������н������һ��
																//���ж���жϣ����԰Ѵ˴������main�����ڣ�whileѭ��֮ǰ
																//�����ö�����÷���Ĵ��룬���ִ�е����ûḲ����ִ�е�����
	
	/*NVIC����*/
	NVIC_InitTypeDef NVIC_InitStructure;						//����ṹ�����
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;				//ѡ������NVIC��TIM2��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				//ָ��NVIC��·ʹ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;	//ָ��NVIC��·����ռ���ȼ�Ϊ2
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;			//ָ��NVIC��·����Ӧ���ȼ�Ϊ1
	NVIC_Init(&NVIC_InitStructure);								//���ṹ���������NVIC_Init������NVIC����
	
	/*TIMʹ��*/
	TIM_Cmd(TIM2, ENABLE);			//ʹ��TIM2����ʱ����ʼ����
}

//* ��ʱ���жϺ��������Ը��Ƶ�ʹ�����ĵط�
void TIM2_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)
	{
		
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
		Serial_SendString("S1!");
		Delay_ms(20);
		OLED_ClearLine(1);
		OLED_ClearLine(2);
		OLED_ClearLine(3);
		OLED_ClearLine(4);
		Delay_ms(20);
		OLED_String(2,6, 3, 133, 134, 135);
		
		PWR_EnterSTOPMode(PWR_Regulator_LowPower, PWR_STOPEntry_WFI);
		
		OLED_ShowString(2, 6, "      ");
		SystemInit();
		
		OLED_ShowIcon(1,15,6); 
		Serial_SendString("S0!");
		keynum = 0;
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
	}
}
