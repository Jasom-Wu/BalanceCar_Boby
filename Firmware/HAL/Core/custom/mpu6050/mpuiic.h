#ifndef __MPUIIC_H
#define __MPUIIC_H
#include "sys.h"

#define MPU_GPIO_PORT GPIOB

//IO��������
#define MODE_IN 1
#define MODE_OUT 0
#define MPU_SDA_IN()  {MPU_GPIO_MODESET(1,MODE_IN);}
#define MPU_SDA_OUT() {MPU_GPIO_MODESET(1,MODE_OUT);}


//#define SDA_IN()  {GPIOB->CRH&=0XFFFFFF0F;GPIOB->CRH|=8<<4;}
//#define SDA_OUT() {GPIOB->CRH&=0XFFFFFF0F;GPIOB->CRH|=3<<4;}

//IO��������	 //�ǵøģ���
#define MPU_IIC_SCL     PBout(0) //SCL
#define MPU_IIC_SDA     PBout(1) //SDA	
#define MPU_READ_SDA   	PBin(1) //SDA	




//IIC���в�������
void MPU_IIC_Delay(void);				//MPU IIC��ʱ����
void MPU_IIC_Init(void);                //��ʼ��IIC��IO��				 
void MPU_IIC_Start(void);				//����IIC��ʼ�ź�
void MPU_IIC_Stop(void);	  			//����IICֹͣ�ź�
void MPU_IIC_Send_Byte(uint8_t txd);			//IIC����һ���ֽ�
uint8_t MPU_IIC_Read_Byte(unsigned char ack);//IIC��ȡһ���ֽ�
uint8_t MPU_IIC_Wait_Ack(void); 				//IIC�ȴ�ACK�ź�
void MPU_IIC_Ack(void);					//IIC����ACK�ź�
void MPU_IIC_NAck(void);				//IIC������ACK�ź�

void MPU_GPIO_MODESET(uint8_t PINx,uint8_t INorOUT);
void IMPU_IC_Write_One_Byte(uint8_t daddr,uint8_t addr,uint8_t data);
uint8_t MPU_IIC_Read_One_Byte(uint8_t daddr,uint8_t addr);	  
#endif
















