#ifndef __SERVO_CONTROL_H
#define __SERVO_CONTROL_H

#include "stm32f10x.h"

// 回调函数类型定义
typedef void (*ActionCallback)(void);

// 回调函数结构体
typedef struct {
    ActionCallback RelaxedGetDown;
    ActionCallback Upright;
    ActionCallback GetDown;
    ActionCallback Sit;
    ActionCallback SetInitialPosition;
} ActionCallbacks;

// 全局回调函数结构体实例
extern ActionCallbacks actionCallbacks;

// 注册回调函数
void Action_RegisterCallbacks(void);

// 动作函数声明
void Action_relaxed_getdowm(void);
void Action_upright(void);
void Action_getdowm(void);
void Action_sit(void);
void Servo_SetInitialPosition(void);

#endif // __SERVO_CONTROL_H
