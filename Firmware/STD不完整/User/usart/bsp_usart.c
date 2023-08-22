#include "bsp_usart.h"
#include "stdarg.h"
static uint8_t BSP_USART_FLAG = 1;//1表示串口1输出输入模式，2表示串口2输出输入模式，依此推类。

//加入以下代码,支持printf函数,而不需要选择use MicroLIB
#if 0
#pragma import(__use_no_semihosting)             
//标准库需要的支持函数                 
struct __FILE 
{ 
	int handle; 

}; 

FILE __stdout;       
//定义_sys_exit()以避免使用半主机模式    
_sys_exit(int x) 
{ 
	x = x; 
} 
//重定义fputc函数 
int fputc(int ch, FILE *f)
{      
	while((USART1->SR&0X40)==0);//循环发送,直到发送完毕   
    USART1->DR = (u8) ch;      
	return ch;
}
#endif 

void USART_NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  NVIC_InitStructure.NVIC_IRQChannel = DEBUG_USART1_IRQ; /* 配置USART为中断源 */
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;/* 抢断优先级*/
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;/* 子优先级 */
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; /* 使能中断 */
  NVIC_Init(&NVIC_InitStructure);/* 初始化配置NVIC */
	
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
	// 将USART Tx的GPIO配置为推挽复用模式
	GPIO_InitStructure.GPIO_Pin = DEBUG_USART1_TX_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(DEBUG_USART1_TX_GPIO_PORT, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = DEBUG_USART2_TX_GPIO_PIN;
	GPIO_Init(DEBUG_USART2_TX_GPIO_PORT, &GPIO_InitStructure);
	
  // 将USART Rx的GPIO配置为浮空输入模式
	GPIO_InitStructure.GPIO_Pin = DEBUG_USART1_RX_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(DEBUG_USART1_RX_GPIO_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = DEBUG_USART2_RX_GPIO_PIN;
	GPIO_Init(DEBUG_USART2_RX_GPIO_PORT, &GPIO_InitStructure);
	//重映射USART2引脚
	//RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	//GPIO_PinRemapConfig(GPIO_Remap_USART2,ENABLE);
	
	USART_InitStructure.USART_BaudRate = DEBUG_USART1_BAUDRATE;// 配置波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;// 配置 针数据字长
	USART_InitStructure.USART_StopBits = USART_StopBits_1;// 配置停止位
	USART_InitStructure.USART_Parity = USART_Parity_No ;// 配置校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;// 配置硬件流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;// 配置工作模式，收发一起
	
	USART_Init(USART1, &USART_InitStructure);
	USART_InitStructure.USART_BaudRate = DEBUG_USART2_BAUDRATE;// 配置波特率
	USART_Init(USART2, &USART_InitStructure);
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);	//RX有东西时进行读操作
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);	//RX有东西时进行读操作
	
	USART_Cmd(USART1, ENABLE);
	USART_Cmd(USART2, ENABLE);	
	//如下语句解决第1个字节无法正确发送出去的问题
	  USART_ClearFlag(USART1, USART_FLAG_TC);        //清串口1发送标志
		USART_ClearFlag(USART2, USART_FLAG_TC);        //清串口2发送标志
}


/* 发送一个字节 */
void Usart_SendByte(USART_TypeDef* pUSARTx, uint8_t data)
{
	USART_SendData(pUSARTx, data);
	while( USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET );//检测TX寄存器是否变空，即有无进入移位寄存器，若无返回0
}

/* 发送两个字节的数据 */
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

/* 发送8位数据的数组 */
void Usart_SendArray(USART_TypeDef* pUSARTx, uint8_t *array,uint8_t num)
{
	uint8_t i;
	for( i=0; i<num; i++ )
  {
		Usart_SendByte(pUSARTx, array[i]);
	}
	while( USART_GetFlagStatus(pUSARTx, USART_FLAG_TC) == RESET );
}

/* 发送字符串 */
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
//利用BSP_USART_FLAG巧妙解决多个串口重定向printf问题

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

///重定向c库函数printf到串口，重定向后可使用printf函数
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

///重定向c库函数scanf到串口，重写向后可使用scanf、getchar等函数
int fgetc(FILE *f)
{
		/* 等待串口输入数据 */
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























