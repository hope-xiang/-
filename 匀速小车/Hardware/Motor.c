#include "stm32f10x.h"
#include "PWM.h"

/**
  * 函    数：直流电机初始化
  * 参    数：无
  * 返 回 值：无
  */
void Motor_Init(void)
{
    /*开启时钟*/
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

    GPIO_InitTypeDef GPIO_InitStructure;
    // 初始化PB14和PB15
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15 | GPIO_Pin_14;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    // 初始化PB12和PB13
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_12;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    PWM_Init();
}

/**
  * 函    数：直流电机设置速度
  * 参    数：Speed1 第一路电机要设置的速度，范围：-100~100
  *         Speed2 第二路电机要设置的速度，范围：-100~100
  * 返 回 值：无
  */
void Motor_SetSpeed(int16_t Speed1, int16_t Speed2)
{
    // 控制第一路电机
    if (Speed1 >= 0)
    {
        GPIO_SetBits(GPIOB, GPIO_Pin_15);
        GPIO_ResetBits(GPIOB, GPIO_Pin_14);
        PWM_SetCompare1(Speed1);
    }
    else
    {
        GPIO_ResetBits(GPIOB, GPIO_Pin_15);
        GPIO_SetBits(GPIOB, GPIO_Pin_14);
        PWM_SetCompare1(-Speed1);
    }

    // 控制第二路电机
    if (Speed2 >= 0)
    {
        GPIO_SetBits(GPIOB, GPIO_Pin_13);
        GPIO_ResetBits(GPIOB, GPIO_Pin_12);
        PWM_SetCompare2(Speed2);
    }
    else
    {
        GPIO_ResetBits(GPIOB, GPIO_Pin_13);
        GPIO_SetBits(GPIOB, GPIO_Pin_12);
        PWM_SetCompare2(-Speed2);
    }
}
