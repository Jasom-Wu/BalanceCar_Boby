#ifndef __BLUE_TEETH_H
#define	__BLUE_TEETH_H

#include "stm32f1xx_hal.h"

#define TDYMAXSIZE  39 //最后一位要作为数据为，例如数组长为10，则只有 0 - 8 是有效数据，9 是校验位。
#define TDYBUFFLEN  40
#define NAMEMAXLEN 20
#define PASSWORDLEN 5
typedef struct USER_DATA
{
	char CustomName[NAMEMAXLEN];
	char PassWord[PASSWORDLEN];
	uint8_t Baudrate;
	signed char VelocityVal;
	signed char TurnVal;//编译器特性！！char和unsigned char 都是无符号，要有符号就要加signed！！
}UserDataTypedef;

extern _Bool BLUE_RXCONPLETED;
extern __IO UserDataTypedef User_Data;
extern __IO char TDY_data[TDYMAXSIZE];
extern _Bool BLUE_RXCONPLETED;
void BLUE_Config(void);
void BLUE_Process(void);
#endif
