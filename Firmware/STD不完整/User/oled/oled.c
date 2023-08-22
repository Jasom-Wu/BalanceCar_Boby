#include "stm32f10x.h"
#include "oled.h"
#include "i2c.h"
#include "delay.h"
#include "codetab.h"//��ģ��
int OLED_SEND_Cmd(uint8_t cmd)//д������ͬ�豸��һ��,����Ҫ�Լ�����
{
	i2c_Start(OLED_GPIO_PORT, OLED_GPIO_SDA_PIN,OLED_GPIO_SCL_PIN);
	i2c_SendByte(OLED_ADDRESS,OLED_GPIO_PORT, OLED_GPIO_SDA_PIN,OLED_GPIO_SCL_PIN);
	if(i2c_WaitAck(OLED_GPIO_PORT, OLED_GPIO_SDA_PIN,OLED_GPIO_SCL_PIN) !=0)
	goto fail;
	i2c_SendByte(0x00,OLED_GPIO_PORT, OLED_GPIO_SDA_PIN,OLED_GPIO_SCL_PIN);//����OLCD,0x00��ָ��Ļ���ַ��д���ָ��ִ��һ�Σ���һ��д��ʱ�͸�����
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
int OLED_SEND_Data(uint8_t data)//д������ͬ�豸��һ��,����Ҫ�Լ�����
{
	i2c_Start(OLED_GPIO_PORT, OLED_GPIO_SDA_PIN,OLED_GPIO_SCL_PIN);
	i2c_SendByte(OLED_ADDRESS,OLED_GPIO_PORT, OLED_GPIO_SDA_PIN,OLED_GPIO_SCL_PIN);
	if(i2c_WaitAck(OLED_GPIO_PORT, OLED_GPIO_SDA_PIN,OLED_GPIO_SCL_PIN) !=0)
	goto fail;
	i2c_SendByte(0x40,OLED_GPIO_PORT, OLED_GPIO_SDA_PIN,OLED_GPIO_SCL_PIN);//����OLCD,0x40��������Ļ���ַ
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
	delay_ms(1000);		// 1s,�������ʱ����Ҫ,�ϵ����ʱ��û�д�����������
 OLED_SEND_Cmd(0xAE);  //�ر���ʾ
 OLED_SEND_Cmd(0xD5);  //����ʱ�ӷ�Ƶ����,��Ƶ��
 OLED_SEND_Cmd(0x80);    //[3:0],��Ƶ����;[7:4],��Ƶ��
 OLED_SEND_Cmd(0xA8);  //��������·��
 OLED_SEND_Cmd(0X3F);  //Ĭ��0X3F(1/64) 
 OLED_SEND_Cmd(0xD3);  //������ʾƫ��
 OLED_SEND_Cmd(0X00);  //Ĭ��Ϊ0
 OLED_SEND_Cmd(0x00); //������ʾλ�á��е͵�ַ
 OLED_SEND_Cmd(0x10); //������ʾλ�á��иߵ�ַ
 OLED_SEND_Cmd(0x40);  //������ʾ��ʼ�� [5:0],����.
 OLED_SEND_Cmd(0x8D);  //��ɱ�����
 OLED_SEND_Cmd(0x14);  //bit2������/�ر�
 OLED_SEND_Cmd(0x20);  //�����ڴ��ַģʽ
 OLED_SEND_Cmd(0x02);  //[1:0],00���е�ַģʽ; 01���е�ַģʽ; 10,ҳ��ַģʽ;Ĭ��10;
 OLED_SEND_Cmd(0xA1);  //���ض�������,bit0: 0,0->0; 1,0->127;
 //OLED_SEND_Cmd(0xC0);  //����COMɨ�跽��;bit3: 0,��ͨģʽ; 1,�ض���ģʽ COM[N-1]->COM0; N:����·��
 OLED_SEND_Cmd(0xC8);  //����COMɨ�跽��
 OLED_SEND_Cmd(0xDA);  //����COMӲ����������
 OLED_SEND_Cmd(0x12);  //[5:4]���� 
 OLED_SEND_Cmd(0x81);  //�Աȶ�����
 OLED_SEND_Cmd(0xEF);  //1~255;Ĭ��0X7F (��������,Խ��Խ��)
 OLED_SEND_Cmd(0xD9);  //����Ԥ�������
 OLED_SEND_Cmd(0xf1);  //[3:0],PHASE 1;[7:4],PHASE 2;
 OLED_SEND_Cmd(0xDB);  //����VCOMH ��ѹ����
 OLED_SEND_Cmd(0x30);  //[6:4] 000,0.65*vcc; 001,0.77*vcc; 011,0.83*vcc;
 OLED_SEND_Cmd(0xA4);  //ȫ����ʾ����;bit0:1,����;0,�ر�;(����/����)
 OLED_SEND_Cmd(0xA6);  //������ʾ��ʽ;bit0:1,������ʾ;0,������ʾ              
 OLED_SEND_Cmd(0xAF);  //������ʾ 
 OLED_CLS(0); //��������
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
  * @brief  OLED_ShowCN����ʾcodetab.h�еĺ���,16*16����
  * @param  x,y: ��ʼ������(x:0~127, y:0~7); 
	*					N:������codetab.h�е�����
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
  * @brief  OLED_ShowStr����ʾcodetab.h�е�ASCII�ַ�,��6*8��8*16��ѡ��
  * @param  x,y : ��ʼ������(x:0~127, y:0~7);
	*					ch[] :- Ҫ��ʾ���ַ���; 
	*					TextSize : �ַ���С(1:6*8 ; 2:8*16)
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
  * @brief  OLED_DrawBMP����ʾBMPλͼ
  * @param  x0,y0 :��ʼ������(x0:0~127, y0:0~7);
	*					x1,y1 : ���Խ���(������)������(x1:1~128,y1:1~8)
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
void OLED_Fill(unsigned char fill_Data,unsigned char begin,unsigned char end)//�������   fill_Data:Ҫ�������ݣ�begin:��ʼ�У���СΪ0��end:�����У����Ϊ7��
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
void OLED_SetPos(unsigned char x, unsigned char y) //������ʼ������
{ 
	OLED_SEND_Cmd(0xb0+y);
	OLED_SEND_Cmd(((x&0xf0)>>4)|0x10);
	OLED_SEND_Cmd((x&0x0f)|0x01);
}
/**************************************************
��������:��ת��Ļ��ʾ
��ڲ���:i=0:������ʾ;
         i=1:��ת��ʾ-----����ڣ��ڱ���                      
**************************************************/
void OLED_ColorTurn(uint8_t i)
{
	if(i==0)
		{
			OLED_SEND_Cmd(0xA6);//������ʾ
		}
	if(i==1)
		{
			OLED_SEND_Cmd(0xA7);//��ɫ��ʾ
		}
}
/**************************************************
��������:��Ļ��ת180��
��ڲ���:i=0:������ʾ
         i=1:��ת��ʾ
**************************************************/
void OLED_DisplayTurn(uint8_t i)
{
	if(i==0)
		{
			OLED_SEND_Cmd(0xC8);//������ʾ
			OLED_SEND_Cmd(0xA1);
		}
	if(i==1)
		{
			OLED_SEND_Cmd(0xC0);//��ת��ʾ
			OLED_SEND_Cmd(0xA0);
		}
}
/**
  * @brief  OLED_CLS������,0ȫ�壬1�����棬2������
  */
void OLED_CLS(uint8_t mode)//����
{
	if(mode ==0)
	OLED_Fill(0x00,0,7);
	else if(mode ==1)
	OLED_Fill(0x00,0,1);
	else
	OLED_Fill(0x00,2,7);
}


 /**
  * @brief  OLED_ON����OLED�������л���
  */
void OLED_ON(void)
{
	OLED_SEND_Cmd(0X8D);  //���õ�ɱ�
	OLED_SEND_Cmd(0X14);  //������ɱ�
	OLED_SEND_Cmd(0XAF);  //OLED����
}


 /**
  * @brief  OLED_OFF����OLED���� -- ����ģʽ��,OLED���Ĳ���10uA
  */
void OLED_OFF(void)
{
	OLED_SEND_Cmd(0X8D);  //���õ�ɱ�
	OLED_SEND_Cmd(0X10);  //�رյ�ɱ�
	OLED_SEND_Cmd(0XAE);  //OLED����
}
