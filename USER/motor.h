#ifndef __MOTOR_H
#define __MOTOR_H

#include "stm32f10x.h"

// 舵机通道宏定义
#define MOTOR_LEFT_FRONT  1 // 左前
#define MOTOR_RIGHT_FRONT 2 // 右前
#define MOTOR_LEFT_REAR   3 // 左后
#define MOTOR_RIGHT_REAR  4 // 右后

// 初始化舵机控制的PWM输出
void Motor_Init(void);

// 设置舵机的角度
// 参数：channel - 舵机通道 (使用宏定义)
//       angle   - 舵机角度 (0-180)
void Motor_SetAngle(uint8_t channel, uint8_t angle);

#endif // __MOTOR_H
