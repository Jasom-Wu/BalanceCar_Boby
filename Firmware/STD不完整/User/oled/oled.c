#include "stm32f10x.h"
#include "oled.h"
#include "i2c.h"
#include "delay.h"
#include "codetab.h"//字模库
int OLED_SEND_Cmd(uint8_t cmd)//写操作不同设备不一样,所以要自己定义
{
	i2c_Start(OLED_GPIO_PORT, OLED_GPIO_SDA_PIN,OLED_GPIO_SCL_PIN);
	i2c_SendByte(OLED_ADDRESS,OLED_GPIO_PORT, OLED_GPIO_SDA_PIN,OLED_GPIO_SCL_PIN);
	if(i2c_WaitAck(OLED_GPIO_PORT, OLED_GPIO_SDA_PIN,OLED_GPIO_SCL_PIN) !=0)
	goto fail;
	i2c_SendByte(0x00,OLED_GPIO_PORT, OLED_GPIO_SDA_PIN,OLED_GPIO_SCL_PIN);//对于OLCD,0x00是指令的基地址，写入的指令执行一次，下一次写入时就覆盖了
	if(i2c_WaitAck(OLED_GPIO_PORT, OLED_GPIO_SDA_PIN,OLED_GPIO_SCL_PIN) !=0)
	goto fail;
	i2c_SendByte(cmd,OLED_GPIO_PORT, OLED_GPIO_SDA_PIN,OLED_GPIO_SCL_PIN);
	if(i2c_WaitAck(OLED_GPIO_PORT, OLED_GPIO_SDA_PIN,OLED_GPIO_SCL_PIN) !=0)
	goto fail;
	i2c_Stop(OLED_GPIO_PORT, OLED_GPIO_SDA_PIN,OLED_GPIO_SCL_PIN);
	return 1;
	fail:
	i2c_Stop(OLED_GPIO_PORT, OLED_GPIO_SDA_PIN,OLED_GPIO_SCL_PIN);
	return 0;
}
int OLED_SEND_Data(uint8_t data)//写操作不同设备不一样,所以要自己定义
{
	i2c_Start(OLED_GPIO_PORT, OLED_GPIO_SDA_PIN,OLED_GPIO_SCL_PIN);
	i2c_SendByte(OLED_ADDRESS,OLED_GPIO_PORT, OLED_GPIO_SDA_PIN,OLED_GPIO_SCL_PIN);
	if(i2c_WaitAck(OLED_GPIO_PORT, OLED_GPIO_SDA_PIN,OLED_GPIO_SCL_PIN) !=0)
	goto fail;
	i2c_SendByte(0x40,OLED_GPIO_PORT, OLED_GPIO_SDA_PIN,OLED_GPIO_SCL_PIN);//对于OLCD,0x40是数据域的基地址
	if(i2c_WaitAck(OLED_GPIO_PORT, OLED_GPIO_SDA_PIN,OLED_GPIO_SCL_PIN) !=0)
	goto fail;
	i2c_SendByte(data,OLED_GPIO_PORT, OLED_GPIO_SDA_PIN,OLED_GPIO_SCL_PIN);
	if(i2c_WaitAck(OLED_GPIO_PORT, OLED_GPIO_SDA_PIN,OLED_GPIO_SCL_PIN) !=0)
	goto fail;
	i2c_Stop(OLED_GPIO_PORT, OLED_GPIO_SDA_PIN,OLED_GPIO_SCL_PIN);
	return 1;
	fail:
	i2c_Stop(OLED_GPIO_PORT, OLED_GPIO_SDA_PIN,OLED_GPIO_SCL_PIN);
	return 0;
}
void OLED_Config(void)
{
	I2C_GPIO_Config(OLED_GPIO_PORT , OLED_GPIO_RCC_CMD , OLED_GPIO_RCC_PORT , OLED_GPIO_SDA_PIN , OLED_GPIO_SCL_PIN);
	delay_ms(1000);		// 1s,这里的延时很重要,上电后延时，没有错误的冗余设计
 OLED_SEND_Cmd(0xAE);  //关闭显示
 OLED_SEND_Cmd(0xD5);  //设置时钟分频因子,震荡频率
 OLED_SEND_Cmd(0x80);    //[3:0],分频因子;[7:4],震荡频率
 OLED_SEND_Cmd(0xA8);  //设置驱动路数
 OLED_SEND_Cmd(0X3F);  //默认0X3F(1/64) 
 OLED_SEND_Cmd(0xD3);  //设置显示偏移
 OLED_SEND_Cmd(0X00);  //默认为0
 OLED_SEND_Cmd(0x00); //设置显示位置—列低地址
 OLED_SEND_Cmd(0x10); //设置显示位置—列高地址
 OLED_SEND_Cmd(0x40);  //设置显示开始行 [5:0],行数.
 OLED_SEND_Cmd(0x8D);  //电荷泵设置
 OLED_SEND_Cmd(0x14);  //bit2，开启/关闭
 OLED_SEND_Cmd(0x20);  //设置内存地址模式
 OLED_SEND_Cmd(0x02);  //[1:0],00，列地址模式; 01，行地址模式; 10,页地址模式;默认10;
 OLED_SEND_Cmd(0xA1);  //段重定义设置,bit0: 0,0->0; 1,0->127;
 //OLED_SEND_Cmd(0xC0);  //设置COM扫描方向;bit3: 0,普通模式; 1,重定义模式 COM[N-1]->COM0; N:驱动路数
 OLED_SEND_Cmd(0xC8);  //设置COM扫描方向
 OLED_SEND_Cmd(0xDA);  //设置COM硬件引脚配置
 OLED_SEND_Cmd(0x12);  //[5:4]配置 
 OLED_SEND_Cmd(0x81);  //对比度设置
 OLED_SEND_Cmd(0xEF);  //1~255;默认0X7F (亮度设置,越大越亮)
 OLED_SEND_Cmd(0xD9);  //设置预充电周期
 OLED_SEND_Cmd(0xf1);  //[3:0],PHASE 1;[7:4],PHASE 2;
 OLED_SEND_Cmd(0xDB);  //设置VCOMH 电压倍率
 OLED_SEND_Cmd(0x30);  //[6:4] 000,0.65*vcc; 001,0.77*vcc; 011,0.83*vcc;
 OLED_SEND_Cmd(0xA4);  //全局显示开启;bit0:1,开启;0,关闭;(白屏/黑屏)
 OLED_SEND_Cmd(0xA6);  //设置显示方式;bit0:1,反相显示;0,正常显示              
 OLED_SEND_Cmd(0xAF);  //开启显示 
 OLED_CLS(0); //清屏函数
}
void BULECarInitDisplay(void)
{
	int m;
	delay_ms(1000);		// 2s
	for(m=0;m<7;m++)
	OLED_ShowCN(8+m*16, 0, m);
	for(m=0;m<5;m++)
	{
		delay_ms(200);
		OLED_DrawBMP(30,2,30+68,8,(unsigned char *)BMP1);
		delay_ms(200);
		OLED_DrawBMP(30,2,30+68,8,(unsigned char *)BMP2);
	}
		delay_ms(700);
		OLED_DrawBMP(30,2,30+68,8,(unsigned char *)BMP3);
		delay_ms(300);
		OLED_DrawBMP(30,2,30+68,8,(unsigned char *)BMP2);
		delay_ms(2000);
		OLED_CLS(2);
	delay_ms(300);
	OLED_ShowStr(0,2,2,"[date]:2022.4.26");
	OLED_ShowStr(0,4,1,"[name]:BLUE_TEETH_CAR");
	OLED_ShowStr(0,5,1,"---------------------");
	delay_ms(500);
	OLED_ShowStr(5,7,1,"powered by @wuchunye");
	delay_ms(1000);
	OLED_CLS(2);
}


/**
  * @brief  OLED_ShowCN，显示codetab.h中的汉字,16*16点阵
  * @param  x,y: 起始点坐标(x:0~127, y:0~7); 
	*					N:汉字在codetab.h中的索引
  */
void OLED_ShowCN(unsigned char x, unsigned char y, unsigned char N)
{
	unsigned char wm=0;
	unsigned int  adder=32*N;
	OLED_SetPos(x , y);
	for(wm = 0;wm < 16;wm++)
	{
		OLED_SEND_Data(F16x16[adder]);
		adder += 1;
	}
	OLED_SetPos(x,y + 1);
	for(wm = 0;wm < 16;wm++)
	{
		OLED_SEND_Data(F16x16[adder]);
		adder += 1;
	}
}
/**
  * @brief  OLED_ShowStr，显示codetab.h中的ASCII字符,有6*8和8*16可选择
  * @param  x,y : 起始点坐标(x:0~127, y:0~7);
	*					ch[] :- 要显示的字符串; 
	*					TextSize : 字符大小(1:6*8 ; 2:8*16)
  */
int OLED_ShowStr(unsigned char x, unsigned char y,unsigned char TextSize, char *formatString,...)
{
	int c = 0,i = 0,j = 0,length;
	char StrBuffer[STRBUFFERSIZE];
	va_list args;
	va_start(args,formatString);
	length = vsnprintf(StrBuffer,STRBUFFERSIZE,formatString,args);
	switch(TextSize)
	{
		case 1:
		{
			while(StrBuffer[j] != '\0')
			{
				c = StrBuffer[j] - 32;
				if(x > 126)
				{
					x = 0;
					y++;
				}
				OLED_SetPos(x,y);
				for(i=0;i<6;i++)
					OLED_SEND_Data(F6x8[c][i]);
				x += 6;
				j++;
			}
		}break;
		case 2:
		{
			while(StrBuffer[j] != '\0')
			{
				c = StrBuffer[j] - 32;
				if(x > 120)
				{
					x = 0;
					y++;
				}
				OLED_SetPos(x,y);
				for(i=0;i<8;i++)
					OLED_SEND_Data(F8X16[c*16+i]);
				OLED_SetPos(x,y+1);
				for(i=0;i<8;i++)
					OLED_SEND_Data(F8X16[c*16+i+8]);
				x += 8;
				j++;
			}
		}break;
	}
	va_end(args);
	return length;
}
/**
  * @brief  OLED_DrawBMP，显示BMP位图
  * @param  x0,y0 :起始点坐标(x0:0~127, y0:0~7);
	*					x1,y1 : 起点对角线(结束点)的坐标(x1:1~128,y1:1~8)
  */
void OLED_DrawBMP(unsigned char x0,unsigned char y0,unsigned char x1,unsigned char y1,unsigned char BMP[])
{
	unsigned int j=0;
	unsigned char x,y;

  if(y1%8==0)
		y = y1/8;
  else
		y = y1/8 + 1;
	for(y=y0;y<y1;y++)
	{
		OLED_SetPos(x0,y);
    for(x=x0;x<x1;x++)
		{
			OLED_SEND_Data(BMP[j++]);
		}
	}
}
void OLED_Fill(unsigned char fill_Data,unsigned char begin,unsigned char end)//按行填充   fill_Data:要填充的数据；begin:起始行，最小为0；end:结束行，最大为7；
{
	unsigned char n,m;
	for(m=begin;m<=end;m++)
	{
		OLED_SEND_Cmd(0xb0+m);		//page0-page1
		OLED_SEND_Cmd(0x00);		//low column start address
		OLED_SEND_Cmd(0x10);		//high column start address
		for(n=0;n<128;n++)
			{
				OLED_SEND_Data(fill_Data);
			}
	}
}
void OLED_SetPos(unsigned char x, unsigned char y) //设置起始点坐标
{ 
	OLED_SEND_Cmd(0xb0+y);
	OLED_SEND_Cmd(((x&0xf0)>>4)|0x10);
	OLED_SEND_Cmd((x&0x0f)|0x01);
}
/**************************************************
函数功能:反转屏幕显示
入口参数:i=0:正常显示;
         i=1:反转显示-----蓝变黑，黑变蓝                      
**************************************************/
void OLED_ColorTurn(uint8_t i)
{
	if(i==0)
		{
			OLED_SEND_Cmd(0xA6);//正常显示
		}
	if(i==1)
		{
			OLED_SEND_Cmd(0xA7);//反色显示
		}
}
/**************************************************
函数功能:屏幕旋转180度
入口参数:i=0:正常显示
         i=1:反转显示
**************************************************/
void OLED_DisplayTurn(uint8_t i)
{
	if(i==0)
		{
			OLED_SEND_Cmd(0xC8);//正常显示
			OLED_SEND_Cmd(0xA1);
		}
	if(i==1)
		{
			OLED_SEND_Cmd(0xC0);//反转显示
			OLED_SEND_Cmd(0xA0);
		}
}
/**
  * @brief  OLED_CLS，清屏,0全清，1清上面，2清下面
  */
void OLED_CLS(uint8_t mode)//清屏
{
	if(mode ==0)
	OLED_Fill(0x00,0,7);
	else if(mode ==1)
	OLED_Fill(0x00,0,1);
	else
	OLED_Fill(0x00,2,7);
}


 /**
  * @brief  OLED_ON，将OLED从休眠中唤醒
  */
void OLED_ON(void)
{
	OLED_SEND_Cmd(0X8D);  //设置电荷泵
	OLED_SEND_Cmd(0X14);  //开启电荷泵
	OLED_SEND_Cmd(0XAF);  //OLED唤醒
}


 /**
  * @brief  OLED_OFF，让OLED休眠 -- 休眠模式下,OLED功耗不到10uA
  */
void OLED_OFF(void)
{
	OLED_SEND_Cmd(0X8D);  //设置电荷泵
	OLED_SEND_Cmd(0X10);  //关闭电荷泵
	OLED_SEND_Cmd(0XAE);  //OLED休眠
}
