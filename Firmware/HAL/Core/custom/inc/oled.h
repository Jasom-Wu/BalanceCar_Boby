#ifndef __OLED_H_
#define __OLED_H_

#include "stdarg.h"
#include "stdio.h"
#include "stm32f1xx_hal.h"
#define OLED_ADDRESS	0x78   //通过调整0R电阻,屏可以0x78和0x7A两个地址 -- 默认0x78
#define STRBUFFERSIZE 100

#define OLED_GPIO_PORT GPIOB
#define OLED_GPIO_SDA_PIN GPIO_PIN_8
#define OLED_GPIO_CLK_PIN GPIO_PIN_9

void OLED_Config(void);
void OLED_CLS(uint8_t mode);
void OLED_ON(void);
void OLED_OFF(void);
void OLED_ShowCN(unsigned char x, unsigned char y, unsigned char N);
int OLED_ShowStr(unsigned char x, unsigned char y,unsigned char TextSize, char *formatString,...);
void OLED_ShowCN(unsigned char x, unsigned char y, unsigned char N);
void OLED_DrawBMP(unsigned char x0,unsigned char y0,unsigned char x1,unsigned char y1,unsigned char BMP[]);
void OLED_ColorTurn(uint8_t i);
void OLED_DisplayTurn(uint8_t i);
void BULECarInitDisplay(void);

int OLED_SEND_Data(uint8_t data);
int OLED_SEND_Cmd(uint8_t cmd);
void OLED_Fill(unsigned char fill_Data,unsigned char begin,unsigned char end);
void OLED_SetPos(unsigned char x, unsigned char y);
#endif//__OLED_H_
