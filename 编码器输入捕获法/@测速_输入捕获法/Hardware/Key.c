#include "stm32f10x.h"                  // Device header
#include "Delay.h"

/**
  * 函    数：按键初始化
  * 参    数：无
  * 返 回 值：无
  */
void Key_Init(void)
{
	/*开启时钟*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);		
	
	/*GPIO初始化*/
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);						
}

/**
  * 函    数：按键获取键码
  * 参    数：无
  * 返 回 值：按下按键的键码值，范围：0~2，返回0代表没有按键按下
  * 注意事项：此函数是阻塞式操作，当按键按住不放时，函数会卡住，直到按键松手
  */
uint8_t Key_GetNum(void)
{
	uint8_t KeyNum = 0;		
	
	if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_0) == 0)			
	{
		Delay_ms(20);											
		while (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_0) == 0);	
		Delay_ms(20);
		KeyNum = 1;											
	}
	
	if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_10) == 0)			
	{
		Delay_ms(20);											
		while (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_10) == 0);	
		Delay_ms(20);											
		KeyNum = 2;												
	}
	
	return KeyNum;			
}
