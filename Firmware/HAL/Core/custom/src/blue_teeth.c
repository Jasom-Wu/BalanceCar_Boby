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
__IO UserDataTypedef User_Data = {"Balance_Car","1234",8,0,0};//Ĭ������Ϊ"Blance_Car";����Ϊ"1234";������Ϊ115200;�ٶȻ�����Ϊ0;ת������Ϊ0;
/*
������ѡ��
4��9600
5��19200
6��38400
7��57600
8��115200
9��128000
*/
/*
ͨѶЭ�飺(char)
vel_val turn_val	is_outload	vel_p vel_i vel_d bal_p bal_i bal_d turn_p turn_i turn_d  ��39
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
	USART_DMARec_Stop(&huart2);//�������������������Ϣ
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
			//INT_FLAG=0;//����mpu�ж�
			//Set_Pwm(0,0);
			
			memset(&velocity_PID,0,sizeof(velocity_PID));
			User_Data.VelocityVal = RxBuffer2[index++];//����ע��char�ı���������
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
				__HAL_TIM_SetCounter(&htim3,0);//����һ��Ҫ�嶨ʱ���������������������ֵ���úܴ��´���
				__HAL_TIM_SetCounter(&htim4,0);
				//INT_FLAG=1;
			}
			//PID���ԣ��ǵ�ע�͵���
			//velocity_PID.target_val = 0;
			//delay_s(2);
			//__HAL_TIM_SetCounter(&htim3,0);//����һ��Ҫ�嶨ʱ���������������������ֵ���úܴ��´���
			//__HAL_TIM_SetCounter(&htim4,0);
			//INT_FLAG=1;
		}
	}
	if(memcmp(RxBuffer2,"AT+",3)==0)
	{
		if(memcmp(RxBuffer2,"AT+NAME",7)==0)
		{
			if(Rec_Length2 == 7)
				DMA_Printf(USART2,"С������Ϊ��\"%s\"\n",User_Data.CustomName);
			else
			{
				for(x=0;x<Rec_Length2-7;x++)
					User_Data.CustomName[x] = RxBuffer2[7+x];
				User_Data.CustomName[x] = '\0';
				DMA_Printf(USART2,"�ɹ���С�������޸�Ϊ��\"%s\"\n",User_Data.CustomName);
			}
		}
		else if(memcmp(RxBuffer2,"AT+BAUD",7)==0)
		{
			if(Rec_Length2==7)DMA_Printf(USART2,"С���������������Ϊ\"%d\"\n������ѡ��\n4��9600\n5��19200\n6��38400\n7��57600\n8��115200\n9��128000",User_Data.Baudrate);
			else if(Rec_Length2==8)
			User_Data.Baudrate = RxBuffer2[8];
		}
		else if(memcmp(RxBuffer2,"AT+PIN",6)==0)
		{
			if(Rec_Length2==6)DMA_Printf(USART2,"С����������Ϊ��\"%s\"\n",User_Data.PassWord);
			else if(Rec_Length2 == 10)
			{
				for(x=0;x<Rec_Length2-6;x++)
				User_Data.PassWord[x] = RxBuffer2[6+x];
				User_Data.PassWord[x] = '\0';
				DMA_Printf(USART2,"�ɹ���С�����������޸�Ϊ��\"%s\"\n",User_Data.PassWord);
			}
			else 
			{
				DMA_Printf(USART2,"����������ֻ����λ����\n");	
			}
		}
		else if(memcmp(RxBuffer2,"AT+RELOAD",9)==0)
		{
			EEPROMReload();
			DMA_Printf(USART2,"�ؼ��سɹ�\n");
		}
		else if(memcmp(RxBuffer2,"AT+OUTLOAD",10)==0)
		{
			EEPROMOutload();
			DMA_Printf(USART2,"ϵͳ���ݱ��ݳɹ�\n");
		}
		else if(memcmp(RxBuffer2,"AT+DISC",7)==0)
		{
			DMA_Printf(USART2,"�������³ɹ�\n");
			delay_ms(100);
			DMA_Printf(USART2,"�����Ͽ��ɹ�\n");
			delay_ms(100);
			DMA_Printf(USART2,"AT+DISC\r\n");
			delay_ms(100);
			BLUE_Config();
		}
	}
}
