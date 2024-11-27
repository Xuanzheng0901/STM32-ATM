#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "User.h"
#include "GUI.h"
#include <string.h>
#include <stdlib.h>


extern US_S User[10];
extern int keynum, count;
extern int List[11], CycleEnable, Admin_Flag;

void Reg(void);
void login(void);
int verify(int a);
void SetPwd(int a, int mode);
int ConfirmPwd(char* a);

void UserOperate(int index);
void ChangeMoney(int a);
void Transfer(int a);
void TransferMoney(int a, int b);
void ChaYuE(int a);
void QuKuan(int a, unsigned  int fmon);
void CunKuan(int a, unsigned int fmon);
void YiChu(void);

void AdminOperate(void);
void AdminManage(void);
void AdminDelete(int a);
void AdminChange(int a);
void AdminChangeName(int a);
void AdminChangePwd(int a);
void AdminChangeMon(int a);

int GetNumLength(int a);
char GetUnusedUserIndex(void);

void login(void)
{
	OLED_ShowString(1, 1, "            ");	
	OLED_ClearLine(2);
	OLED_ClearLine(3);
	OLED_ClearLine(4);
	keynum = 0;
	char name[7] = {0};
	while(CycleEnable)
	{
		OLED_String(1, 1, 2, 15, 16);//"<Fn1"
		OLED_String(2, 1, 6, 17, 18, 19, 21, 22, 20);//请输入用户名
		OLED_ShowChar(2, 13, ':');
		int i = 0;
		while(name[i])
		{
			OLED_ShowChar(3, 6+i, name[i]);
			i++;
		}
		if(i <= 5)
			OLED_ShowChar(3, 6+i, '_');
		if(keynum)
		{
			if(keynum <= 10 && i <= 5)
			{	
				name[i] = keynum - 1 + '0';
				keynum = 0;
			}
			else if (keynum == 11 && i >= 0)
			{
				OLED_ShowChar(3, i+6, ' ');
				name[i-1] = 0;
				keynum = 0;
			}
			else if(keynum == 12 && i == 6)
			{
				keynum = 0;
				int a;
				a = FindUser(name);
				
				if(a == -1 || (a == 0 && Admin_Flag == 0))
				{
					OLED_ClearLine(2);
					OLED_ClearLine(3);
					
					while(keynum == 0 && CycleEnable)
					{
						OLED_String(2, 1, 2, 21, 22);//用户
						OLED_ShowString(2, 5, name);
						OLED_String(2, 11, 3, 23, 24, 25);//不存在
						OLED_String(4, 1, 7, 26, 27, 28, 29, 30, 31, 32);//按任意键继续…
					}
					keynum = 0;
					int temp;
					for(temp = 0; temp < 6; temp++)
					{
						name[temp] = 0;
					}
					OLED_ClearLine(2);
					OLED_ClearLine(3);
					OLED_ClearLine(4);
				}
				else
				{
					int result = verify(a);
					if(result == 0)
					{
						OLED_ShowString(1, 1, "     ");
						OLED_ClearLine(2);
						OLED_ClearLine(3);
						OLED_ClearLine(4);
						return;
					}
				}
			}
			else if(keynum == 13)
			{
				OLED_ShowString(1, 1, "     ");
				OLED_ClearLine(2);
				OLED_ClearLine(3);
				OLED_ClearLine(4);
				
				keynum = 0;
				return;
			}
		}
	}
	OLED_ShowString(1, 1, "     ");
	OLED_ClearLine(2);
	OLED_ClearLine(3);
	OLED_ClearLine(4);
	CycleEnable = 1;
	return;
}

int verify(int a)
{
	//OLED_ShowNum(1, 13, a, 1); 
	OLED_ClearLine(2);
	OLED_ClearLine(3);
	OLED_ClearLine(4);
	char pwd[7] = {0};
	//char pwd_show[7] = {0};
	while(CycleEnable)
	{
		OLED_String(1, 1, 2, 15, 16); //《Fn1
		OLED_String(2, 1, 5, 17, 18, 19, 33, 34);//请输入密码
		//OLED_ShowString(4, 1, User[a].pwd);
		OLED_ShowChar(2, 11, ':');
		int i = 0;
		while(pwd[i])
		{
			OLED_ShowChar(3, 6+i, '*');
			i++;
		}
		if(i <= 5)
			OLED_ShowChar(3, 6+i, '_');
		if(keynum)
		{
			if(keynum <= 10 && i <= 5)
			{	
				pwd[i] = keynum - 1 + '0';
				keynum = 0;
			}
			else if(keynum == 11 && i >= 0)
			{
				OLED_ShowChar(3, i+6, ' ');
				pwd[i-1] = 0;
				keynum = 0;
			}
			else if(keynum == 12 && i == 6)
			{
				keynum = 0;
				if(strncmp(User[a].pwd, pwd, 6) == 0)
				{

					OLED_ShowString(1, 1, "     ");
					OLED_ClearLine(2);
					OLED_ClearLine(3);
					OLED_ClearLine(4);
					int temp;
					for(temp = 0; temp < 6; temp++)
					{
						pwd[temp] = 0;
					}
					if(a == 0)
					{
						AdminOperate();
						return 0;
					}
					else
					{
						UserOperate(a);
					}
					return 0;
				}
				else
				{
					User[a].error ++;
					if(User[a].error < 5)
					{
						OLED_ClearLine(2);
						OLED_ClearLine(3);
						OLED_String(2, 1, 4, 33, 34, 35, 36);//密码错误
						OLED_ShowChar(2, 9, '!');
						OLED_String(3, 1, 4, 37, 38, 39, 40);//还可重试
						OLED_ShowChar(3, 9, ':');
						OLED_ShowNum(3, 9, 5-User[a].error, 1);
						OLED_ShowIcon(3, 10, 41);//次
						OLED_String(4, 1, 7, 26, 27, 28, 29, 30, 31, 32);//按任意键继续…
						
						while(keynum == 0 && CycleEnable);
						keynum = 0;
						int temp;
						for(temp = 0; temp < 6; temp++)
						{
							pwd[temp] = 0;
						}
						OLED_ClearLine(2);
						OLED_ClearLine(3);
						OLED_ClearLine(4);
					}
					else
					{
						User[a].error = 4;
						int temp;
						for(temp = 0; temp < 6; temp++)
						{
							pwd[temp] = 0;
						}
						OLED_ShowString(1, 1, "     ");
						OLED_ClearLine(2);
						OLED_ClearLine(3);
						OLED_ClearLine(4);
						OLED_String(2, 1, 6, 35, 36, 41, 48, 49, 50);//错误次数过多
						OLED_ShowChar(2, 13, '!');
						OLED_String(3, 1, 2, 17, 25);	//请在			
						OLED_String(3, 7, 4, 51, 52, 39, 40);//秒后重试
						int cnt = 0;
						EXTI_InitTypeDef EXTI_InitStructure;
						EXTI_InitStructure.EXTI_Line = EXTI_Line0 | EXTI_Line1 | EXTI_Line2 | EXTI_Line3;
						EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
						EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
						EXTI_InitStructure.EXTI_LineCmd = DISABLE;
						RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, DISABLE);
						EXTI_Init(&EXTI_InitStructure);
						while(cnt < 10)
						{
							OLED_ShowNum(3, 5, 10-cnt, 2);
							Delay_s(1);
							
							cnt++;
							keynum = 0;
						}
						EXTI_InitStructure.EXTI_LineCmd = ENABLE;
						EXTI_Init(&EXTI_InitStructure);
						RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);	
						TIM2->CNT = 0;
						CycleEnable = 1;
						keynum = 0;
						OLED_ShowString(1, 1, "     ");
						OLED_ClearLine(2);
						OLED_ClearLine(3);
						OLED_ClearLine(4);
					}
				}
			}
			else if(keynum == 13)
			{
				OLED_ShowString(1, 1, "     ");
				OLED_ClearLine(2);
				OLED_ClearLine(3);
				OLED_ClearLine(4);
				
				keynum = 0;
				return 1;
			}
		}
	}
	OLED_ShowString(1, 1, "     ");
	OLED_ClearLine(2);
	OLED_ClearLine(3);
	OLED_ClearLine(4);
	return 1;
}

void UserOperate(int index)
{
	keynum = 0;
	int opt = 1, i;
	OLED_ShowString(1, 1, "              ");
	OLED_ClearLine(2);
	OLED_ClearLine(3);
	OLED_ClearLine(4);
	while(CycleEnable)
	{
		OLED_String(1, 1, 2, 15, 16);//《Fn1
		OLED_String(2, 1, 2, 44, 45);//用户
		OLED_ShowString(2, 5, User[index].name);
		OLED_ShowChar(2, 11, ',');
		OLED_String(2, 12, 2, 75, 76);//您好
		OLED_String(3, 5, 2, 70, 71);//转账
		OLED_String(3, 11, 3, 72, 73, 74);//存取款
		OLED_String(4, 3, 3, 83, 84, 80);//查余额
		OLED_String(4, 11, 3, 85, 33, 34);//改密码
		for(i = 1; i <= 4; i++)			
		{
			if(i == opt)
			{
				OLED_ShowString((i+1)/2+2, ((i+1)%2)*8+2, ">");
			}
			else
			{
				OLED_ShowString((i+1)/2+2, ((i+1)%2)*8+2, " ");
			}
		}
		if(keynum)
		{
			if(keynum == 3 || keynum == 9)
			{
				if(opt == 1)opt = 3;
				else if(opt == 2)opt = 4;
				else if(opt == 3)opt = 1;
				else if(opt == 4)opt = 2;
			}
			if(keynum == 5)
			{
				opt -= 1;
				if(opt <= 0)
				{
					opt = 4;
				}
			}
			if(keynum == 7)
			{
				opt += 1;
				if(opt >= 5)
				{
					opt = 1;
				}
			}
			else if(keynum == 6 || keynum == 12)
			{
				if(opt == 1)
				{
					Transfer(index);
				}
				
				else if(opt == 2)
				{
					ChangeMoney(index);
				}
				else if(opt == 3)
				{
					ChaYuE(index);
				}
				else if(opt == 4)
				{
					SetPwd(index, 1);
				}
			}
			else if(keynum == 13)
			{
				keynum = 0;
				OLED_ShowString(1, 1, "              ");
				OLED_ClearLine(2);
				OLED_ClearLine(3);
				OLED_ClearLine(4);
				return;
			}
			keynum = 0;
		}
	}
	OLED_ShowString(1, 1, "              ");
	OLED_ClearLine(2);
	OLED_ClearLine(3);
	OLED_ClearLine(4);
	return;
}

void AdminOperate(void)
{
	keynum = 0;
	OLED_ShowString(1, 1, "              ");
	OLED_ClearLine(2);
	OLED_ClearLine(3);
	OLED_ClearLine(4);
	while(CycleEnable)
	{
		OLED_ShowString(1, 1, "ATM");
		OLED_String(1, 4, 2, 108, 109);//管理
		OLED_ShowString(2, 1, "Fn1");
		OLED_String(2, 4, 2, 119, 120);//修改
		OLED_ShowString(2, 10, "Fn2");
		OLED_String(2, 13, 2, 121, 122);//删除
		OLED_String(3, 1, 8, 26, 27, 28, 29, 123, 124, 108, 109);//按任意键开始管理
		OLED_String(4, 1, 1, 26);//按
		OLED_ShowString(4, 3, "Fn3");
		OLED_String(4, 6, 2, 54, 55);//返回
		if(keynum)
		{
			OLED_ShowString(1, 1, "              ");
			OLED_ClearLine(2);
			OLED_ClearLine(3);
			OLED_ClearLine(4);
			if(keynum == 15)
			{
				keynum = 0;
				return;
			}
			else
			{
				keynum = 0;
				AdminManage();
				//return;
			}
		}
	}
}
void AdminManage()
{
	int position = 1;
	int i, j = 0, ShowFlag = 0;
	int temp[10] = {0};
	for(i = 0; i < 10; i++)
	{
		if(List[i])
		{	
			temp[j++] = i;
		}
	}	
	while(CycleEnable)
	{
		OLED_String(1, 1, 2, 125, 126);
		OLED_ShowChar(2, 1, '>');
		if(ShowFlag == 0)
		{
			OLED_String(1, 6, 2, 71, 126);
			OLED_String(1, 11, 2, 33, 34);
		}
		else
		{
			OLED_String(1, 6, 2, 84, 80);
		}
		for(i = 0; i < 3; i++)
		{
			int a = (position + i) % count + (position + i) / count;
				OLED_ShowNum(2+i, 2, temp[a], 1);
				if(ShowFlag == 0)
				{
					OLED_ShowString(2+i, 4, User[temp[a]].name);
					OLED_ShowString(2+i, 11, User[temp[a]].pwd);
				}
				else
				{
					OLED_ShowNum(2+i, 6, User[temp[a]].money, GetNumLength(User[temp[a]].money));
					for(j = 6+GetNumLength(User[temp[a]].money); j <= 16; j++)
						OLED_ShowChar(i+2, j, ' ');
				}
		}
		if(keynum)
		{
			if(keynum == 3)
			{
				keynum = 0;
				position --;
				if(position <= 0 )
				{
					position = count -1;
				}
			}
			else if(keynum == 9)
			{
				keynum = 0;
				position ++;
				if(position >= count)
				{
					position = 1;
				}
			}
			else if(keynum == 5 || keynum == 7)
			{
				keynum = 0;
				ShowFlag = (ShowFlag == 0) ? 1 : 0;
				OLED_ShowString(1, 5, "          ");
				OLED_ClearLine(2);
				OLED_ClearLine(3);
				OLED_ClearLine(4);
			}
			else if(keynum == 13)
			{
				AdminChange(temp[position]);
			}
			else if(keynum == 14)
			{
				AdminDelete(temp[position]);
				for(; position < 9; position++)
					temp[position] = temp[position+1];
				temp[9] = 0;
				position = 1;
			}
			else if(keynum == 15)
			{
				keynum = 0;
				OLED_ShowString(1, 1, "              ");
				OLED_ClearLine(2);
				OLED_ClearLine(3);
				OLED_ClearLine(4);
				return;
			}
			keynum = 0;
		}
	}
}
void AdminDelete(int a)
{
	keynum = 0;
	OLED_ShowString(1, 1, "              ");
	OLED_ClearLine(2);
	OLED_ClearLine(3);
	OLED_ClearLine(4);
	while(CycleEnable)
	{
		OLED_String(1, 1, 2, 15, 16);//"<Fn1"
		OLED_String(2, 1, 3, 21, 22, 20);
		OLED_ShowChar(2, 7, ':');
		OLED_ShowString(2, 9, User[a].name);
		OLED_String(3, 1, 4, 56, 57, 121, 122);
		OLED_ShowChar(3, 9, '?');
		OLED_ShowString(4, 1, "Fn1");
		OLED_String(4, 4, 2, 56, 57);//确认
		OLED_ShowString(4, 10, "Fn2");
		OLED_String(4, 13, 2, 54, 55);//返回
		if(keynum == 13)
		{
			keynum = 0;
			OLED_ShowString(1, 1, "              ");
			OLED_ClearLine(2);
			OLED_ClearLine(3);
			OLED_ClearLine(4);
			OLED_String(2, 1, 4, 90, 91, 92, 32);
			DeleteUserInfo(a);
			OLED_String(3, 1, 2, 64, 65);//完成
			OLED_ShowChar(3, 5, '!');
			Delay_s(1);
			Delay_ms(500);
			OLED_ShowString(1, 1, "            ");	
			OLED_ClearLine(2);
			OLED_ClearLine(3);
			OLED_ClearLine(4);
			return;
		}
		else if(keynum == 14)
		{
			keynum = 0;
			OLED_ShowString(1, 1, "              ");
			OLED_ClearLine(2);
			OLED_ClearLine(3);
			OLED_ClearLine(4);
			return;
		}
	}
}

void AdminChange(int a)
{
	keynum = 0;
	OLED_ShowString(1, 1, "              ");
	OLED_ClearLine(2);
	OLED_ClearLine(3);
	OLED_ClearLine(4);
	while(CycleEnable)
	{
		OLED_String(1, 1, 2, 15, 16);//"<Fn1"
		OLED_String(2, 1, 8, 127, 128, 129, 119, 120, 130, 131, 132);//选择要修改的信息
		OLED_ShowString(3, 1, "Fn2");
		OLED_String(3, 4, 3, 21, 22, 20);//用户名
		OLED_ShowString(4, 1, "Fn3");
		OLED_String(4, 4, 2, 33, 34);//密码
		OLED_ShowString(4, 10, "Fn4");
		OLED_String(4, 13, 2, 84, 80);//余额
		if(keynum)
		{
			if(keynum == 13)
			{
				keynum = 0;
				OLED_ShowString(1, 1, "              ");
				OLED_ClearLine(2);
				OLED_ClearLine(3);
				OLED_ClearLine(4);
				return;
			}
			else if(keynum == 14)
			{
				keynum = 0;
				AdminChangeName(a);
			}
			else if(keynum == 15)
			{
				keynum = 0;
				AdminChangePwd(a);
			}
			else if(keynum == 16)
			{
				keynum = 0;
				AdminChangeMon(a);
			}
		}
	}
}

void AdminChangeName(int a)
{
	keynum = 0;
	OLED_ShowString(1, 1, "              ");
	OLED_ClearLine(2);
	OLED_ClearLine(3);
	OLED_ClearLine(4);
	char name[7] = {0};
	while(CycleEnable)
	{
		OLED_String(1, 1, 2, 15, 16);
		OLED_String(2, 1, 7, 17, 18, 19, 112, 21, 22, 20);//请输入新用户名
		OLED_ShowChar(2, 15, ':');
		int i = 0;
		while(name[i])
		{
			OLED_ShowChar(3, 6+i, name[i]);
			i++;
		}
		if(i <= 5)
			OLED_ShowChar(3, 6+i, '_');
		if(keynum)
		{
			if(keynum <= 10 && i <= 5)
			{	
				name[i] = keynum - 1 + '0';
				keynum = 0;
			}
			else if(keynum == 11 && i >= 1)
			{
				OLED_ShowChar(3, i+6, ' ');
				name[i-1] = 0;
				keynum = 0;
			}
			else if(keynum == 12 && i == 6)
			{
				int a = FindUser(name);
				keynum = 0;
				if(a != -1)
				{
					
					OLED_ClearLine(2);
					OLED_ClearLine(3);
					OLED_ClearLine(4);
					OLED_String(2, 1, 6, 21, 22, 20, 53, 24, 25);//用户名已存在
					OLED_ShowChar(2, 13, '!');
					OLED_String(3, 1, 8, 26, 27, 28, 29, 54, 55, 39, 40);//按任意键返回重试
					int temp;
					for(temp = 0; temp < 6; temp++)
					{
						name[temp] = 0;
					}
					while(keynum == 0 && CycleEnable);
					keynum = 0;
					OLED_ClearLine(2);
					OLED_ClearLine(3);
					OLED_ClearLine(4);
				}
				else
				{
					keynum = 0;
					OLED_ShowString(1, 1, "              ");
					OLED_ClearLine(2);
					OLED_ClearLine(3);
					OLED_ClearLine(4);
					strncpy(User[a].name, name, 6);
					OLED_String(2, 1, 4, 90, 91, 92, 32);//处理中…
					ChangeUserInfo(a, User[a].name, User[a].pwd, User[a].money);
					
					
					OLED_String(3, 1, 2, 64, 65);//完成
					OLED_ShowChar(3, 5, '!');
					Delay_s(1);
					Delay_ms(500);
					OLED_ShowString(1, 1, "            ");	
					OLED_ClearLine(2);
					OLED_ClearLine(3);
					OLED_ClearLine(4);
					keynum = 0;
					return;
				}
			}
			else if(keynum == 13)
			{
				OLED_ShowString(1, 1, "     ");
				OLED_ClearLine(2);
				OLED_ClearLine(3);
				OLED_ClearLine(4);
				
				keynum = 0;
				return;
			}
		}
	}
}


void AdminChangePwd(int a)
{
	keynum = 0;
	OLED_ShowString(1, 1, "              ");
	OLED_ClearLine(2);
	OLED_ClearLine(3);
	OLED_ClearLine(4);
	char pwd[7] = {0};
	while(CycleEnable)
	{
		OLED_String(1, 1, 2, 15, 16);
		OLED_String(2, 1, 6, 17, 18, 19, 112, 33, 34);//请输入新密码
		OLED_ShowChar(2, 13, ':');
		int i = 0;
		while(pwd[i])
		{
			OLED_ShowChar(3, 6+i, pwd[i]);
			i++;
		}
		if(i <= 5)
			OLED_ShowChar(3, 6+i, '_');
		if(keynum)
		{
			if(keynum <= 10 && i <= 5)
			{	
				pwd[i] = keynum - 1 + '0';
				keynum = 0;
			}
			else if(keynum == 11 && i >= 1)
			{
				OLED_ShowChar(3, i+6, ' ');
				pwd[i-1] = 0;
				keynum = 0;
			}
			else if(keynum == 12 && i == 6)
			{
				keynum = 0;
				OLED_ShowString(1, 1, "              ");
				OLED_ClearLine(2);
				OLED_ClearLine(3);
				OLED_ClearLine(4);
				strncpy(User[a].pwd, pwd, 6);
				OLED_String(2, 1, 4, 90, 91, 92, 32);//处理中…
				ChangeUserInfo(a, User[a].name, User[a].pwd, User[a].money);
				
				OLED_String(3, 1, 2, 64, 65);//完成
				OLED_ShowChar(3, 5, '!');
				Delay_s(1);
				Delay_ms(500);
				OLED_ShowString(1, 1, "            ");	
				OLED_ClearLine(2);
				OLED_ClearLine(3);
				OLED_ClearLine(4);
				keynum = 0;
				return;
			}
			else if(keynum == 13)
			{
				OLED_ShowString(1, 1, "     ");
				OLED_ClearLine(2);
				OLED_ClearLine(3);
				OLED_ClearLine(4);
				keynum = 0;
				return;
			}
		}
	}
}

void AdminChangeMon(int a)
{
	keynum = 0;
	OLED_ShowString(1, 1, "              ");
	OLED_ClearLine(2);
	OLED_ClearLine(3);
	OLED_ClearLine(4);
	char money[11] = {0};
	while(CycleEnable)
	{
		OLED_String(1, 1, 2, 15, 16);
		OLED_String(2, 1, 5, 17, 18, 19, 84, 80);//请输入余额
		OLED_ShowChar(2, 11, ':');
		int i = 0;
		while(money[i])
		{
			OLED_ShowChar(3, 1+i, money[i]);
			i++;
		}
		if(i <= 9)
			OLED_ShowChar(3, 1+i, '_');
		if(keynum)
		{
			if(keynum <= 10 && i <= 9)
			{	
				money[i] = keynum - 1 + '0';
				keynum = 0;
			}
			else if(keynum == 11 && i >= 1)
			{
				OLED_ShowChar(3, i+1, ' ');
				money[i-1] = 0;
				keynum = 0;
			}
			else if(keynum == 12 && i >= 1)
			{
				unsigned int fmon = atoi(money);
				keynum = 0;
				OLED_ShowString(1, 1, "              ");
				OLED_ClearLine(2);
				OLED_ClearLine(3);
				OLED_ClearLine(4);
				User[a].money = fmon;
				OLED_String(2, 1, 4, 90, 91, 92, 32);//处理中…
				ChangeUserInfo(a, User[a].name, User[a].pwd, User[a].money);
				OLED_String(3, 1, 2, 64, 65);//完成
				OLED_ShowChar(3, 5, '!');
				Delay_s(1);
				Delay_ms(500);
				OLED_ShowString(1, 1, "            ");	
				OLED_ClearLine(2);
				OLED_ClearLine(3);
				OLED_ClearLine(4);
				keynum = 0;
				return;
			}
			else if(keynum == 13)
			{
				OLED_ShowString(1, 1, "     ");
				OLED_ClearLine(2);
				OLED_ClearLine(3);
				OLED_ClearLine(4);
				keynum = 0;
				return;
			}
		}
	}
}

void ChaYuE(int a)
{
	
	OLED_ShowString(1, 1, "              ");
	OLED_ClearLine(2);
	OLED_ClearLine(3);
	OLED_ClearLine(4);
	keynum = 0;
	while(CycleEnable && keynum != 13 && keynum != 14)
	{
		OLED_String(1, 1, 2, 44, 45);
		OLED_ShowString(1, 5, User[a].name);
		OLED_ShowChar(1, 11, ':');
		OLED_String(2, 1, 2, 84, 80);
		OLED_ShowChar(2, 5, ':');
		OLED_ShowNum(2, 6, User[a].money, GetNumLength(User[a].money));
		OLED_ShowIcon(3, 1, 26);//按
		OLED_ShowString(3, 3, "Fn1");
		OLED_String(3, 6, 2, 54, 55);//返回
		OLED_ShowIcon(4, 1, 26);//按
		OLED_ShowString(4, 3, "Fn2");
		OLED_String(4, 6, 4, 111, 85, 33, 34);//修改密码
	}
	if(keynum == 13)
	{
		keynum = 0;
		OLED_ShowString(1, 1, "              ");
		OLED_ClearLine(2);
		OLED_ClearLine(3);
		OLED_ClearLine(4);
		return;
	}
	else if(keynum == 14)
	{
		keynum = 0;
		SetPwd(a, 1);
		//return;
	}
}

void ChangeMoney(int a)
{
	keynum = 0;
	OLED_ShowString(1, 1, "            ");	
	OLED_ClearLine(2);
	OLED_ClearLine(3);
	OLED_ClearLine(4);
	
	
	while(CycleEnable && keynum != 13 && keynum != 14 && keynum != 15)
	{
		OLED_ShowIcon(2, 1, 26);//按
		OLED_ShowString(2, 3, "Fn1");
		OLED_String(2, 6, 2, 72, 74);//存款

		OLED_ShowIcon(3, 1, 26);//按
		OLED_ShowString(3, 3, "Fn2");
		OLED_String(3, 6, 2, 73, 74);//取款
		
		OLED_ShowIcon(4, 1, 26);//按
		OLED_ShowString(4, 3, "Fn3");
		OLED_String(4, 6, 2, 54, 55);//返回
	}
	char money[11] = {0};
	if(keynum == 13)
	{
		keynum = 0;
		OLED_ShowString(1, 1, "            ");	
		OLED_ClearLine(2);
		OLED_ClearLine(3);
		OLED_ClearLine(4);
		while(CycleEnable)
		{
			OLED_String(1, 1, 2, 15, 16);//《Fn1
			OLED_String(2, 1, 7, 17, 18, 19, 72, 74, 79, 80);//请输入存款金额
			OLED_ShowChar(2, 15, ':');
			int i = 0;
			while(money[i])
			{
				OLED_ShowChar(3, 1+i, money[i]);
				i++;
			}
			if(i <= 9)
				OLED_ShowChar(3, 1+i, '_');
			if(keynum)
			{
				if(keynum <= 10 && i <= 9)
				{	
					money[i] = keynum - 1 + '0';
					keynum = 0;
				}
				else if (keynum == 11 && i >= 0)
				{
					OLED_ShowChar(3, i+1, ' ');
					money[i-1] = 0;
					keynum = 0;
				}
				else if(keynum == 12 && i > 0)
				{
					if(strlen(money) == 10 && strcmp(money, "4294967295") > 0)
					{
						memset(money, 0, sizeof(money));
						YiChu();
					}
					else
					{
						unsigned int fmon = atoi(money);
						keynum = 0;
						CunKuan(a, fmon);
						return;
					}
				}
				else if(keynum == 13)
				{
					OLED_ShowString(1, 1, "            ");	
					OLED_ClearLine(2);
					OLED_ClearLine(3);
					OLED_ClearLine(4);
					keynum = 0;
					break;
				}
				keynum = 0;
			}
		}
	}
	else if(keynum == 14)
	{
		keynum = 0;
		OLED_ShowString(1, 1, "            ");	
		OLED_ClearLine(2);
		OLED_ClearLine(3);
		OLED_ClearLine(4);
		while(CycleEnable)
		{
			OLED_String(1, 1, 2, 15, 16);//《Fn1
			OLED_String(2, 1, 7, 17, 18, 19, 73, 74, 79, 80);//请输入取款金额
			OLED_ShowChar(2, 15, ':');
			int i = 0;
			while(money[i])
			{
				OLED_ShowChar(3, 1+i, money[i]);
				i++;
			}
			if(i <= 9)
				OLED_ShowChar(3, 1+i, '_');
			if(keynum)
			{
				if(keynum <= 10 && i <= 9)
				{	
					money[i] = keynum - 1 + '0';
					keynum = 0;
				}
				else if (keynum == 11 && i >= 0)
				{
					OLED_ShowChar(3, i+1, ' ');
					money[i-1] = 0;
					keynum = 0;
				}
				else if(keynum == 12 && i > 0)
				{
					keynum = 0;
					OLED_ShowString(1, 1, "            ");	
					OLED_ClearLine(2);
					OLED_ClearLine(3);
					OLED_ClearLine(4);
					if(strlen(money) == 10 && strcmp(money, "4294967295") > 0)
					{
						memset(money, 0, sizeof(money));
						YiChu();
					}
					else
					{
						unsigned int fmon = atoi(money);
						QuKuan(a, fmon);
						return;
					}
					
				}
				else if(keynum == 13)
				{
					OLED_ShowString(1, 1, "            ");	
					OLED_ClearLine(2);
					OLED_ClearLine(3);
					OLED_ClearLine(4);
					keynum = 0;
					break;
				}
				keynum = 0;
			}
		}
	}
	else if(keynum == 15)
	{
		OLED_ShowString(1, 1, "            ");	
		OLED_ClearLine(2);
		OLED_ClearLine(3);
		OLED_ClearLine(4);
		return;
	}
}

void CunKuan(int a, unsigned int fmon)
{
	keynum = 0;
	OLED_ShowString(1, 1, "            ");	
	OLED_ClearLine(2);
	OLED_ClearLine(3);
	OLED_ClearLine(4);
	
	if(fmon == 0)
	{
		OLED_String(2, 1, 7, 72, 74, 79, 80, 93, 94, 97);//存款金额不能为
		OLED_ShowString(2, 15, "0!");
		Delay_s(1);
		Delay_ms(500);
		keynum = 0;
		OLED_ShowString(1, 1, "            ");
		OLED_ClearLine(2);
		OLED_ClearLine(3);
		OLED_ClearLine(4);
		return;
	}
	else if(0xFFFFFFFF - fmon < User[a].money)
	{
		YiChu();
		return;
	}
	
	
	keynum = 0;
	
	while(CycleEnable && keynum != 13 && keynum != 14)
	{
		OLED_String(1, 1, 4, 72, 74, 79, 80);//存款金额
		OLED_ShowChar(1, 9, ':');
		OLED_ShowNum(2, 1, fmon, GetNumLength(fmon));
		OLED_ShowIcon(3, 1, 26);//按
		OLED_ShowString(3, 3, "Fn1");
		OLED_String(3, 6, 2, 56, 57);//确认
		OLED_ShowIcon(4, 1, 26);//按
		OLED_ShowString(4, 3, "Fn2");
		OLED_String(4, 6, 2, 54, 55);//返回
	}
	
	if(keynum == 13)
	{
		keynum = 0;
		OLED_ShowString(1, 1, "            ");	
		OLED_ClearLine(2);
		OLED_ClearLine(3);
		OLED_ClearLine(4);
		OLED_String(2, 1, 4, 90, 91, 92, 32);//处理中…
		User[a].money += fmon;
		ChangeUserInfo(a, User[a].name, User[a].pwd, User[a].money);
		OLED_ClearLine(2);
		OLED_String(2, 1, 2, 64, 65);//完成
		OLED_ShowChar(2, 5, '!');
		OLED_String(3, 1, 4, 88, 89, 84, 80);//当前余额
		OLED_ShowChar(3, 9, ':');
		OLED_ShowNum(4, 1, User[a].money, GetNumLength(User[a].money));
		Delay_s(1);
		Delay_ms(500);
		keynum = 0;
		OLED_ShowString(1, 1, "            ");	
		OLED_ClearLine(2);
		OLED_ClearLine(3);
		OLED_ClearLine(4);
		return;
	}
	else if(keynum == 14)
	{
		keynum = 0;
		OLED_ShowString(1, 1, "            ");	
		OLED_ClearLine(2);
		OLED_ClearLine(3);
		OLED_ClearLine(4);
		return;
	}
}

void QuKuan(int a, unsigned int fmon)
{
	keynum = 0;
	if(fmon > User[a].money)
	{
		keynum = 0;
		while(keynum == 0 && CycleEnable)
		{
			OLED_String(1, 1, 4, 84, 80, 86, 87);//余额不足
			OLED_ShowChar(1, 9, '!');
			OLED_String(2, 1, 4, 88, 89, 84, 80);//当前余额
			OLED_ShowNum(3, 1, User[a].money, GetNumLength(User[a].money));
			OLED_String(4, 1, 6, 26, 27, 28, 29, 54, 55);//按任意键返回
		}
		keynum = 0;
		OLED_ShowString(1, 1, "            ");	
		OLED_ClearLine(2);
		OLED_ClearLine(3);
		OLED_ClearLine(4);
		return;
	}
	else if(fmon == 0)
	{
		OLED_String(2, 1, 7, 73, 74, 79, 80, 93, 94, 97);//取款金额不能为
		OLED_ShowString(2, 15, "0!");
		Delay_s(1);
		Delay_ms(500);
		keynum = 0;
		OLED_ShowString(1, 1, "            ");
		OLED_ClearLine(2);
		OLED_ClearLine(3);
		OLED_ClearLine(4);
		return;
	}
	else
	{
		
		
		keynum = 0;
		
		while(CycleEnable && keynum != 13 && keynum != 14)
		{
			OLED_String(1, 1, 4, 73, 74, 79, 80);//取款金额
			OLED_ShowChar(1, 9, ':');
			OLED_ShowNum(2, 1, fmon, GetNumLength(fmon));
			OLED_ShowIcon(3, 1, 26);//按
			OLED_ShowString(3, 3, "Fn1");
			OLED_String(3, 6, 2, 56, 57);//确认
			OLED_ShowIcon(4, 1, 26);//按
			OLED_ShowString(4, 3, "Fn2");
			OLED_String(4, 6, 2, 54, 55);//返回
		}
		
		if(keynum == 13)
		{
			keynum = 0;
			OLED_ShowString(1, 1, "            ");	
			OLED_ClearLine(2);
			OLED_ClearLine(3);
			OLED_ClearLine(4);
			OLED_String(2, 1, 4, 90, 91, 92, 32);//处理中…

			User[a].money -= fmon;
			ChangeUserInfo(a, User[a].name, User[a].pwd, User[a].money);
			OLED_ClearLine(2);
			OLED_String(2, 1, 2, 64, 65);//完成
			OLED_ShowChar(2, 5, '!');
			OLED_String(3, 1, 4, 88, 89, 84, 80);//当前余额
			OLED_ShowChar(3, 9, ':');
			OLED_ShowNum(4, 1, User[a].money, GetNumLength(User[a].money));
			Delay_s(1);
			Delay_ms(500);
			keynum = 0;
			OLED_ShowString(1, 1, "            ");	
			OLED_ClearLine(2);
			OLED_ClearLine(3);
			OLED_ClearLine(4);
			return;
		}
		else if(keynum == 14)
		{
			keynum = 0;
			OLED_ShowString(1, 1, "            ");	
			OLED_ClearLine(2);
			OLED_ClearLine(3);
			OLED_ClearLine(4);
			return;
		}
	}
}
void Transfer(int a)
{
	OLED_ShowString(1, 1, "            ");	
	OLED_ClearLine(2);
	OLED_ClearLine(3);
	OLED_ClearLine(4);
	keynum = 0;
	char name[7] = {0};
	while(CycleEnable)
	{
		OLED_String(1, 1, 2, 15, 16);//《Fn1
		OLED_String(2, 1, 7, 17, 18, 19, 70, 71, 77, 78);//请输入转账对象
		OLED_ShowChar(2, 15, ':');
		int i = 0;
		while(name[i])
		{
			OLED_ShowChar(3, 6+i, name[i]);
			i++;
		}
		if(i <= 5)
			OLED_ShowChar(3, 6+i, '_');
		if(keynum)
		{
			if(keynum <= 10 && i <= 5)
			{	
				name[i] = keynum - 1 + '0';
				keynum = 0;
			}
			else if (keynum == 11 && i >= 0)
			{
				OLED_ShowChar(3, i+6, ' ');
				name[i-1] = 0;
				keynum = 0;
			}
			else if(keynum == 12 && i == 6)
			{
				keynum = 0;
				int b;
				b = FindUser(name);
				if(b == a)
				{
					OLED_ShowString(1, 1, "            ");	
					OLED_ClearLine(2);
					OLED_ClearLine(3);
					OLED_ClearLine(4);
					OLED_String(2, 1, 7, 93, 94, 77, 95, 96, 70, 71);//不能对自己转账
					OLED_ShowChar(2, 15, '!');
					Delay_s(2);
					keynum = 0;
					return;
				}
				if(b > 0)
				{
					TransferMoney(a, b);
					return;
				}
				else
				{
					OLED_ClearLine(2);
					OLED_ClearLine(3);	
					keynum = 0;
					while(keynum == 0 && CycleEnable)
					{
						OLED_String(2, 1, 2, 21, 22);//用户
						OLED_ShowString(2, 5, name);
						OLED_String(2, 11, 3, 23, 24, 25);//不存在
						OLED_String(4, 1, 7, 26, 27, 28, 29, 30, 31, 32);//按任意键返回…
					}
					keynum = 0;
					OLED_ShowString(1, 1, "            ");	
					OLED_ClearLine(2);
					OLED_ClearLine(3);
					OLED_ClearLine(4);
					memset(name, 0, sizeof(name));
				}
			}
			else if(keynum == 13)
			{
				OLED_ShowString(1, 1, "     ");
				OLED_ClearLine(2);
				OLED_ClearLine(3);
				OLED_ClearLine(4);
				
				keynum = 0;
				return;
			}
		}
	}
}
void TransferMoney(int a, int b)
{
	OLED_ShowString(1, 1, "            ");	
	OLED_ClearLine(2);
	OLED_ClearLine(3);
	OLED_ClearLine(4);
	keynum = 0;
	char money[11] = {0};
	while(CycleEnable)
	{
		OLED_String(1, 1, 2, 15, 16);//《Fn1
		OLED_String(2, 1, 7, 17, 18, 19, 70, 71, 79, 80);//请输入转账金额
		OLED_ShowChar(2, 15, ':');
		int i = 0;
		while(money[i])
		{
			OLED_ShowChar(3, 1+i, money[i]);
			i++;
		}
		if(i <= 9)
			OLED_ShowChar(3, 1+i, '_');
		if(keynum)
		{
			if(keynum <= 10 && i <= 9)
			{	
				money[i] = keynum - 1 + '0';
				keynum = 0;
			}
			else if (keynum == 11 && i >= 0)
			{
				OLED_ShowChar(3, i+1, ' ');
				money[i-1] = 0;
				keynum = 0;
			}
			else if(keynum == 12 && i > 0)
			{
				keynum = 0;
				OLED_ShowString(1, 1, "            ");	
				OLED_ClearLine(2);
				OLED_ClearLine(3);
				OLED_ClearLine(4);
				if(strlen(money) == 10 && strcmp(money, "4294967295") > 0)
				{
					memset(money, 0, sizeof(money));
					YiChu();
					return;
				}
				unsigned int fmon = atoi(money);
				if(fmon == 0)
				{
					OLED_String(2, 1, 7, 71, 72, 79, 80, 93, 94, 97);//转账金额不能为

					OLED_ShowString(2, 15, "0!");
					memset(money, 0, sizeof(money));
					Delay_s(2);
				}
				else if(fmon > User[a].money)
				{
					while(keynum == 0 && CycleEnable)
					{
						OLED_String(1, 1, 4, 84, 80, 86, 87);//余额不足
						OLED_ShowChar(1, 9, '!');
						OLED_String(2, 1, 4, 88, 89, 84, 80);//当前余额
						OLED_ShowNum(3, 1, User[a].money, GetNumLength(User[a].money));
						OLED_String(4, 1, 6, 26, 27, 28, 29, 54, 55, 32);//按任意键返回…
					}
					keynum = 0;
					OLED_ShowString(1, 1, "            ");	
					OLED_ClearLine(2);
					OLED_ClearLine(3);
					OLED_ClearLine(4);
					memset(money, 0, sizeof(money));
				}
				else if(0xFFFFFFFF - fmon < User[b].money)
					{
						memset(money, 0, sizeof(money));
						YiChu();
						break;
					}
				else
				{
					keynum = 0;
					while(keynum != 13 && keynum != 14 && CycleEnable)
					{
						OLED_String(1, 1, 4, 70, 71, 79, 80);//转账金额
						OLED_ShowChar(1, 9, ':');
						OLED_ShowNum(2, 1, fmon, GetNumLength(fmon));
						OLED_String(3, 1, 4, 70, 71, 77, 78);//转账对象
						OLED_ShowChar(3, 9, ':');
						OLED_ShowString(3, 10, User[b].name);
						OLED_ShowString(4, 1, "Fn1");
						OLED_String(4, 4, 2, 56, 57);//确认
						OLED_ShowString(4, 10, "Fn2");
						OLED_String(4, 13, 2, 54, 55);//返回
					}
					
					if(keynum == 13)//确认
					{
						keynum = 0;
						OLED_ShowString(1, 1, "            ");	
						OLED_ClearLine(2);
						OLED_ClearLine(3);
						OLED_ClearLine(4);
						OLED_String(2, 1, 4, 90, 91, 92, 32);//处理中
						User[a].money -= fmon;
						User[b].money += fmon;
						ChangeUserInfo(a, User[a].name, User[a].pwd, User[a].money);
						Delay_ms(300);
						ChangeUserInfo(b, User[b].name, User[b].pwd, User[b].money);
						OLED_String(3, 1, 2, 64, 65);//完成
						OLED_ShowChar(3, 5, '!');
						Delay_s(1);
						Delay_ms(500);
						OLED_ShowString(1, 1, "            ");	
						OLED_ClearLine(2);
						OLED_ClearLine(3);
						OLED_ClearLine(4);
						return;
					}
					else if(keynum == 14)//取消
					{
						keynum = 0;
						memset(money, 0, sizeof(money));
						OLED_ShowString(1, 1, "            ");	
						OLED_ClearLine(2);
						OLED_ClearLine(3);
						OLED_ClearLine(4);
					}
				}
			}
			else if(keynum == 13)
			{
				OLED_ShowString(1, 1, "            ");	
				OLED_ClearLine(2);
				OLED_ClearLine(3);
				OLED_ClearLine(4);
				return;
			}
		}
	}
}

int GetNumLength(int a)
{
	int i;
	for(i = 1; i < 16; i++)
	{
		if(a / 10 != 0)
			a /= 10;
		else 
			return i;
	}
	return 16;
}

void Reg(void)
{
	keynum = 0;
	OLED_ShowString(1, 1, "              ");
	OLED_ClearLine(2);
	OLED_ClearLine(3);
	OLED_ClearLine(4);
	char name[7] = {0};

	char user = GetUnusedUserIndex();
	while(CycleEnable)
	{
		OLED_String(1, 1, 2, 15, 16);
		OLED_String(2, 1, 6, 17, 18, 19, 21, 22, 20);//请输入用户名
		OLED_ShowChar(2, 13, ':');
		int i = 0;
		while(name[i])
		{
			OLED_ShowChar(3, 6+i, name[i]);
			i++;
		}
		if(i <= 5)
			OLED_ShowChar(3, 6+i, '_');
		if(keynum)
		{
			if(keynum <= 10 && i <= 5)
			{	
				name[i] = keynum - 1 + '0';
				keynum = 0;
			}
			else if(keynum == 11 && i >= 0)
			{
				OLED_ShowChar(3, i+6, ' ');
				name[i-1] = 0;
				keynum = 0;
			}
			else if(keynum == 12 && i == 6)
			{
				int a = FindUser(name);
				keynum = 0;
				if(a != -1)
				{
					
					OLED_ClearLine(2);
					OLED_ClearLine(3);
					OLED_ClearLine(4);
					OLED_String(2, 1, 6, 21, 22, 20, 53, 24, 25);//用户名已存在
					OLED_ShowChar(2, 13, '!');
					OLED_String(3, 1, 8, 26, 27, 28, 29, 54, 55, 39, 40);//按任意键返回重试
					int temp;
					for(temp = 0; temp < 6; temp++)
					{
						name[temp] = 0;
					}
					while(keynum == 0 && CycleEnable);
					keynum = 0;
					OLED_ClearLine(2);
					OLED_ClearLine(3);
					OLED_ClearLine(4);
				}
				else
				{
					keynum = 0;
					strncpy(User[user].name, name, 6);
					SetPwd(user, 0);
					return;
				}
			}
			else if(keynum == 13)
			{
				int temp;
				for(temp = 0; temp < 6; temp++)
				{
					name[temp] = 0;
				}
				OLED_ShowString(1, 1, "     ");
				OLED_ClearLine(2);
				OLED_ClearLine(3);
				OLED_ClearLine(4);
				
				keynum = 0;
				return;
			}
		}
	}
	OLED_ShowString(1, 1, "     ");
	OLED_ClearLine(2);
	OLED_ClearLine(3);
	OLED_ClearLine(4);
	CycleEnable = 1;
	return;
}

char GetUnusedUserIndex()
{
	int i = 0;
	for(i = 0; i < 10; i++)
	{
		if(List[i] == 0)
			return i;
	}
	return 99;
}
void SetPwd(int a, int mode)
{
	keynum = 0;
	OLED_ShowString(1, 1, "              ");
	//OLED_ShowCNString(1, 1, 15, 16); 
	OLED_ClearLine(2);
	OLED_ClearLine(3);
	OLED_ClearLine(4);
	char pwd[7] = {0};
	
	while(CycleEnable)
	{
		//OLED_ShowCNString(1, 1, 15, 16); 
		OLED_String(2, 1, 5, 17, 18, 19, 33, 34);//请输入密码
		OLED_ShowChar(2, 11, ':');
		OLED_String(1, 1, 2, 15, 16); 
		int i = 0;
		while(pwd[i])
		{
			OLED_ShowChar(3, 6+i, '*');
			i++;
		}
		if(i <= 5)
			OLED_ShowChar(3, 6+i, '_');
		if(keynum)
		{
			if(keynum <= 10 && i <= 5)
			{	
				pwd[i] = keynum - 1 + '0';
				keynum = 0;
			}
			else if(keynum == 11 && i >= 0)
			{
				OLED_ShowChar(3, i+6, ' ');
				pwd[i-1] = 0;
				keynum = 0;
			}
			else if(keynum == 12 && i == 6)
			{
				keynum = 0;
				if(strncmp(pwd, User[a].pwd, 6) == 0 && mode == 1)
				{
					while(keynum == 0 && CycleEnable)
					{
						OLED_ShowString(1, 1, "              ");
						OLED_ClearLine(2);
						OLED_ClearLine(3);
						OLED_ClearLine(4);
						OLED_String(2, 1, 8, 112, 33, 34, 93, 94, 113, 114, 33);//新密码不能与原密
						OLED_String(3, 1, 3, 34, 115, 116);//码相同
						OLED_ShowChar(3, 7, '!');
						OLED_String(4, 1, 7, 26, 27, 28, 29, 30, 31, 32);//按任意键返回…
					}
				}
				else
				{
					int result = ConfirmPwd(pwd);
					if(result)
					{
						if(mode == 0)
						{
							strncpy(User[a].pwd, pwd, 6);
							
							OLED_ShowString(1, 1, "              ");
							OLED_ClearLine(2);
							OLED_ClearLine(3);
							OLED_ClearLine(4);
							OLED_String(2, 1, 4, 13, 14, 64, 65);//注册完成
							OLED_ShowChar(2, 9, '!');
							OLED_String(3, 1, 6, 66, 67, 68, 69, 11, 12, 32);//即将自动登录…
							InsertUserIndex(a);
							ChangeUserInfo(a, User[a].name, User[a].pwd, 0);
							Delay_s(2);
							UserOperate(a);
							return;
						}
						else if(mode == 1)
						{
							strncpy(User[a].pwd, pwd, 6);
							
							OLED_ShowString(1, 1, "              ");
							OLED_ClearLine(2);
							OLED_ClearLine(3);
							OLED_ClearLine(4);
							OLED_String(2, 1, 4, 111, 85, 64, 65);//修改完成
							OLED_ShowChar(2, 9, '!');
							ChangeUserInfo(a, User[a].name, User[a].pwd, 0);
							Delay_s(2);
							return;
						}
					}
					else
					{
						int temp;
						for(temp = 0; temp < 6; temp++)
						{
							pwd[temp] = 0;
						}
						OLED_ShowString(1, 1, "              ");
						OLED_ClearLine(2);
						OLED_ClearLine(3);
						OLED_ClearLine(4);
					}
				}
			}
			else if(keynum == 13)
			{
				keynum = 0;
				OLED_ShowString(1, 1, "              ");
				OLED_ClearLine(2);
				OLED_ClearLine(3);
				OLED_ClearLine(4);
				return;
			}
		}
	}
	OLED_ShowString(1, 1, "     ");
	OLED_ClearLine(2);
	OLED_ClearLine(3);
	OLED_ClearLine(4);
	return;
}

int ConfirmPwd(char* a)
{
	keynum = 0;
	OLED_ShowString(1, 1, "              ");
	OLED_ClearLine(2);
	OLED_ClearLine(3);
	OLED_ClearLine(4);
	char pwd[7] = {0};
	
	while(CycleEnable)
	{
		OLED_String(1, 1, 2, 15, 16);//《Fn1
		OLED_String(2, 1, 4, 56, 57, 33, 34);//确认密码
		OLED_ShowChar(2, 9, ':');
		int i = 0;
		while(pwd[i])
		{
			OLED_ShowChar(3, 6+i, '*');
			i++;
		}
		if(i <= 5)
			OLED_ShowChar(3, 6+i, '_');
		if(keynum)
		{
			if(keynum <= 10 && i <= 5)
			{	
				pwd[i] = keynum - 1 + '0';
				keynum = 0;
			}
			else if(keynum == 11 && i >= 0)
			{
				OLED_ShowChar(3, i+6, ' ');
				pwd[i-1] = 0;
				keynum = 0;
			}
			else if(keynum == 12 && i == 6)
			{
				OLED_ShowString(1, 1, "              ");
				OLED_ClearLine(2);
				OLED_ClearLine(3);
				OLED_ClearLine(4);
				keynum = 0;
				if(strncmp(a, pwd, 6) == 0)
				{
					return 1;
				}
				else
				{
					OLED_String(2, 1, 7, 60, 61, 33, 34, 23, 58, 59);//前后密码不一致
					OLED_ShowChar(2, 15, '!');

					OLED_ShowIcon(3, 1, 26);//按
					OLED_ShowString(3, 3, "Fn1");
					OLED_String(3, 6, 4, 54, 55, 39, 63);//返回重设
				
					OLED_ShowIcon(4, 1, 26);//按
					OLED_ShowString(4, 3, "Fn2");
					OLED_String(4, 6, 4, 62, 41, 56, 57);//再次确认

					while(keynum != 13 && keynum != 14);
					
					if(keynum == 14)
					{
						OLED_ShowString(1, 1, "              ");
						OLED_ClearLine(2);
						OLED_ClearLine(3);
						OLED_ClearLine(4);
						keynum = 0;
						int temp;
						for(temp = 0; temp < 6; temp ++)
						{
							pwd[temp] = 0;
						}
					}
					else if(keynum == 13)
					{
						keynum = 0;
						return 0;
					}
				}
			}
			else if(keynum == 13)
			{
				keynum = 0;
				return 0;
			}
		}
	}
	OLED_ShowString(1, 1, "     ");
	OLED_ClearLine(2);
	OLED_ClearLine(3);
	OLED_ClearLine(4);
	return 0;
}
void YiChu()
{
	keynum = 0;
	OLED_ShowString(1, 1, "            ");	
	OLED_ClearLine(2);
	OLED_ClearLine(3);
	OLED_ClearLine(4);
	OLED_String(1, 1, 4, 98, 99, 100, 101);
	OLED_ShowChar(1, 9, '!');
	OLED_String(2, 1, 4, 102, 103, 104, 105);
	OLED_ShowChar(2, 9, '!');
	OLED_String(3, 1, 6, 17 , 106, 107, 108, 109, 110);
	OLED_String(4, 1, 7, 26, 27, 28, 29, 30, 31, 32);
	while(keynum == 0);
	keynum = 0;
	OLED_ShowString(1, 1, "            ");	
	OLED_ClearLine(2);
	OLED_ClearLine(3);
	OLED_ClearLine(4);
}
