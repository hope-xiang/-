#include "stm32f10x.h"

void Timer_Init(void)
{
    // 1. 开启 TIM1 的时钟
    // TIM1 在 APB2 总线上
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);

    // 2. 配置时钟源 (使用内部时钟)
    TIM_InternalClockConfig(TIM1);

    // 3. 配置时基单元
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
    TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;     // 不分频
    TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up; // 向上计数模式
    TIM_TimeBaseInitStruct.TIM_Period = 1000 - 1;                // 自动重载值 (ARR)
    TIM_TimeBaseInitStruct.TIM_Prescaler = 7200 - 1;             // 预分频器 (PSC)
    TIM_TimeBaseInitStruct.TIM_RepetitionCounter = 0;            // 重复计数器，高级定时器特有，这里不用
    TIM_TimeBaseInit(TIM1, &TIM_TimeBaseInitStruct);

    // 4. 清除更新标志位
    TIM_ClearFlag(TIM1, TIM_FLAG_Update);

    // 5. 开启定时器更新中断
    TIM_ITConfig(TIM1, TIM_IT_Update, ENABLE);

    // 6. 配置 NVIC (嵌套向量中断控制器)
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    NVIC_InitTypeDef NVIC_InitStruct;
    NVIC_InitStruct.NVIC_IRQChannel = TIM1_UP_IRQn;              // TIM1 向上计数溢出中断通道
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStruct);

    // 7. 使能定时器
    // 注意：对于高级定时器，需要调用 TIM_Cmd 和 TIM_CtrlPWMOutputs
    // 即使不使用PWM，只使用中断，也需要使能主输出，否则计数器可能不工作
    TIM_CtrlPWMOutputs(TIM1, ENABLE);
    TIM_Cmd(TIM1, ENABLE);
}
