#ifndef __USER_H__
#define __USER_H__
#include "stm32f10x.h"                  // Device header

void GetUserInfo(int user);
void ChangeUserInfo(int user, char* name, char* pwd, unsigned int money);
void DeleteUserInfo(int x);
void InsertUserIndex(int i);
void User_Init(void);
int FindUser(char* name);

typedef struct 
{
	char name[7];
	char pwd[7];
	unsigned int money;
	int error;
} US_S;

#endif
