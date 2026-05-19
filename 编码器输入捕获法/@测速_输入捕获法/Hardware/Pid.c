#include "PID.h"
#include "Motor.h"
#include "Encoder.h"

// PID实例
PID_HandleTypeDef PID_Instance1 = {0};
PID_HandleTypeDef PID_Instance2 = {0};

// 外部声明当前速度变量，用于PID计算
extern int16_t CurrentSpeed1;
extern int16_t CurrentSpeed2;

/**
  * @brief  PID参数初始化
  * @param  pid: PID结构体指针
  * @param  kp: 比例系数
  * @param  ki: 积分系数
  * @param  kd: 微分系数
  * @param  integralLimit: 积分限幅
  * @param  outputLimit: 输出限幅
  */
void PID_Init(PID_HandleTypeDef *pid, float kp, float ki, float kd, 
             int16_t integralLimit, int16_t outputLimit) {
    pid->Kp = kp;
    pid->Ki = ki;
    pid->Kd = kd;
    pid->Target = 0;
    pid->LastError = 0;
    pid->Integral = 0;
    pid->IntegralLimit = integralLimit;
    pid->OutputLimit = outputLimit;
}

/**
  * @brief  设置PID目标值
  * @param  pid: PID结构体指针
  * @param  target: 目标值
  */
void PID_SetTarget(PID_HandleTypeDef *pid, int16_t target) {
    pid->Target = target;
}

/**
  * @brief  PID计算函数
  * @param  pid: PID结构体指针
  * @param  current: 当前测量值
  * @return 计算后的输出值
  */
int16_t PID_Calculate(PID_HandleTypeDef *pid, int16_t current) {
    // 计算偏差
    int16_t error = pid->Target - current;

    // 梯形积分
    pid->Integral += (error + pid->LastError) / 2;
    // 积分限幅
    if (pid->Integral > pid->IntegralLimit) {
        pid->Integral = pid->IntegralLimit;
    } else if (pid->Integral < -pid->IntegralLimit) {
        pid->Integral = -pid->IntegralLimit;
    }

    // 微分项
    int16_t differential = error - pid->LastError;
    // 更新上一次误差
    pid->LastError = error;

    // PID总输出
    int32_t output = (int32_t)(pid->Kp * error + pid->Ki * pid->Integral + pid->Kd * differential);

    // 输出限幅
    if (output > pid->OutputLimit) {
        output = pid->OutputLimit;
    } else if (output < -pid->OutputLimit) {
        output = -pid->OutputLimit;
    }

    return (int16_t)output;
}

/**
  * @brief  双路PID控制周期执行函数
  * @note   建议在定时器中断中调用，与编码器测速周期同步
  */
void PID_ControlLoop(void) {
    // 计算PID输出
    int16_t pwm1 = PID_Calculate(&PID_Instance1, CurrentSpeed1);
    int16_t pwm2 = PID_Calculate(&PID_Instance2, CurrentSpeed2);

    // 根据PID输出调整电机PWM占空比
    // 如果 CurrentSpeed < TargetSpeed，error为正，output为正，PWM占空比增大
    // 如果 CurrentSpeed > TargetSpeed，error为负，output为负，PWM占空比减小
    Motor1_SetSpeed(pwm1);
    Motor2_SetSpeed(pwm2);
}
