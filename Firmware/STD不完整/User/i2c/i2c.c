#include "i2c.h"
#include "stm32f10x.h"
#include "delay.h"
void I2C_GPIO_Config(GPIO_TypeDef* PORTX ,RCC_ClockCmd CMD ,uint32_t RCC_PORT,uint16_t GPIO_SDA_PIN_X,uint16_t GPIO_SCL_PIN_X)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	CMD(RCC_PORT,ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_SDA_PIN_X | GPIO_SCL_PIN_X;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;  	/* 开漏输出 */
	GPIO_Init(PORTX, &GPIO_InitStructure);

	i2c_Stop(PORTX,GPIO_SDA_PIN_X,GPIO_SCL_PIN_X);
}

uint8_t i2c_CheckDevice(uint8_t _Address,GPIO_TypeDef* PORTX, uint16_t GPIO_SDA_PIN_X,uint16_t GPIO_SCL_PIN_X)
{
	uint8_t ucAck;

	i2c_Start(PORTX,GPIO_SDA_PIN_X,GPIO_SCL_PIN_X);		/* 发送启动信号 */

	/* 发送设备地址+读写控制bit（0 = w， 1 = r) bit7 先传 */
	i2c_SendByte(_Address | I2C_WR,PORTX,GPIO_SDA_PIN_X,GPIO_SCL_PIN_X);
	ucAck = i2c_WaitAck(PORTX,GPIO_SDA_PIN_X,GPIO_SCL_PIN_X);	/* 检测设备的ACK应答 */

	i2c_Stop(PORTX,GPIO_SDA_PIN_X,GPIO_SCL_PIN_X);			/* 发送停止信号 */

	return ucAck;
}
//=====================以下是I2C的具体动作===================
static void i2c_Delay(void)
{
	//uint8_t i;

	/*　
	 	下面的时间是通过逻辑分析仪测试得到的。
    工作条件：CPU主频72MHz ，MDK编译环境，1级优化
  
		循环次数为10时，SCL频率 = 205KHz 
		循环次数为7时，SCL频率 = 347KHz， SCL高电平时间1.5us，SCL低电平时间2.87us 
	 	循环次数为5时，SCL频率 = 421KHz， SCL高电平时间1.25us，SCL低电平时间2.375us 
	*/
	//for (i = 0; i < 7; i++);
	delay_us(2);
}

void i2c_Start(GPIO_TypeDef* PORTX, uint16_t GPIO_SDA_PIN_X,uint16_t GPIO_SCL_PIN_X)
{
	/* 当SCL高电平时，SDA出现一个下跳沿表示I2C总线启动信号 */
	I2C_GPIOSET_1(PORTX,GPIO_SDA_PIN_X);
	I2C_GPIOSET_1(PORTX,GPIO_SCL_PIN_X);
	i2c_Delay();
	I2C_GPIOSET_0(PORTX,GPIO_SDA_PIN_X);
	i2c_Delay();
	I2C_GPIOSET_0(PORTX,GPIO_SCL_PIN_X);
	i2c_Delay();
}

void i2c_Stop(GPIO_TypeDef* PORTX, uint16_t GPIO_SDA_PIN_X,uint16_t GPIO_SCL_PIN_X)
{
	/* 当SCL高电平时，SDA出现一个上跳沿表示I2C总线停止信号 */
	I2C_GPIOSET_0(PORTX,GPIO_SDA_PIN_X);
	I2C_GPIOSET_1(PORTX,GPIO_SCL_PIN_X);
	i2c_Delay();
	I2C_GPIOSET_1(PORTX,GPIO_SDA_PIN_X);
}

void i2c_SendByte(uint8_t _ucByte,GPIO_TypeDef* PORTX, uint16_t GPIO_SDA_PIN_X,uint16_t GPIO_SCL_PIN_X)
{
	uint8_t i;

	/* 先发送字节的高位bit7 */
	for (i = 0; i < 8; i++)
	{		
		if (_ucByte & 0x80)
		{
			I2C_GPIOSET_1(PORTX,GPIO_SDA_PIN_X);
		}
		else
		{
			I2C_GPIOSET_0(PORTX,GPIO_SDA_PIN_X);
		}
		i2c_Delay();
		I2C_GPIOSET_1(PORTX,GPIO_SCL_PIN_X);
		i2c_Delay();	
		I2C_GPIOSET_0(PORTX,GPIO_SCL_PIN_X);
		if (i == 7)
		{
			 I2C_GPIOSET_1(PORTX,GPIO_SDA_PIN_X); // 释放总线
		}
		_ucByte <<= 1;	/* 左移一个bit */
		i2c_Delay();
	}
}

uint8_t i2c_ReadByte(GPIO_TypeDef* PORTX, uint16_t GPIO_SDA_PIN_X,uint16_t GPIO_SCL_PIN_X)
{
	uint8_t i;
	uint8_t value;

	/* 读到第1个bit为数据的bit7 */
	value = 0;
	for (i = 0; i < 8; i++)
	{
		value <<= 1;
		I2C_GPIOSET_1(PORTX,GPIO_SCL_PIN_X);
		i2c_Delay();
		if (I2C_SDA_READ(PORTX,GPIO_SDA_PIN_X))
		{
			value++;
		}
		I2C_GPIOSET_0(PORTX,GPIO_SCL_PIN_X);
		i2c_Delay();
	}
	return value;
}

uint8_t i2c_WaitAck(GPIO_TypeDef* PORTX, uint16_t GPIO_SDA_PIN_X,uint16_t GPIO_SCL_PIN_X)//拉低代表有响应，即返回0
{
	uint8_t re;

	I2C_GPIOSET_1(PORTX,GPIO_SDA_PIN_X);	/* CPU释放SDA总线 */
	i2c_Delay();
	I2C_GPIOSET_1(PORTX,GPIO_SCL_PIN_X);	/* CPU驱动SCL = 1, 此时器件会返回ACK应答 */
	i2c_Delay();
	if (I2C_SDA_READ(PORTX,GPIO_SDA_PIN_X))	/* CPU读取SDA口线状态 */
	{
		re = 1;
	}
	else
	{
		re = 0;
	}
	I2C_GPIOSET_0(PORTX,GPIO_SCL_PIN_X);
	i2c_Delay();
	return re;
}

void i2c_Ack(GPIO_TypeDef* PORTX, uint16_t GPIO_SDA_PIN_X,uint16_t GPIO_SCL_PIN_X)
{
	I2C_GPIOSET_0(PORTX,GPIO_SDA_PIN_X);	/* CPU驱动SDA = 0 */
	i2c_Delay();
	I2C_GPIOSET_1(PORTX,GPIO_SCL_PIN_X);	/* CPU产生1个时钟 */
	i2c_Delay();
	I2C_GPIOSET_0(PORTX,GPIO_SCL_PIN_X);
	i2c_Delay();
	I2C_GPIOSET_1(PORTX,GPIO_SDA_PIN_X);	/* CPU释放SDA总线 */
}

void i2c_NAck(GPIO_TypeDef* PORTX, uint16_t GPIO_SDA_PIN_X,uint16_t GPIO_SCL_PIN_X)
{
	I2C_GPIOSET_1(PORTX,GPIO_SDA_PIN_X);	/* CPU驱动SDA = 1 */
	i2c_Delay();
	I2C_GPIOSET_1(PORTX,GPIO_SCL_PIN_X);	/* CPU产生1个时钟 */
	i2c_Delay();
	I2C_GPIOSET_0(PORTX,GPIO_SCL_PIN_X);
	i2c_Delay();	
}
