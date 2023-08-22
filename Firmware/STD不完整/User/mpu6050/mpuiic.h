#ifndef __MPUIIC_H
#define __MPUIIC_H
#include "sys.h"

#define MPU_GPIO_PORT GPIOB
#define MPU_GPIO_SDA_PIN GPIO_Pin_10
#define MPU_GPIO_SCL_PIN GPIO_Pin_11
#define MPU_GPIO_RCC_PORT RCC_APB2Periph_GPIOB
//IO方向设置
#define MODE_IN 1
#define MODE_OUT 0
#define MPU_SDA_IN()  {MPU_GPIO_MODESET(10,MODE_IN);}
#define MPU_SDA_OUT() {MPU_GPIO_MODESET(10,MODE_OUT);}


//#define SDA_IN()  {GPIOB->CRH&=0XFFFFFF0F;GPIOB->CRH|=8<<4;}
//#define SDA_OUT() {GPIOB->CRH&=0XFFFFFF0F;GPIOB->CRH|=3<<4;}

//IO操作函数	 //记得改！！
#define MPU_IIC_SCL     PBout(11) //SCL
#define MPU_IIC_SDA     PBout(10) //SDA	
#define MPU_READ_SDA   	PBin(10) //SDA	

//#define IIC_SCL    PBout(8) //SCL
//#define IIC_SDA    PBout(9) //SDA	 
//#define READ_SDA   PBin(9)  //输入SDA 



//IIC所有操作函数
void MPU_IIC_Delay(void);				//MPU IIC延时函数
void MPU_IIC_Init(void);                //初始化IIC的IO口				 
void MPU_IIC_Start(void);				//发送IIC开始信号
void MPU_IIC_Stop(void);	  			//发送IIC停止信号
void MPU_IIC_Send_Byte(u8 txd);			//IIC发送一个字节
u8 MPU_IIC_Read_Byte(unsigned char ack);//IIC读取一个字节
u8 MPU_IIC_Wait_Ack(void); 				//IIC等待ACK信号
void MPU_IIC_Ack(void);					//IIC发送ACK信号
void MPU_IIC_NAck(void);				//IIC不发送ACK信号

void MPU_GPIO_MODESET(uint8_t PINx,uint8_t INorOUT);
void IMPU_IC_Write_One_Byte(u8 daddr,u8 addr,u8 data);
u8 MPU_IIC_Read_One_Byte(u8 daddr,u8 addr);	  
#endif
















