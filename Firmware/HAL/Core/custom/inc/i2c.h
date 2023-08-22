#ifndef __I2C_H_
#define __I2C_H_
#include "stm32f1xx_hal.h"
//=======================================================================================================
#define I2C_WR	0		/* 写控制bit */
#define I2C_RD	1		/* 读控制bit */
    /*　注意：如下写法，在IAR最高级别优化时，会被编译器错误优化 */
	#define I2C_GPIOSET_1(PORT,PIN)  PORT->BSRR = PIN				/* SCL/SDA = 1 */
	#define I2C_GPIOSET_0(PORT,PIN)  PORT->BRR = PIN				/* SCL/SDA = 0 */
	#define I2C_SDA_READ(PORT,PIN)  ((PORT->IDR & PIN) != 0)	/* 读SDA口线状态 */
//=======================================================================================================
/****************
注意：模拟IIC时，任意IO口（除JTAG口外，都可以做SDA和SCL）
****************/
//以下为应用函数
void i2c_SendByte(uint8_t _ucByte,GPIO_TypeDef* PORTX, uint16_t GPIO_SDA_PIN_X,uint16_t GPIO_SCL_PIN_X);
uint8_t i2c_ReadByte(GPIO_TypeDef* PORTX, uint16_t GPIO_SDA_PIN_X,uint16_t GPIO_SCL_PIN_X);
uint8_t i2c_CheckDevice(uint8_t _Address,GPIO_TypeDef* PORTX, uint16_t GPIO_SDA_PIN_X,uint16_t GPIO_SCL_PIN_X);
void I2C_GPIO_Config(GPIO_TypeDef* PORTX ,uint16_t GPIO_SDA_PIN_X,uint16_t GPIO_SCL_PIN_X);
//以下为驱动函数
void i2c_Start(GPIO_TypeDef* PORTX, uint16_t GPIO_SDA_PIN_X,uint16_t GPIO_SCL_PIN_X);
void i2c_Stop(GPIO_TypeDef* PORTX, uint16_t GPIO_SDA_PIN_X,uint16_t GPIO_SCL_PIN_X);
uint8_t i2c_WaitAck(GPIO_TypeDef* PORTX, uint16_t GPIO_SDA_PIN_X,uint16_t GPIO_SCL_PIN_X);
void i2c_Ack(GPIO_TypeDef* PORTX, uint16_t GPIO_SDA_PIN_X,uint16_t GPIO_SCL_PIN_X);
void i2c_NAck(GPIO_TypeDef* PORTX, uint16_t GPIO_SDA_PIN_X,uint16_t GPIO_SCL_PIN_X);

#endif
