#ifndef __BLUE_TEETH_H
#define	__BLUE_TEETH_H

#include "stm32f1xx_hal.h"

#define TDYMAXSIZE  39 //���һλҪ��Ϊ����Ϊ���������鳤Ϊ10����ֻ�� 0 - 8 ����Ч���ݣ�9 ��У��λ��
#define TDYBUFFLEN  40
#define NAMEMAXLEN 20
#define PASSWORDLEN 5
typedef struct USER_DATA
{
	char CustomName[NAMEMAXLEN];
	char PassWord[PASSWORDLEN];
	uint8_t Baudrate;
	signed char VelocityVal;
	signed char TurnVal;//���������ԣ���char��unsigned char �����޷��ţ�Ҫ�з��ž�Ҫ��signed����
}UserDataTypedef;

extern _Bool BLUE_RXCONPLETED;
extern __IO UserDataTypedef User_Data;
extern __IO char TDY_data[TDYMAXSIZE];
extern _Bool BLUE_RXCONPLETED;
void BLUE_Config(void);
void BLUE_Process(void);
#endif
