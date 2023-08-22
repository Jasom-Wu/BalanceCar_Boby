#include "bsp_usart.h"
#include "stdarg.h"
static uint8_t BSP_USART_FLAG = 1;//1��ʾ����1�������ģʽ��2��ʾ����2�������ģʽ���������ࡣ

//�������´���,֧��printf����,������Ҫѡ��use MicroLIB
#if 0
#pragma import(__use_no_semihosting)             
//��׼����Ҫ��֧�ֺ���                 
struct __FILE 
{ 
	int handle; 

}; 

FILE __stdout;       
//����_sys_exit()�Ա���ʹ�ð�����ģʽ    
_sys_exit(int x) 
{ 
	x = x; 
} 
//�ض���fputc���� 
int fputc(int ch, FILE *f)
{      
	while((USART1->SR&0X40)==0);//ѭ������,ֱ���������   
    USART1->DR = (u8) ch;      
	return ch;
}
#endif 

void USART_NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  NVIC_InitStructure.NVIC_IRQChannel = DEBUG_USART1_IRQ; /* ����USARTΪ�ж�Դ */
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;/* �������ȼ�*/
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;/* �����ȼ� */
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; /* ʹ���ж� */
  NVIC_Init(&NVIC_InitStructure);/* ��ʼ������NVIC */
	
	NVIC_InitStructure.NVIC_IRQChannel = DEBUG_USART2_IRQ;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_Init(&NVIC_InitStructure);
}

void USART_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	DEBUG_USART1_GPIO_APBxClkCmd(DEBUG_USART1_GPIO_CLK, ENABLE);
	DEBUG_USART1_APBxClkCmd(DEBUG_USART1_CLK, ENABLE);
	DEBUG_USART2_GPIO_APBxClkCmd(DEBUG_USART2_GPIO_CLK, ENABLE);
	DEBUG_USART2_APBxClkCmd(DEBUG_USART2_CLK, ENABLE);
	// ��USART Tx��GPIO����Ϊ���츴��ģʽ
	GPIO_InitStructure.GPIO_Pin = DEBUG_USART1_TX_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(DEBUG_USART1_TX_GPIO_PORT, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = DEBUG_USART2_TX_GPIO_PIN;
	GPIO_Init(DEBUG_USART2_TX_GPIO_PORT, &GPIO_InitStructure);
	
  // ��USART Rx��GPIO����Ϊ��������ģʽ
	GPIO_InitStructure.GPIO_Pin = DEBUG_USART1_RX_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(DEBUG_USART1_RX_GPIO_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = DEBUG_USART2_RX_GPIO_PIN;
	GPIO_Init(DEBUG_USART2_RX_GPIO_PORT, &GPIO_InitStructure);
	//��ӳ��USART2����
	//RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	//GPIO_PinRemapConfig(GPIO_Remap_USART2,ENABLE);
	
	USART_InitStructure.USART_BaudRate = DEBUG_USART1_BAUDRATE;// ���ò�����
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;// ���� �������ֳ�
	USART_InitStructure.USART_StopBits = USART_StopBits_1;// ����ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No ;// ����У��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;// ����Ӳ��������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;// ���ù���ģʽ���շ�һ��
	
	USART_Init(USART1, &USART_InitStructure);
	USART_InitStructure.USART_BaudRate = DEBUG_USART2_BAUDRATE;// ���ò�����
	USART_Init(USART2, &USART_InitStructure);
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);	//RX�ж���ʱ���ж�����
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);	//RX�ж���ʱ���ж�����
	
	USART_Cmd(USART1, ENABLE);
	USART_Cmd(USART2, ENABLE);	
	//�����������1���ֽ��޷���ȷ���ͳ�ȥ������
	  USART_ClearFlag(USART1, USART_FLAG_TC);        //�崮��1���ͱ�־
		USART_ClearFlag(USART2, USART_FLAG_TC);        //�崮��2���ͱ�־
}


/* ����һ���ֽ� */
void Usart_SendByte(USART_TypeDef* pUSARTx, uint8_t data)
{
	USART_SendData(pUSARTx, data);
	while( USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET );//���TX�Ĵ����Ƿ��գ������޽�����λ�Ĵ��������޷���0
}

/* ���������ֽڵ����� */
void Usart_SendHalfWord(USART_TypeDef* pUSARTx, uint16_t data)
{
	uint8_t temp_h,temp_l;
	
	temp_h = (data&0xff00) >> 8 ;
	temp_l = data&0xff;
	
	USART_SendData(pUSARTx, temp_h);
	while( USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET );
	
	USART_SendData(pUSARTx, temp_l);
	while( USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET );
}

/* ����8λ���ݵ����� */
void Usart_SendArray(USART_TypeDef* pUSARTx, uint8_t *array,uint8_t num)
{
	uint8_t i;
	for( i=0; i<num; i++ )
  {
		Usart_SendByte(pUSARTx, array[i]);
	}
	while( USART_GetFlagStatus(pUSARTx, USART_FLAG_TC) == RESET );
}

/* �����ַ��� */
void Usart_SendStr(USART_TypeDef* pUSARTx, uint8_t *str)
{
	uint8_t i=0;
	do
  {
		Usart_SendByte(pUSARTx, *(str+i));
		i++;
	}while(*(str+i) != '\0');
	while( USART_GetFlagStatus(pUSARTx, USART_FLAG_TC) == RESET );
}
//����BSP_USART_FLAG��������������ض���printf����

int Printf(USART_TypeDef *USARTx,char *formatString,...)
{
	char TXBuffer[TX_DATA_SIZE];
	int x;int length;
	va_list args;
	va_start(args,formatString);
	length = vsnprintf(TXBuffer,TX_DATA_SIZE,formatString,args);
	if(USARTx == USART1)
			BSP_USART_FLAG = 1;
	else if (USARTx == USART2)
			BSP_USART_FLAG = 2;
	va_end(args);
	for(x=0;x<length;x++)
	printf("%c",TXBuffer[x]);
	return length;
}
char Getchar(USART_TypeDef *USARTx)
{
	char temp;
	if(USARTx == USART1)
		{
			BSP_USART_FLAG = 1;
			temp = getchar();
		}
	else if (USARTx == USART2)
		{
			BSP_USART_FLAG = 2;
			temp = getchar();
		}
		return temp;
}

///�ض���c�⺯��printf�����ڣ��ض�����ʹ��printf����
int fputc(int ch, FILE *f)
{
		if(BSP_USART_FLAG ==1)
		{
			USART_SendData(USART1, (uint8_t) ch);
			while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);		
		}
		else if (BSP_USART_FLAG ==2)
		{
			USART_SendData(USART2, (uint8_t) ch);
			while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);		
		}
		return (ch);
}

///�ض���c�⺯��scanf�����ڣ���д����ʹ��scanf��getchar�Ⱥ���
int fgetc(FILE *f)
{
		/* �ȴ������������� */
	int temp;
	if(BSP_USART_FLAG ==1)
	{
		while (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET);
		temp = (int)USART_ReceiveData(USART1);
	}
		
	else if (BSP_USART_FLAG ==2)
	{
		while (USART_GetFlagStatus(USART2, USART_FLAG_RXNE) == RESET);
		temp = (int)USART_ReceiveData(USART2);
	}
	return temp;
}























