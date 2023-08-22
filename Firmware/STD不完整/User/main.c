#include "stm32f10x.h"
#include "delay.h"
#include "bsp_usart.h"
#include "timer.h"
#include "encoder.h"
#include "oled.h"
#include "pwm.h"
#include "mpu6050.h"
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h" 
#include "eeprom.h"
#include "ultra.h"
#include "nvic_config.h"

float pitch,roll,yaw; 		//ŷ����
short gyrox,gyroy,gyroz;	//������ԭʼ����

int main(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);/* Ƕ�������жϿ�������ѡ�� */
	delay_init();   //��ʼ���ӳٺ���
	USART_Config();
	USART_NVIC_Configuration();
	UltraConfig();
	Timer_Init();
	//OLED_Config();
	//BULECarInitDisplay();
	//EEPROM_Config();
	//EEPROMReload();
	Encoder_Init();
	Motor_Init();
	printf("%d",MPU_Init()); //��ʼ��MPU6050	
	printf("%d",mpu_dmp_init());
	NVIC_Config();
	
	//Set_Pwm(5000,5000);
	while(1)
	{
		
	}
}

