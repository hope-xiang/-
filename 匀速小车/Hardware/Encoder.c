#include "stm32f10x.h"
#include "encoder.h"

volatile int16_t Encoder_Count1 = 0;
volatile int16_t Encoder_Count2 = 0;

void Encoder_Init(void)
{
    /*开启时钟*/
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);		
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);		
    
    /*GPIO初始化*/
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_4 | GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);						
    
    /*AFIO选择中断引脚*/
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource6);
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource7);
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource4);
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource5);
    
    /*EXTI初始化*/
    EXTI_InitTypeDef EXTI_InitStructure;						
    EXTI_InitStructure.EXTI_Line = EXTI_Line6 | EXTI_Line7 | EXTI_Line4 | EXTI_Line5;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;					
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;			
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
    EXTI_Init(&EXTI_InitStructure);								
    
    /*NVIC中断分组*/
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);				
    
    /*NVIC配置*/
    NVIC_InitTypeDef NVIC_InitStructure;						
    NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;	
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;			
    NVIC_Init(&NVIC_InitStructure);							
}

// 统一的中断处理函数
void EXTI9_5_IRQHandler(void)
{
    // 处理编码器1（PB6和PB7）
    if (EXTI_GetITStatus(EXTI_Line6) == SET)
    {
        // 检查这是编码器1的A相（PB6）
        if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_6) == 0) 
        {
            // 读取编码器1的B相（PB7）状态来判断方向
            if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_7) == 0)
            {
                Encoder_Count1--;	// 编码器1反转
            }
            else
            {
                Encoder_Count1++;	// 编码器1正转
            }
        }
        EXTI_ClearITPendingBit(EXTI_Line6);
    }

    // 处理编码器1（PB7中断）
    if (EXTI_GetITStatus(EXTI_Line7) == SET)
    {
        // 检查这是编码器1的B相（PB7）
        if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_7) == 0)
        {
            // 读取编码器1的A相（PB6）状态来判断方向
            if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_6) == 0)
            {
                Encoder_Count1++;	// 编码器1正转
            }
            else
            {
                Encoder_Count1--;	// 编码器1反转
            }
        }
        EXTI_ClearITPendingBit(EXTI_Line7);
    }

    // 处理编码器2（PB4和PB5）
    if (EXTI_GetITStatus(EXTI_Line4) == SET)
    {
        // 检查这是编码器2的A相（PB4）
        if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_4) == 0) 
        {
            // 读取编码器2的B相（PB5）状态来判断方向
            if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_5) == 0)
            {
                Encoder_Count2--;	// 编码器2反转
            }
            else
            {
                Encoder_Count2++;	// 编码器2正转
            }
        }
        EXTI_ClearITPendingBit(EXTI_Line4);
    }

    // 处理编码器2（PB5中断）
    if (EXTI_GetITStatus(EXTI_Line5) == SET)
    {
        // 检查这是编码器2的B相（PB5）
        if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_5) == 0)
        {
            // 读取编码器2的A相（PB4）状态来判断方向
            if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_4) == 0)
            {
                Encoder_Count2++;	// 编码器2正转
            }
            else
            {
                Encoder_Count2--;	// 编码器2反转
            }
        }
        EXTI_ClearITPendingBit(EXTI_Line5);
    }
}
