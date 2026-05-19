#include "stm32f10x.h"
#include "Delay.h"
#include "OLED.h"
#include "Motor.h"
#include "Key.h"
#include "Encoder.h"
#include "Timer.h"
#include <stdio.h>


uint8_t KeyNum = 0;
int16_t Gear;
int16_t Speed1, Speed2; // 用于存储两路电机速度
const uint16_t C1 = 260;
const uint16_t C2 = 9900;
const uint16_t C3 = 100;
uint8_t RxData;

int main(void)
{
    /*模块初始化*/
    Motor_Init();
    Key_Init();
    Encoder_Init();
    Timer_Init();
    OLED_Init();
    OLED_ShowString(1, 1, "Gear:");
    OLED_ShowString(2, 1, "Speed1:");
    OLED_ShowString(3, 1, "Speed2:");

    while (1)
    {
        KeyNum = Key_GetNum();
        if (KeyNum == 1)
        {
            Gear += 10;
            if (Gear > 100)
            {
                Gear = 0;
            }
        }
        if (KeyNum == 2)
        {
            Gear -= 10;
            if (Gear < -100)
            {
                Gear = 0;
            }
        }
        // 设置两路电机速度
        Motor_SetSpeed(Gear, Gear);

        // 显示档位和两路电机速度
        OLED_ShowSignedNum(1, 6, Gear, 4);

        OLED_ShowSignedNum(2, 8, Speed1, 3);
        OLED_ShowSignedNum(3, 8, Speed2, 3);

        Delay_ms(100); // 适当延迟，避免刷新过于频繁
    }
}

void TIM3_IRQHandler(void) // 每0.1秒调用一次
{
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
	 {
    Speed1 = ((Encoder_Count1 / (C1 * 0.1) * 60) * (0.048 * 3.1415926));
    Speed2 = ((Encoder_Count2 / (C1 * 0.1) * 60) * (0.048 * 3.1415926));
    Encoder_Count1 = 0;
    Encoder_Count2 = 0;
    TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
	 }
}
