#include "stm32f10x.h"
#include "Delay.h"
#include "OLED.h"
#include "Motor.h"
#include "Key.h"
#include "Encoder.h"
#include "Timer.h"
#include "PID.h"  

uint8_t KeyNum;
// 目标速度和当前速度变量
int16_t TargetSpeed1 = 0, TargetSpeed2 = 0;
int16_t CurrentSpeed1 = 0, CurrentSpeed2 = 0;
// 电机减速比与编码器脉冲数的综合参数
const uint16_t C1 = 1040;

int main(void)
{
    // 外设初始化
    OLED_Init();
    Motor_Init();         // 初始化电机（TIM2 PWM）
    Key_Init();           // 按键初始化
    Encoder1_Init();      // 编码器1（TIM4）
    Encoder2_Init();      // 编码器2（TIM3）
    Timer_Init();         // 定时器（TIM1，100ms中断）

    // PID初始化（参数需根据实际调试调整）
    // 输出限幅设为 100，因为 Motor_SetSpeed 的范围是 -100 ~ 100
    PID_Init(&PID_Instance1, 0.30f, 0.01f, 0.0f, 500, 100);
    PID_Init(&PID_Instance2, 1.0f, 0.01f, 0.5f, 500, 100);

    // OLED显示初始化
    OLED_ShowString(1, 1, "T1:");    // 目标速度1
    OLED_ShowString(2, 1, "C1:");    // 当前速度1
    OLED_ShowString(3, 1, "T2:");    // 目标速度2
    OLED_ShowString(4, 1, "C2:");    // 当前速度2

    while (1)
    {
        // 按键处理（设置目标速度）
        KeyNum = Key_GetNum();
        if (KeyNum == 1)  // 按键1：调整电机1目标速度
        {
            TargetSpeed1 += 88;
            // 上限为最大转速 880
            if (TargetSpeed1 > 880)
            {
                TargetSpeed1 = 0;
            }
            // 更新PID的目标值
            PID_SetTarget(&PID_Instance1, TargetSpeed1);
        }
        if (KeyNum == 2)  // 按键2：调整电机2目标速度
        {
            TargetSpeed2 += 88;
            if (TargetSpeed2 > 880)
            {
                TargetSpeed2 = 0;
            }
            PID_SetTarget(&PID_Instance2, TargetSpeed2);
        }

        // OLED显示更新
        OLED_ShowSignedNum(1, 4, TargetSpeed1, 3);
        OLED_ShowSignedNum(2, 4, CurrentSpeed1, 4);
        OLED_ShowSignedNum(3, 4, TargetSpeed2, 3);
        OLED_ShowSignedNum(4, 4, CurrentSpeed2, 4);

        Delay_ms(50);  // 主循环延时，用于按键消抖
    }
}

// TIM1定时中断服务函数（100ms执行一次）
void TIM1_UP_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM1, TIM_IT_Update) == SET)
    {
        // 1. 计算当前速度（单位：r/min）
        // Encoder1_GetCount() 内部已清零计数器
        CurrentSpeed1 = (int16_t)(Encoder1_GetCount() * 60.0f / (C1 * 0.1f));
        CurrentSpeed2 = (int16_t)(Encoder2_GetCount() * 60.0f / (C1 * 0.1f));

        // 2. 执行PID控制
        // 该函数会根据 CurrentSpeed 和 TargetSpeed 的偏差来计算并输出PWM
        PID_ControlLoop();

        // 3. 清除中断标志位
        TIM_ClearITPendingBit(TIM1, TIM_IT_Update);
    }
}
