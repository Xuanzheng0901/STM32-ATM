#include "stm32f10x.h"                  // Device header
#include "User.h"
#include "USART.h"
#include "String.h"
#include "OLED.h"
#include "stdlib.h"
#include "Delay.h"


extern US_S User[10];
extern char List_Origin[50];

extern int count, List[11];

void User_Init()
{	
	int i;
	memset(Serial_RxPacket, 0, 100);
	OLED_String(2, 1, 6, 0, 1, 2, 3, 4, 5);
	while(Serial_RxFlag == 0)
	{
		OLED_String(2, 1, 6, 0, 1, 2, 3, 4, 5);
		GPIO_ResetBits(GPIOC, GPIO_Pin_13);
		for(i = 13; i <= 15; i++)
		{
			OLED_ShowChar(2,i,'.');
			Delay_ms(200);
		}
		GPIO_SetBits(GPIOC, GPIO_Pin_13);
		for(i = 15; i >= 13; i--)
		{
			OLED_ShowChar(2, i, ' ');
			Delay_ms(200);
		}
	}
	
	if(Serial_RxPacket[0] == '?')
	{
		GPIO_ResetBits(GPIOC, GPIO_Pin_13);
		Serial_RxFlag = 0;
		OLED_ClearLine(1);
	}
	
	OLED_ShowIcon(1,15,6);
	
	memset(Serial_RxPacket, 0, 5);
	Serial_SendString("L!");
	OLED_String(2, 1, 6, 42, 43, 44, 45, 46, 47);
	OLED_ShowIcon(2, 13, 32);
	while(Serial_RxFlag == 0);
	strcpy(List_Origin, Serial_RxPacket);
	char *token = strtok(List_Origin, ",");// 以逗号为分隔符
	i = 0; 
    while (token != NULL) 
	{
        List[atoi(token)] = 1;  // 将字符串转换为整数并存入数组
        i++;
		count++;
        token = strtok(NULL, ",");  // 获取下一个子字符串
    }
	i = 0;
	for(i = 0; i < 10; i++)
	{
		if(List[i])
			GetUserInfo(i);
	}
	//Serial_SendArray(List, count);
	OLED_ClearLine(2);
	OLED_ShowString(2, 7, "OK!");
	Delay_ms(800);
	OLED_ClearLine(2);
}

void SendGet(int user)
{
	char GetStr[4] = {0};
	GetStr[0] = 'G';
	GetStr[1] = user + '0';
	GetStr[2] = '!';
	Serial_SendString(GetStr);
}

void GetUserInfo(int user)
{
	char money[11];
	int i;
	SendGet(user);
	Serial_RxFlag = 0;
	while(Serial_RxFlag == 0);
	for(i = 0; i < 6; i++)
	{
		User[user].name[i] = Serial_RxPacket[i];
		User[user].pwd[i] = Serial_RxPacket[i+6];
	}
	i = 0;
	while(Serial_RxPacket[i+12])
	{
		money[i] = Serial_RxPacket[i+12];
		i++;
	}
	money[i+1] = 0;
	memset(Serial_RxPacket, 0, 6);
	Serial_RxFlag = 0;
		
	User[user].money = atoi(money);
}

void ChangeUserInfo(int user, char* name, char* pwd, unsigned int money)//最大支持4294967295
{

	
//	strncpy(User[user].name, name, 6);
//	strncpy(User[user].pwd ,pwd, 6);
//	User[user].money = money;
	
	char PostStr[27] = {0};
	memset(PostStr, 0, sizeof(PostStr));
	
	PostStr[0] = 'P';
	PostStr[1] = user + '0';
	strncat(PostStr, User[user].name, 6);
	strncat(PostStr, User[user].pwd, 6);
	
	char fmoney[11];//4294967295
	sprintf(fmoney, "%d", User[user].money);
	strcat(PostStr, fmoney);
	
	strcat(PostStr, "?!");
	
	Serial_SendString(PostStr);
	
	while(Serial_RxFlag == 0);
	Serial_RxFlag = 0;
}
void InsertUserIndex(int i)
{
	List[i] = 1;
	count++;
}

void DeleteUserInfo(int x)
{
	char DelStr[4] = {0};
	DelStr[0] = 'D';
	DelStr[1] = x + '0';
	DelStr[2] = '!';
	Serial_SendString(DelStr);
	memset(User[x].pwd, 0, sizeof(User[x].pwd));
	memset(User[x].name, 0, sizeof(User[x].name));
	User[x].money = 0;
	List[x] = 0;
	count --;
}

int FindUser(char* name)
{
	int i = 0;
	while(i < 10)
	{
		if(strncmp(User[i].name, name, 6) == 0)
			return i;
		i++;
	}
	return -1;
}
