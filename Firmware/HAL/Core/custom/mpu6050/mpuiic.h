#ifndef __MPUIIC_H
#define __MPUIIC_H
#include "sys.h"

#define MPU_GPIO_PORT GPIOB

//IO方向设置
#define MODE_IN 1
#define MODE_OUT 0
#define MPU_SDA_IN()  {MPU_GPIO_MODESET(1,MODE_IN);}
#define MPU_SDA_OUT() {MPU_GPIO_MODESET(1,MODE_OUT);}


//#define SDA_IN()  {GPIOB->CRH&=0XFFFFFF0F;GPIOB->CRH|=8<<4;}
//#define SDA_OUT() {GPIOB->CRH&=0XFFFFFF0F;GPIOB->CRH|=3<<4;}

//IO操作函数	 //记得改！！
#define MPU_IIC_SCL     PBout(0) //SCL
#define MPU_IIC_SDA     PBout(1) //SDA	
#define MPU_READ_SDA   	PBin(1) //SDA	




//IIC所有操作函数
void MPU_IIC_Delay(void);				//MPU IIC延时函数
void MPU_IIC_Init(void);                //初始化IIC的IO口				 
void MPU_IIC_Start(void);				//发送IIC开始信号
void MPU_IIC_Stop(void);	  			//发送IIC停止信号
void MPU_IIC_Send_Byte(uint8_t txd);			//IIC发送一个字节
uint8_t MPU_IIC_Read_Byte(unsigned char ack);//IIC读取一个字节
uint8_t MPU_IIC_Wait_Ack(void); 				//IIC等待ACK信号
void MPU_IIC_Ack(void);					//IIC发送ACK信号
void MPU_IIC_NAck(void);				//IIC不发送ACK信号

void MPU_GPIO_MODESET(uint8_t PINx,uint8_t INorOUT);
void IMPU_IC_Write_One_Byte(uint8_t daddr,uint8_t addr,uint8_t data);
uint8_t MPU_IIC_Read_One_Byte(uint8_t daddr,uint8_t addr);	  
#endif
















