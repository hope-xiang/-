#include "stm32f10x.h"                  // Device header

/**
  * 函    数：电机1编码器初始化（PB6/PB7）
  * 参    数：无
  * 返 回 值：无
  */
void Encoder1_Init(void)
{
	/*开启时钟*/
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);			
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);			
	
	/*GPIO初始化*/
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;  // 电机1编码器引脚
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);							
	
	/*时基单元初始化（与电机2参数一致）*/
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;				
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;    
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up; 
	TIM_TimeBaseInitStructure.TIM_Period = 65535 - 1;                // 最大计数范围
	TIM_TimeBaseInitStructure.TIM_Prescaler = 1 - 1;                // 不分频
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;            
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseInitStructure);             
	
	/*输入捕获初始化（滤波参数相同）*/
	TIM_ICInitTypeDef TIM_ICInitStructure;							
	TIM_ICStructInit(&TIM_ICInitStructure);							
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;				
	TIM_ICInitStructure.TIM_ICFilter = 0xF;							// 相同滤波系数
	TIM_ICInit(TIM4, &TIM_ICInitStructure);							
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;				
	TIM_ICInitStructure.TIM_ICFilter = 0xF;							
	TIM_ICInit(TIM4, &TIM_ICInitStructure);							
	
	/*编码器接口配置（模式与极性相同）*/
	TIM_EncoderInterfaceConfig(TIM4, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);																	
	
	/*TIM使能*/
	TIM_Cmd(TIM4, ENABLE);			
}

/**
  * 函    数：电机2编码器初始化（PB4/PB5）
  * 参    数：无
  * 返 回 值：无
  * 说    明：与电机1配置完全一致（同型号电机）
  */
void Encoder2_Init(void)
{
	/*开启时钟*/
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);

	/*关闭JTAG，释放PB4/PB5，并将TIM3部分重映射到PB4/PB5*/
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
	GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3, ENABLE);			
	
	/*GPIO初始化（电机2编码器引                                                                                                                                                                                                脚PB4/PB5）*/
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);							
	
	/*时基单元初始化（参数与电机1完全相同）*/
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;				
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;    
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up; 
	TIM_TimeBaseInitStructure.TIM_Period = 65535 - 1;               
	TIM_TimeBaseInitStructure.TIM_Prescaler = 1 - 1;               
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;            
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStructure);             
	
	/*输入捕获初始化（滤波参数相同）*/
	TIM_ICInitTypeDef TIM_ICInitStructure;							
	TIM_ICStructInit(&TIM_ICInitStructure);							
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;  // TIM3_CH1对应PB4
	TIM_ICInitStructure.TIM_ICFilter = 0xF;							
	TIM_ICInit(TIM3, &TIM_ICInitStructure);							
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;  // TIM3_CH2对应PB5
	TIM_ICInitStructure.TIM_ICFilter = 0xF;							
	TIM_ICInit(TIM3, &TIM_ICInitStructure);							
	
	/*编码器接口配置（模式与极性完全相同）*/
	TIM_EncoderInterfaceConfig(TIM3, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);																	
	
	/*TIM使能*/
	TIM_Cmd(TIM3, ENABLE);			
}

/**
  * 函    数：获取电机1编码器计数
  * 参    数：无
  * 返 回 值：当前计数（16位有符号数）
  */
int16_t Encoder1_GetCount(void)
{
	int16_t count = TIM_GetCounter(TIM4);
	TIM_SetCounter(TIM4, 0);  // 读取后清零，方便下次计数
	return count;
}

/**
  * 函    数：获取电机2编码器计数
  * 参    数：无
  * 返 回 值：当前计数（16位有符号数）
  */
int16_t Encoder2_GetCount(void)
{
	int16_t count = TIM_GetCounter(TIM3);
	TIM_SetCounter(TIM3, 0);  // 读取后清零，方便下次计数
	return count;
}
