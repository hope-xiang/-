#include "stm32f10x.h"                  // Device header

void PWM_Init(void)
{
	/*开启时钟*/
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);			
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);			
	
	/*GPIO初始化*/
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;  // 增加PA1作为电机2的PWM输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);							
	
	/*配置时钟源*/
	TIM_InternalClockConfig(TIM2);		
	
	/*时基单元初始化*/
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;				
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;     
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period = 100 - 1;                  // 保持100级调速
	TIM_TimeBaseInitStructure.TIM_Prescaler = 720 - 1;               // 72MHz/720=100kHz
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;            
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure);            
	
	// 清除标志位
	TIM_ClearFlag(TIM2, TIM_FLAG_Update);
	
	// 中断输出控制（不需要更新中断）
	
	/*输出比较初始化 - 电机1（原通道1）*/ 
	TIM_OCInitTypeDef TIM_OCInitStructure;							
	TIM_OCStructInit(&TIM_OCInitStructure);                         
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;              
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;       
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;   
	TIM_OCInitStructure.TIM_Pulse = 0;								
	TIM_OC1Init(TIM2, &TIM_OCInitStructure);                       
	
	/*输出比较初始化 - 电机2（新增通道2）*/
	TIM_OCInitStructure.TIM_Pulse = 0;
	TIM_OC2Init(TIM2, &TIM_OCInitStructure);  // 配置TIM2_CH2
	
	/*TIM使能*/
	TIM_Cmd(TIM2, ENABLE);			
}

/**
  * 函数：设置电机1的PWM占空比
  * 参数：Compare 范围0~100
  */
void PWM_SetCompare1(uint16_t Compare)
{
	TIM_SetCompare1(TIM2, Compare);		
}

/**
  * 函数：设置电机2的PWM占空比（新增）
  * 参数：Compare 范围0~100
  */
void PWM_SetCompare2(uint16_t Compare)
{
	TIM_SetCompare2(TIM2, Compare);		
}
