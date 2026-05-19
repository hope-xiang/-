#include "stm32f10x.h"                  // Device header

void Timer_Init(void)
{
	//开启时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);   //PWM用的TIM2这里避免重复用3
	//配置时钟源
	TIM_InternalClockConfig(TIM3);
	//配置时基单元
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseInitStruct;
	TIM_TimeBaseInitStruct.TIM_ClockDivision=TIM_CKD_DIV1;    
	TIM_TimeBaseInitStruct.TIM_CounterMode=TIM_CounterMode_Up;   //计数模式——向上计数
	TIM_TimeBaseInitStruct.TIM_Period=1000-1;       //Period和Prescaler都<=65535
	TIM_TimeBaseInitStruct.TIM_Prescaler=7200-1;     //频率=72MHz/((Period+1)*(Prescaler+1))=10Hz--每0.1秒读取一次
	TIM_TimeBaseInitStruct.TIM_RepetitionCounter=0;    //重复计数器，高级寄存器才有
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStruct);
	
	//清除标志位，如果没有此步骤，开启中断就会先进行一次中断
	TIM_ClearFlag(TIM3,TIM_FLAG_Update);
	
	//中断输出控制
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);

	//配置NVIC
	NVIC_PriorityGroupConfig( NVIC_PriorityGroup_2);
	NVIC_InitTypeDef Structure;
	Structure.NVIC_IRQChannel=TIM3_IRQn;
	Structure.NVIC_IRQChannelPreemptionPriority=2;
	Structure.NVIC_IRQChannelSubPriority=1;
	Structure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&Structure);
	
	//定时器使能
	TIM_Cmd(TIM3,ENABLE);
}	

