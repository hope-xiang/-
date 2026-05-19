#include "stm32f10x.h"                  // Device header
#include "PWM.h"

/**
  * 函数：双电机初始化
  * 说明：初始化电机1（B14/B15）和电机2（B12/B13）的控制引脚
  */
void Motor_Init(void)
{
	/*开启GPIOB时钟*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);		
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	// 电机1：B14/B15；电机2：B12/B13（新增）
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15 | GPIO_Pin_14 | GPIO_Pin_13 | GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);						
	
	PWM_Init();	// 初始化PWM（包含两个通道）
}

/**
  * 函数：设置电机1速度
  * 参数：Speed 范围-100~100（正负表示方向）
  */
void Motor1_SetSpeed(int16_t Speed)
{
	if (Speed >= 0)							
	{
		GPIO_SetBits(GPIOB, GPIO_Pin_15);	// 正转方向
		GPIO_ResetBits(GPIOB, GPIO_Pin_14);
		PWM_SetCompare1(Speed);				
	}
	else									
	{
		GPIO_ResetBits(GPIOB, GPIO_Pin_15);	// 反转方向
		GPIO_SetBits(GPIOB, GPIO_Pin_14);
		PWM_SetCompare1(-Speed);			
	}
}

/**
  * 函数：设置电机2速度（新增）
  * 参数：Speed 范围-100~100（正负表示方向）
  */
void Motor2_SetSpeed(int16_t Speed)
{
	if (Speed >= 0)							
	{
		GPIO_SetBits(GPIOB, GPIO_Pin_13);	// 正转方向
		GPIO_ResetBits(GPIOB, GPIO_Pin_12);
		PWM_SetCompare2(Speed);				
	}
	else									
	{
		GPIO_ResetBits(GPIOB, GPIO_Pin_13);	// 反转方向
		GPIO_SetBits(GPIOB, GPIO_Pin_12);
		PWM_SetCompare2(-Speed);			
	}
}
