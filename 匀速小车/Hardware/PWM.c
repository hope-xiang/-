#include "stm32f10x.h"                  // Device header

/**
  * 函    数：PWM初始化
  * 参    数：无
  * 返 回 值：无
  */
void PWM_Init(void)
{
    /*开启时钟*/
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    /*GPIO初始化*/
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    /*配置时钟源*/
    TIM_InternalClockConfig(TIM2);

    /*时基单元初始化*/
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInitStructure.TIM_Period = 100 - 1; // 根据需求调整
    TIM_TimeBaseInitStructure.TIM_Prescaler = 720 - 1; // 根据需求调整
    TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure);

    //清除标志位
    TIM_ClearFlag(TIM2, TIM_FLAG_Update);

    //中断输出控制
    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);

    /*输出比较初始化 - 通道1 */
    TIM_OCInitTypeDef TIM_OCInitStructure1;
    TIM_OCStructInit(&TIM_OCInitStructure1);
    TIM_OCInitStructure1.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure1.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OCInitStructure1.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure1.TIM_Pulse = 0;
    TIM_OC1Init(TIM2, &TIM_OCInitStructure1);

    /*输出比较初始化 - 通道2 */
    TIM_OCInitTypeDef TIM_OCInitStructure2;
    TIM_OCStructInit(&TIM_OCInitStructure2);
    TIM_OCInitStructure2.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure2.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OCInitStructure2.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure2.TIM_Pulse = 0;
    TIM_OC2Init(TIM2, &TIM_OCInitStructure2);

    /*TIM使能*/
    TIM_Cmd(TIM2, ENABLE);
}

/**
  * 函    数：PWM设置CCR1
  * 参    数：Compare 要写入的CCR1的值，范围：0~100
  * 返 回 值：无
  * 注意事项：CCR1和ARR共同决定占空比，此函数仅设置CCR1的值，并不直接是占空比
  *           占空比Duty = CCR1 / (ARR + 1)
  */
void PWM_SetCompare1(uint16_t Compare)
{
    TIM_SetCompare1(TIM2, Compare);
}

/**
  * 函    数：PWM设置CCR2
  * 参    数：Compare 要写入的CCR2的值，范围：0~100
  * 返 回 值：无
  * 注意事项：CCR2和ARR共同决定占空比，此函数仅设置CCR2的值，并不直接是占空比
  *           占空比Duty = CCR2 / (ARR + 1)
  */
void PWM_SetCompare2(uint16_t Compare)
{
    TIM_SetCompare2(TIM2, Compare);
}
