#include "blue_teeth.h"
#include "bsp_usart.h"
#include "string.h"
#include "delay.h"
#include "eeprom.h"
#include "pid.h"
_Bool BLUE_RXCONPLETED;
__IO char TDY_data[TDYMAXSIZE]={0};
__IO UserDataTypedef User_Data = {"Blance_Car","1234",8,0,0};//默认名字为"Blance_Car";密码为"1234";波特率为115200;速度环增量为0;转向环增量为0;
/*
波特率选择
4：9600
5：19200
6：38400
7：57600
8：115200
9：128000
*/
/*
通讯协议：(char)
vel_val turn_val	is_outload	vel_p vel_i vel_d bal_p bal_i bal_d turn_p turn_i turn_d  共39
	 1    		1					1						4     4     4     4     4     4     4      4     4
*/
static float ReadCharToFloat(uint16_t *index)
{
	uint8_t c[sizeof(float)] = {0};
	uint16_t x = *index,i;
	for(i=0;i<sizeof(float);i++)
	 c[i] = TDY_data[x+i];
	*index += sizeof(float);
	return *(float*)c;
}
void BlueTeethConfig(void)
{
	Printf(USART2,"AT+NAME%s\r\n",User_Data.CustomName);
	delay_ms(100);	
	Printf(USART2,"AT+PIN%s\r\n",User_Data.PassWord);
	delay_ms(100);		
	Printf(USART2,"AT+BAUD%d\r\n",User_Data.Baudrate);
}
int BlueTeethProcess(void)
{
	char temp,sum=0;
	char temparr[TDYMAXSIZE]={0};
	static char buff[TDYBUFFLEN]={0};
	static uint16_t count=0,buff_count=0; 
	uint16_t x,i,index = 0;
	temp = Getchar(USART2);
	if(temp == 0xA5)
	{
		count = 0;
	}
	else if(temp != 0xA5)
	{
			if(temp != 0x5A)
			{
				temparr[count] = temp;
				count++;
			}
			else if(count !=0)
			{
				for(x=0;x<count-1;x++)
				sum+=temparr[x];
				if(temparr[count-1] == sum)
				{
					memcpy((char *)TDY_data,temparr,count*sizeof(char));
					User_Data.VelocityVal = TDY_data[index++];
					User_Data.VelocityVal = TDY_data[index++];
					if(TDY_data[index++]==1)EEPROMReload();
					velocity_PID.Kp = ReadCharToFloat(&index);
					velocity_PID.Ki = ReadCharToFloat(&index);
					velocity_PID.Kd = ReadCharToFloat(&index);
					balance_PID.Kp = ReadCharToFloat(&index);
					balance_PID.Ki = ReadCharToFloat(&index);
					balance_PID.Kd = ReadCharToFloat(&index);
					turn_PID.Kp = ReadCharToFloat(&index);
					turn_PID.Ki = ReadCharToFloat(&index);
					turn_PID.Kd = ReadCharToFloat(&index);
					BLUE_RXCONPLETED = 1;
					return count;
				}
				
			}
	}
	if(temp!= '\n')
	{
		if(temp != '\r')
		{
				buff[buff_count++] = temp;
				if(buff_count ==3)
				{
					if(memcmp(buff,"AT+",3)!=0)
						buff_count = 0;	
				}
				else if(buff_count >=6)
				{
					if(memcmp(buff,"AT+NAME",7)!=0&&memcmp(buff,"AT+BAUD",7)!=0&&memcmp(buff,"AT+PIN",6)!=0)
						buff_count = 0;
				}
		}
		else if(temp == '\r'&&buff_count >=6)
		{
				if(memcmp(buff,"AT+NAME",7)==0)
				{
					if(buff_count ==7)Printf(USART1,"小车名字为：\"%s\"\n",User_Data.CustomName);	
					else if(buff_count!=7)
					{
						for(i=0;i<buff_count-7;i++)
						User_Data.CustomName[i] = buff[7+i];
						User_Data.CustomName[i] = '\0';
					}
				}
				else if(memcmp(buff,"AT+BAUD",7)==0)
				{
					if(buff_count==7)Printf(USART1,"波特率选择\n4：9600\n5：19200\n6：38400\n7：57600\n8：115200\n9：128000");
					else if(buff_count==8)
					User_Data.Baudrate = buff[8];
				}
				else if(memcmp(buff,"AT+PIN",6)==0)
				{
					if(buff_count !=10)Printf(USART1,"【错误】密码只能四位数！\n");	
					else if(buff_count == 10)
					{
						for(i=0;i<buff_count-6;i++)
						User_Data.PassWord[i] = buff[6+i];
						User_Data.PassWord[i] = '\0';
					}
				}
				buff_count = 0;
		}
		else if(temp == '\r'&&buff_count <6)
			buff_count = 0;
	}
	return 0;
}
