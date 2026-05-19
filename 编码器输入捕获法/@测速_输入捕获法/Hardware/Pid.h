#ifndef __PID_H__
#define __PID_H__

#include "stm32f10x.h"

// PID参数结构体
typedef struct {
    float Kp;               // 比例系数
    float Ki;               // 积分系数
    float Kd;               // 微分系数
    int16_t Target;         // 目标值
    int16_t LastError;      // 上一次误差
    int16_t Integral;       // 积分值
    int16_t IntegralLimit;  // 积分限幅
    int16_t OutputLimit;    // 输出限幅
} PID_HandleTypeDef;

// 外部声明两路PID结构体
extern PID_HandleTypeDef PID_Instance1;
extern PID_HandleTypeDef PID_Instance2;

/**
 * @brief 初始化PID参数
 * @param pid: PID结构体指针
 * @param kp: 比例系数
 * @param ki: 积分系数
 * @param kd: 微分系数
 * @param integralLimit: 积分限幅
 * @param outputLimit: 输出限幅
 */
void PID_Init(PID_HandleTypeDef *pid, float kp, float ki, float kd, 
             int16_t integralLimit, int16_t outputLimit);

/**
 * @brief 设置PID目标值
 * @param pid: PID结构体指针
 * @param target: 目标值
 */
void PID_SetTarget(PID_HandleTypeDef *pid, int16_t target);

/**
 * @brief PID计算函数
 * @param pid: PID结构体指针
 * @param current: 当前测量值
 * @return 计算后的输出值
 */
int16_t PID_Calculate(PID_HandleTypeDef *pid, int16_t current);

/**
 * @brief 双路PID控制周期执行函数
 * @note 建议在定时器中断中调用，与编码器测速周期同步
 */
void PID_ControlLoop(void);

#endif
