#ifndef __I2C_H_
#define __I2C_H_
#include "stm32f10x.h"
//=======================================================================================================
#define I2C_WR	0		/* д����bit */
#define I2C_RD	1		/* ������bit */
    /*��ע�⣺����д������IAR��߼����Ż�ʱ���ᱻ�����������Ż� */
	#define I2C_GPIOSET_1(PORT,PIN)  PORT->BSRR = PIN				/* SCL/SDA = 1 */
	#define I2C_GPIOSET_0(PORT,PIN)  PORT->BRR = PIN				/* SCL/SDA = 0 */
	#define I2C_SDA_READ(PORT,PIN)  ((PORT->IDR & PIN) != 0)	/* ��SDA����״̬ */
//=======================================================================================================
/****************
ע�⣺ģ��IICʱ������IO�ڣ���JTAG���⣬��������SDA��SCL��
****************/
#ifndef __RCC_ClockCmdType_
#define	__RCC_ClockCmdType_
typedef void (*RCC_ClockCmd)(uint32_t, FunctionalState);
#endif /*__RCC_ClockCmdType_*/
//����ΪӦ�ú���
void i2c_SendByte(uint8_t _ucByte,GPIO_TypeDef* PORTX, uint16_t GPIO_SDA_PIN_X,uint16_t GPIO_SCL_PIN_X);
uint8_t i2c_ReadByte(GPIO_TypeDef* PORTX, uint16_t GPIO_SDA_PIN_X,uint16_t GPIO_SCL_PIN_X);
uint8_t i2c_CheckDevice(uint8_t _Address,GPIO_TypeDef* PORTX, uint16_t GPIO_SDA_PIN_X,uint16_t GPIO_SCL_PIN_X);
void I2C_GPIO_Config(GPIO_TypeDef* PORTX ,RCC_ClockCmd CMD ,uint32_t RCC_PORT,uint16_t GPIO_SDA_PIN_X,uint16_t GPIO_SCL_PIN_X);
//����Ϊ��������
void i2c_Start(GPIO_TypeDef* PORTX, uint16_t GPIO_SDA_PIN_X,uint16_t GPIO_SCL_PIN_X);
void i2c_Stop(GPIO_TypeDef* PORTX, uint16_t GPIO_SDA_PIN_X,uint16_t GPIO_SCL_PIN_X);
uint8_t i2c_WaitAck(GPIO_TypeDef* PORTX, uint16_t GPIO_SDA_PIN_X,uint16_t GPIO_SCL_PIN_X);
void i2c_Ack(GPIO_TypeDef* PORTX, uint16_t GPIO_SDA_PIN_X,uint16_t GPIO_SCL_PIN_X);
void i2c_NAck(GPIO_TypeDef* PORTX, uint16_t GPIO_SDA_PIN_X,uint16_t GPIO_SCL_PIN_X);

#endif
