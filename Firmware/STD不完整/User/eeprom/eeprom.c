#include "stm32f10x.h"
#include "eeprom.h"
#include "i2c.h"
#include "pid.h"
#include "blue_teeth.h"
#include "string.h"

/*
通讯协议：(uchar)
namelen		name..\0   	password..\0   baudrate  	vel_p vel_i vel_d bal_p bal_i bal_d turn_p turn_i turn_d  
		1		..<NAMEMAXLEN    	5   						1				4     4     4     4     4     4     4      4     4
*/
/*
typedef struct//8个
{
	float target_val;   //目标值
	float err;          //偏差值
	float err_last;     //上一个偏差值
	float integral;     //积分值
	float Kp,Ki,Kd;     //比例、积分、微分系数
	float output_val;   //输出值
}PID_Typedef;*/

static void EEPROMWriteFloat(float f,uint16_t *index)
{
	uint8_t c[sizeof(float)] = {0};
	int x = *index;
	for(int x=0;x<sizeof(float);x++)
	c[x] = *((uint8_t *)(&f)+x);
	EEPROM_WriteBytes(c ,x, sizeof(float));
	*index += sizeof(float);
}
static float EEPROMReadFloat(uint16_t *index)
{
	uint8_t c[sizeof(float)] = {0};
	int x = *index;
	EEPROM_ReadBytes(c ,x, sizeof(float));
	*index += sizeof(float);
	return *(float*)c;
}
void EEPROMReload(void)
{
	uint8_t index = 0,stringlen=0;
	EEPROM_ReadBytes(&stringlen, index, 1);
	index+=1;
	EEPROM_ReadBytes((uint8_t *)User_Data.CustomName ,index, stringlen);
	index+=stringlen;
	EEPROM_ReadBytes((uint8_t *)User_Data.PassWord ,index, 5);
	index+=5;
	velocity_PID.Kp = EEPROMReadFloat((uint16_t *)&index);
	velocity_PID.Ki = EEPROMReadFloat((uint16_t *)&index);
	velocity_PID.Kd = EEPROMReadFloat((uint16_t *)&index);
	balance_PID.Kp = EEPROMReadFloat((uint16_t *)&index);
	balance_PID.Ki = EEPROMReadFloat((uint16_t *)&index);
	balance_PID.Kd = EEPROMReadFloat((uint16_t *)&index);
	turn_PID.Kp = EEPROMReadFloat((uint16_t *)&index);
	turn_PID.Ki = EEPROMReadFloat((uint16_t *)&index);
	turn_PID.Kd = EEPROMReadFloat((uint16_t *)&index);
	BlueTeethConfig();
}
void EEPROMOutload(void)
{
	uint8_t index = 0,stringlen=0;
	stringlen = strlen((const char*)User_Data.CustomName)+1;
	EEPROM_WriteBytes(&stringlen,index, 1);
	index+=1;
	EEPROM_WriteBytes((uint8_t *)User_Data.CustomName ,index, stringlen);
	index+=stringlen;
	EEPROM_WriteBytes((uint8_t *)User_Data.PassWord ,index, 5);
	index+=5;
	EEPROM_WriteBytes((uint8_t *)&User_Data.Baudrate ,index, 1);
	index+=1;
	EEPROMWriteFloat(velocity_PID.Kp,(uint16_t *)&index);
	EEPROMWriteFloat(velocity_PID.Ki,(uint16_t *)&index);
	EEPROMWriteFloat(velocity_PID.Kd,(uint16_t *)&index);
	EEPROMWriteFloat(balance_PID.Kp,(uint16_t *)&index);
	EEPROMWriteFloat(balance_PID.Ki,(uint16_t *)&index);
	EEPROMWriteFloat(balance_PID.Kd,(uint16_t *)&index);
	EEPROMWriteFloat(turn_PID.Kp,(uint16_t *)&index);
	EEPROMWriteFloat(turn_PID.Ki,(uint16_t *)&index);
	EEPROMWriteFloat(turn_PID.Kd,(uint16_t *)&index);
}
void EE_Delay(void)//读写EEPROM交替中间要延迟，否则来不及反应导致起始信号虚发。
{
	uint32_t i = 0xffff;
	while(i--);
}
void EEPROM_Config(void)
{
	I2C_GPIO_Config(EEPROM_GPIO_PORT ,EEPROM_GPIO_RCC_CMD,EEPROM_GPIO_RCC_PORT,EEPROM_GPIO_SDA_PIN,EEPROM_GPIO_SCL_PIN);
}
uint8_t EEPROM_ReadBytes(uint8_t *_pReadBuf, uint16_t _usAddress, uint16_t _usSize)
{
	uint16_t i;
	i2c_Start(EEPROM_GPIO_PORT,EEPROM_GPIO_SDA_PIN,EEPROM_GPIO_SCL_PIN);
	
	i2c_SendByte(EEPROM_DEV_ADDR | I2C_WR,EEPROM_GPIO_PORT,EEPROM_GPIO_SDA_PIN,EEPROM_GPIO_SCL_PIN);	
	if (i2c_WaitAck(EEPROM_GPIO_PORT,EEPROM_GPIO_SDA_PIN,EEPROM_GPIO_SCL_PIN) != 0)
	{
		goto cmd_fail;
	}
	i2c_SendByte((uint8_t)_usAddress,EEPROM_GPIO_PORT,EEPROM_GPIO_SDA_PIN,EEPROM_GPIO_SCL_PIN);
	if (i2c_WaitAck(EEPROM_GPIO_PORT,EEPROM_GPIO_SDA_PIN,EEPROM_GPIO_SCL_PIN) != 0)
	{
		goto cmd_fail;
	}
	i2c_Start(EEPROM_GPIO_PORT,EEPROM_GPIO_SDA_PIN,EEPROM_GPIO_SCL_PIN);

	i2c_SendByte(EEPROM_DEV_ADDR | I2C_RD,EEPROM_GPIO_PORT,EEPROM_GPIO_SDA_PIN,EEPROM_GPIO_SCL_PIN);
	
	if (i2c_WaitAck(EEPROM_GPIO_PORT,EEPROM_GPIO_SDA_PIN,EEPROM_GPIO_SCL_PIN) != 0)
	{
		goto cmd_fail;
	}	
	for (i = 0; i < _usSize; i++)
	{
		_pReadBuf[i] = i2c_ReadByte(EEPROM_GPIO_PORT,EEPROM_GPIO_SDA_PIN,EEPROM_GPIO_SCL_PIN);
		if (i != _usSize - 1)
		{
			i2c_Ack(EEPROM_GPIO_PORT,EEPROM_GPIO_SDA_PIN,EEPROM_GPIO_SCL_PIN);
		}
		else
		{
			i2c_NAck(EEPROM_GPIO_PORT,EEPROM_GPIO_SDA_PIN,EEPROM_GPIO_SCL_PIN);
		}
	}
	i2c_Stop(EEPROM_GPIO_PORT,EEPROM_GPIO_SDA_PIN,EEPROM_GPIO_SCL_PIN);
	return 1;

cmd_fail: 
	i2c_Stop(EEPROM_GPIO_PORT,EEPROM_GPIO_SDA_PIN,EEPROM_GPIO_SCL_PIN);
	return 0;
}

uint8_t EEPROM_WriteBytes(uint8_t *_pWriteBuf, uint16_t _usAddress, uint16_t _usSize)
{
	uint16_t i,m;
	uint16_t usAddr;
	
	/* 
		写串行EEPROM不像读操作可以连续读取很多字节，每次写操作只能在同一个page。
		对于24xx02，page size = 8
		简单的处理方法为：按字节写操作模式，每写1个字节，都发送地址
		为了提高连续写的效率: 本函数采用page wirte操作。
	*/

	usAddr = _usAddress;	
	for (i = 0; i < _usSize; i++)
	{
		if ((i == 0) || (usAddr & (EEPROM_PAGE_SIZE - 1)) == 0)//开始或者开启新的8行时就重新发送一次地址
		{
			i2c_Stop(EEPROM_GPIO_PORT,EEPROM_GPIO_SDA_PIN,EEPROM_GPIO_SCL_PIN);
			
			/* 通过检查器件应答的方式，判断内部写操作是否完成, 一般小于 10ms 			
				CLK频率为200KHz时，查询次数为30次左右
			*/
			for (m = 0; m < 1000; m++)
			{				
				i2c_Start(EEPROM_GPIO_PORT,EEPROM_GPIO_SDA_PIN,EEPROM_GPIO_SCL_PIN);
				i2c_SendByte(EEPROM_DEV_ADDR | I2C_WR,EEPROM_GPIO_PORT,EEPROM_GPIO_SDA_PIN,EEPROM_GPIO_SCL_PIN);
				if (i2c_WaitAck(EEPROM_GPIO_PORT,EEPROM_GPIO_SDA_PIN,EEPROM_GPIO_SCL_PIN) == 0)
				{
					break;
				}
			}
			if (m  == 1000)
			{
				goto cmd_fail;
			}
			i2c_SendByte((uint8_t)usAddr,EEPROM_GPIO_PORT,EEPROM_GPIO_SDA_PIN,EEPROM_GPIO_SCL_PIN);
			if (i2c_WaitAck(EEPROM_GPIO_PORT,EEPROM_GPIO_SDA_PIN,EEPROM_GPIO_SCL_PIN) != 0)
			{
				goto cmd_fail;
			}
		}
		i2c_SendByte(_pWriteBuf[i],EEPROM_GPIO_PORT,EEPROM_GPIO_SDA_PIN,EEPROM_GPIO_SCL_PIN);
		if (i2c_WaitAck(EEPROM_GPIO_PORT,EEPROM_GPIO_SDA_PIN,EEPROM_GPIO_SCL_PIN) != 0)
		{
			goto cmd_fail;
		}
		usAddr++;		
	}
	i2c_Stop(EEPROM_GPIO_PORT,EEPROM_GPIO_SDA_PIN,EEPROM_GPIO_SCL_PIN);
	return 1;
cmd_fail: 
	i2c_Stop(EEPROM_GPIO_PORT,EEPROM_GPIO_SDA_PIN,EEPROM_GPIO_SCL_PIN);
	return 0;
}
