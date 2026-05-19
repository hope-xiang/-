#include "stm32f10x.h"                  // Device header
#include"stdio.h"
#include"stdarg.h"

extern uint8_t RxData;

uint16_t pow(uint8_t a,uint8_t x)
{
	uint16_t sum=1;
	for(uint8_t i=0;i<x;i++)
	{	
		sum*=a;
	}
	return sum;
}
void Serial_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	GPIO_InitTypeDef rap;
	rap.GPIO_Mode=GPIO_Mode_AF_PP;
	rap.GPIO_Pin=GPIO_Pin_9;
	rap.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&rap);
	
	rap.GPIO_Mode=GPIO_Mode_IPU;
	rap.GPIO_Pin=GPIO_Pin_10;
	rap.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&rap);
	
	USART_InitTypeDef Rap;
	Rap.USART_BaudRate=9600;    //配置波特率
	Rap.USART_HardwareFlowControl=USART_HardwareFlowControl_None;   //不使用流控
	Rap.USART_Mode=USART_Mode_Tx |USART_Mode_Rx;  //发送功能 + 接收功能
	Rap.USART_Parity=USART_Parity_No;  //不需要校验位
	Rap.USART_StopBits=USART_StopBits_1;   //一位停止位
	Rap.USART_WordLength=USART_WordLength_8b;  //八位字长
	USART_Init(USART1,&Rap);
	
	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NVIC_InitTypeDef RAP;
	RAP.NVIC_IRQChannel=USART1_IRQn;
	RAP.NVIC_IRQChannelPreemptionPriority=1;
	RAP.NVIC_IRQChannelSubPriority=1;
	RAP. NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&RAP);
	
	USART_Cmd(USART1,ENABLE);
}




void  USART1_IRQHandler(void) //中断进行多次，由下面程序可知数据是一次一次放进去的
{
	
	if(USART_GetFlagStatus(USART1,USART_FLAG_RXNE)==SET)
	{	
		RxData=USART_ReceiveData(USART1);
		
		USART_ClearITPendingBit(USART1,USART_FLAG_RXNE);
	}
}
void Serial_SendByte(uint8_t Byte)
{
	USART_SendData(USART1,Byte);
	while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==RESET);  //要检验TDR数据是否进入移位寄存器
}
void Serial_SendArray(uint8_t *a,uint16_t Length)
{
	uint16_t i;
	for(i=0;i<Length;i++)
	{
		Serial_SendByte(a[i]);
	}
}
void Serial_SendString(char *String) //uint8_t==char
{
	
	
	uint8_t i=0;
	for(i=0;String[i]!=0;i++)
	{
		Serial_SendByte(String[i]);
	}
}
void Serial_SendNumber(uint16_t number,uint8_t Length)
{
	uint8_t i;
	for(i=Length;i;i--)
	{
		Serial_SendByte(number/pow(10,i-1)+0x30);
		number-=number/pow(10,i-1)*pow(10,i-1);
	}
}	
int fputc(int ch,FILE*f)
{
	Serial_SendByte(ch);
	return ch;
}

void Serial_Printf(char* format,...)
{
	char String[100];
	va_list arg;
	va_start(arg,format);
	vsprintf(String,format,arg);
	va_end(arg);
	Serial_SendString(String);
}


