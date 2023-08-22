#include "i2c.h"
#include "stm32f10x.h"
#include "delay.h"
void I2C_GPIO_Config(GPIO_TypeDef* PORTX ,RCC_ClockCmd CMD ,uint32_t RCC_PORT,uint16_t GPIO_SDA_PIN_X,uint16_t GPIO_SCL_PIN_X)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	CMD(RCC_PORT,ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_SDA_PIN_X | GPIO_SCL_PIN_X;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;  	/* ��©��� */
	GPIO_Init(PORTX, &GPIO_InitStructure);

	i2c_Stop(PORTX,GPIO_SDA_PIN_X,GPIO_SCL_PIN_X);
}

uint8_t i2c_CheckDevice(uint8_t _Address,GPIO_TypeDef* PORTX, uint16_t GPIO_SDA_PIN_X,uint16_t GPIO_SCL_PIN_X)
{
	uint8_t ucAck;

	i2c_Start(PORTX,GPIO_SDA_PIN_X,GPIO_SCL_PIN_X);		/* ���������ź� */

	/* �����豸��ַ+��д����bit��0 = w�� 1 = r) bit7 �ȴ� */
	i2c_SendByte(_Address | I2C_WR,PORTX,GPIO_SDA_PIN_X,GPIO_SCL_PIN_X);
	ucAck = i2c_WaitAck(PORTX,GPIO_SDA_PIN_X,GPIO_SCL_PIN_X);	/* ����豸��ACKӦ�� */

	i2c_Stop(PORTX,GPIO_SDA_PIN_X,GPIO_SCL_PIN_X);			/* ����ֹͣ�ź� */

	return ucAck;
}
//=====================������I2C�ľ��嶯��===================
static void i2c_Delay(void)
{
	//uint8_t i;

	/*��
	 	�����ʱ����ͨ���߼������ǲ��Եõ��ġ�
    ����������CPU��Ƶ72MHz ��MDK���뻷����1���Ż�
  
		ѭ������Ϊ10ʱ��SCLƵ�� = 205KHz 
		ѭ������Ϊ7ʱ��SCLƵ�� = 347KHz�� SCL�ߵ�ƽʱ��1.5us��SCL�͵�ƽʱ��2.87us 
	 	ѭ������Ϊ5ʱ��SCLƵ�� = 421KHz�� SCL�ߵ�ƽʱ��1.25us��SCL�͵�ƽʱ��2.375us 
	*/
	//for (i = 0; i < 7; i++);
	delay_us(2);
}

void i2c_Start(GPIO_TypeDef* PORTX, uint16_t GPIO_SDA_PIN_X,uint16_t GPIO_SCL_PIN_X)
{
	/* ��SCL�ߵ�ƽʱ��SDA����һ�������ر�ʾI2C���������ź� */
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
	/* ��SCL�ߵ�ƽʱ��SDA����һ�������ر�ʾI2C����ֹͣ�ź� */
	I2C_GPIOSET_0(PORTX,GPIO_SDA_PIN_X);
	I2C_GPIOSET_1(PORTX,GPIO_SCL_PIN_X);
	i2c_Delay();
	I2C_GPIOSET_1(PORTX,GPIO_SDA_PIN_X);
}

void i2c_SendByte(uint8_t _ucByte,GPIO_TypeDef* PORTX, uint16_t GPIO_SDA_PIN_X,uint16_t GPIO_SCL_PIN_X)
{
	uint8_t i;

	/* �ȷ����ֽڵĸ�λbit7 */
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
			 I2C_GPIOSET_1(PORTX,GPIO_SDA_PIN_X); // �ͷ�����
		}
		_ucByte <<= 1;	/* ����һ��bit */
		i2c_Delay();
	}
}

uint8_t i2c_ReadByte(GPIO_TypeDef* PORTX, uint16_t GPIO_SDA_PIN_X,uint16_t GPIO_SCL_PIN_X)
{
	uint8_t i;
	uint8_t value;

	/* ������1��bitΪ���ݵ�bit7 */
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

uint8_t i2c_WaitAck(GPIO_TypeDef* PORTX, uint16_t GPIO_SDA_PIN_X,uint16_t GPIO_SCL_PIN_X)//���ʹ�������Ӧ��������0
{
	uint8_t re;

	I2C_GPIOSET_1(PORTX,GPIO_SDA_PIN_X);	/* CPU�ͷ�SDA���� */
	i2c_Delay();
	I2C_GPIOSET_1(PORTX,GPIO_SCL_PIN_X);	/* CPU����SCL = 1, ��ʱ�����᷵��ACKӦ�� */
	i2c_Delay();
	if (I2C_SDA_READ(PORTX,GPIO_SDA_PIN_X))	/* CPU��ȡSDA����״̬ */
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
	I2C_GPIOSET_0(PORTX,GPIO_SDA_PIN_X);	/* CPU����SDA = 0 */
	i2c_Delay();
	I2C_GPIOSET_1(PORTX,GPIO_SCL_PIN_X);	/* CPU����1��ʱ�� */
	i2c_Delay();
	I2C_GPIOSET_0(PORTX,GPIO_SCL_PIN_X);
	i2c_Delay();
	I2C_GPIOSET_1(PORTX,GPIO_SDA_PIN_X);	/* CPU�ͷ�SDA���� */
}

void i2c_NAck(GPIO_TypeDef* PORTX, uint16_t GPIO_SDA_PIN_X,uint16_t GPIO_SCL_PIN_X)
{
	I2C_GPIOSET_1(PORTX,GPIO_SDA_PIN_X);	/* CPU����SDA = 1 */
	i2c_Delay();
	I2C_GPIOSET_1(PORTX,GPIO_SCL_PIN_X);	/* CPU����1��ʱ�� */
	i2c_Delay();
	I2C_GPIOSET_0(PORTX,GPIO_SCL_PIN_X);
	i2c_Delay();	
}
