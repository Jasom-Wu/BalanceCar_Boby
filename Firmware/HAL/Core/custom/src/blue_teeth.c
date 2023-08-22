#include "blue_teeth.h"
#include "string.h"
#include "delay.h"
#include "eeprom.h"
#include "pid.h"
#include "usart.h"
#include "pwm.h"
#include "mpu6050.h"
_Bool BLUE_RXCONPLETED;
__IO char TDY_data[TDYMAXSIZE]={0};
__IO UserDataTypedef User_Data = {"Balance_Car","1234",8,0,0};//默认名字为"Blance_Car";密码为"1234";波特率为115200;速度环增量为0;转向环增量为0;
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
	 c[i] = RxBuffer2[x+i];
	*index += sizeof(float);
	return *(float*)c;
}
void BLUE_Config(void)
{
	__HAL_UART_DISABLE_IT(&huart2, UART_IT_IDLE);
	USART_DMARec_Stop(&huart2);//避免接受蓝牙反馈的信息
	DMA_Printf(USART2,"AT+NAME%s\r\n",User_Data.CustomName);
	delay_ms(100);
	DMA_Printf(USART2,"AT+PIN%s\r\n",User_Data.PassWord);	
	delay_ms(100);
	DMA_Printf(USART2,"AT+BAUD%d\r\n",User_Data.Baudrate);
	delay_ms(100);
	DMA_Printf(USART2,"AT+RESET\r\n");
	delay_ms(100);
	HAL_UART_Receive_DMA(&huart2,(uint8_t *)RxBuffer2,RXBUFFERSIZE);
	__HAL_UART_ENABLE_IT(&huart2, UART_IT_IDLE);
}
void BLUE_Process(void)
{
	uint8_t sum=0,flag=0;uint16_t x,index=1;
	if(RxBuffer2[0]==0xA5&&RxBuffer2[Rec_Length2-1]==0x5A)
	{
		for(x=1;x<Rec_Length2-2;x++)
			sum += RxBuffer2[x];
		if(sum==RxBuffer2[x])
		{
			//INT_FLAG=0;//禁用mpu中断
			//Set_Pwm(0,0);
			
			memset(&velocity_PID,0,sizeof(velocity_PID));
			User_Data.VelocityVal = RxBuffer2[index++];//这里注意char的编译器特性
			User_Data.TurnVal = RxBuffer2[index++];
			if(RxBuffer2[index++]==1)flag=1;
			//Gyroy_error =  *((short*)&RxBuffer2[index]);
			//index+=2;
			velocity_PID.Kp = ReadCharToFloat(&index);
			velocity_PID.Ki = ReadCharToFloat(&index);
			velocity_PID.Kd = ReadCharToFloat(&index);
			balance_PID.Kp = ReadCharToFloat(&index);
			balance_PID.Ki = ReadCharToFloat(&index);
			balance_PID.Kd = ReadCharToFloat(&index);
			turn_PID.Kp = ReadCharToFloat(&index);
			turn_PID.Ki = ReadCharToFloat(&index);
			turn_PID.Kd = ReadCharToFloat(&index);
			float temp = ReadCharToFloat(&index);
			if (Mechanical_balance != temp)
			{
				Mechanical_balance = temp;
			}
				
			if(flag==1)
			{
				//INT_FLAG=0;
				//Set_Pwm(0,0);
				EEPROMOutload();
				__HAL_TIM_SetCounter(&htim3,0);//这里一点要清定时器，否则编码器读出来的值会变得很大导致错误
				__HAL_TIM_SetCounter(&htim4,0);
				//INT_FLAG=1;
			}
			//PID调试（记得注释掉）
			//velocity_PID.target_val = 0;
			//delay_s(2);
			//__HAL_TIM_SetCounter(&htim3,0);//这里一点要清定时器，否则编码器读出来的值会变得很大导致错误
			//__HAL_TIM_SetCounter(&htim4,0);
			//INT_FLAG=1;
		}
	}
	if(memcmp(RxBuffer2,"AT+",3)==0)
	{
		if(memcmp(RxBuffer2,"AT+NAME",7)==0)
		{
			if(Rec_Length2 == 7)
				DMA_Printf(USART2,"小车名字为：\"%s\"\n",User_Data.CustomName);
			else
			{
				for(x=0;x<Rec_Length2-7;x++)
					User_Data.CustomName[x] = RxBuffer2[7+x];
				User_Data.CustomName[x] = '\0';
				DMA_Printf(USART2,"成功将小车名字修改为：\"%s\"\n",User_Data.CustomName);
			}
		}
		else if(memcmp(RxBuffer2,"AT+BAUD",7)==0)
		{
			if(Rec_Length2==7)DMA_Printf(USART2,"小车蓝牙波特率序号为\"%d\"\n波特率选择\n4：9600\n5：19200\n6：38400\n7：57600\n8：115200\n9：128000",User_Data.Baudrate);
			else if(Rec_Length2==8)
			User_Data.Baudrate = RxBuffer2[8];
		}
		else if(memcmp(RxBuffer2,"AT+PIN",6)==0)
		{
			if(Rec_Length2==6)DMA_Printf(USART2,"小车蓝牙密码为：\"%s\"\n",User_Data.PassWord);
			else if(Rec_Length2 == 10)
			{
				for(x=0;x<Rec_Length2-6;x++)
				User_Data.PassWord[x] = RxBuffer2[6+x];
				User_Data.PassWord[x] = '\0';
				DMA_Printf(USART2,"成功将小车蓝牙密码修改为：\"%s\"\n",User_Data.PassWord);
			}
			else 
			{
				DMA_Printf(USART2,"【错误】密码只能四位数！\n");	
			}
		}
		else if(memcmp(RxBuffer2,"AT+RELOAD",9)==0)
		{
			EEPROMReload();
			DMA_Printf(USART2,"重加载成功\n");
		}
		else if(memcmp(RxBuffer2,"AT+OUTLOAD",10)==0)
		{
			EEPROMOutload();
			DMA_Printf(USART2,"系统数据备份成功\n");
		}
		else if(memcmp(RxBuffer2,"AT+DISC",7)==0)
		{
			DMA_Printf(USART2,"蓝牙更新成功\n");
			delay_ms(100);
			DMA_Printf(USART2,"蓝牙断开成功\n");
			delay_ms(100);
			DMA_Printf(USART2,"AT+DISC\r\n");
			delay_ms(100);
			BLUE_Config();
		}
	}
}
