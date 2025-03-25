/*
 * @Author: Kunlun-Donkey 1298394344@qq.com
 * @Date: 2025-03-25 10:58:21
 * @LastEditors: Kunlun-Donkey 1298394344@qq.com
 * @LastEditTime: 2025-03-25 13:44:38
 * @FilePath: \stm32f103c8t6\USER\servo_control.c
 * @Description: 添加动作行为
 */
#include "servo_control.h"
#include "motor.h"
#include "delay.h"

// 定义回调函数结构体
ActionCallbacks actionCallbacks;

// 注册回调函数
void Action_RegisterCallbacks(void) {
    actionCallbacks.SetInitialPosition = Servo_SetInitialPosition;
    actionCallbacks.RelaxedGetDown = Action_relaxed_getdowm;
    actionCallbacks.Upright = Action_upright;
    actionCallbacks.GetDown = Action_getdowm;
    actionCallbacks.Sit = Action_sit;

}

// 设置所有舵机到初始位置
void Servo_SetInitialPosition(void) {
    Motor_SetAngle(MOTOR_LEFT_FRONT, 90);
    Motor_SetAngle(MOTOR_RIGHT_FRONT, 90);
    Motor_SetAngle(MOTOR_LEFT_REAR, 90);
    Motor_SetAngle(MOTOR_RIGHT_REAR, 90);
}

// 动作实现
void Action_relaxed_getdowm(void) {
    Motor_SetAngle(MOTOR_LEFT_FRONT, 20);
    Motor_SetAngle(MOTOR_RIGHT_FRONT, 20);
    Delay_ms(80);
    Motor_SetAngle(MOTOR_LEFT_REAR, 160);
    Motor_SetAngle(MOTOR_RIGHT_REAR, 160);
}

void Action_upright(void) { // 站立
    Motor_SetAngle(MOTOR_LEFT_FRONT, 90);
    Motor_SetAngle(MOTOR_RIGHT_FRONT, 90);
    Delay_ms(80);
    Motor_SetAngle(MOTOR_LEFT_REAR, 90);
    Motor_SetAngle(MOTOR_RIGHT_REAR, 90);
}

void Action_getdowm(void) { // 趴下
    Motor_SetAngle(MOTOR_LEFT_FRONT, 20);
    Motor_SetAngle(MOTOR_RIGHT_FRONT, 20);
    Delay_ms(80);
    Motor_SetAngle(MOTOR_LEFT_REAR, 20);
    Motor_SetAngle(MOTOR_RIGHT_REAR, 20);
}

void Action_sit(void) { // 坐下
    Motor_SetAngle(MOTOR_LEFT_FRONT, 90);
    Motor_SetAngle(MOTOR_RIGHT_FRONT, 90);
    Delay_ms(80);
    Motor_SetAngle(MOTOR_LEFT_REAR, 20);
    Motor_SetAngle(MOTOR_RIGHT_REAR, 20);
}

