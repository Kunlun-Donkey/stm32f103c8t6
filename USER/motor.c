/*
 * @Author: Kunlun-Donkey 1298394344@qq.com
 * @Date: 2025-03-24 13:15:07
 * @LastEditors: Kunlun-Donkey 1298394344@qq.com
 * @LastEditTime: 2025-03-26 11:21:50
 * @FilePath: \stm32f103c8t6\USER\motor.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
/******************************************************************************
 * @file    motor.c
 * @author  Kunlun-Donkey 1298394344@qq.com
 * @date    2025-03-24
 * @brief   舵机控制代码
 * @details 提供舵机的初始化和控制函数。
 ******************************************************************************/

#include "stm32f10x.h"
#include "stm32f10x_tim.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "motor.h"

// 定义当前舵机角度数组
static uint8_t current_angles[4] = {90, 90, 90, 90}; // 初始角度为 90 度

// 定义死区范围
#define ANGLE_DEADZONE 5 // 死区范围为 ±2 度

//=============================================================================
// 函数名称：Motor_Init
// 功能概要：初始化舵机控制的PWM输出
// 参数说明：无
// 返回值：无
//=============================================================================
void Motor_Init(void)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_OCInitTypeDef TIM_OCInitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;

    // 开启TIM2和GPIOA时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    // 配置GPIOA的PA0, PA1, PA2, PA3为复用推挽输出
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // 配置TIM2的时间基准
    TIM_TimeBaseStructure.TIM_Period = 20000 - 1; // 20ms周期 (50Hz)
    TIM_TimeBaseStructure.TIM_Prescaler = 72 - 1; // 72MHz / 72 = 1MHz
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

    // 配置PWM模式
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse = 1500; // 默认1.5ms脉宽 (舵机中位)
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;

    // 配置TIM2的4个通道
    TIM_OC1Init(TIM2, &TIM_OCInitStructure);
    TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Enable);

    TIM_OC2Init(TIM2, &TIM_OCInitStructure);
    TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Enable);

    TIM_OC3Init(TIM2, &TIM_OCInitStructure);
    TIM_OC3PreloadConfig(TIM2, TIM_OCPreload_Enable);

    TIM_OC4Init(TIM2, &TIM_OCInitStructure);
    TIM_OC4PreloadConfig(TIM2, TIM_OCPreload_Enable);

    TIM_ARRPreloadConfig(TIM2, ENABLE);

    // 启动TIM2
    TIM_Cmd(TIM2, ENABLE);
}

//=============================================================================
// 函数名称：Motor_SetAngle
// 功能概要：设置舵机的角度
// 参数说明：channel - 舵机通道 (1-4)
//           angle   - 舵机角度 (0-180)
// 返回值：无
//=============================================================================
void Motor_SetAngle(uint8_t channel, uint8_t angle)
{
    if (channel >= 4) {
        return; // 无效的舵机 ID
    }

    // 检查目标角度是否在死区范围内
    if (abs(current_angles[channel] - angle) <= ANGLE_DEADZONE) {
        return; // 如果在死区范围内，不更新角度
    }

    // 更新当前角度
    current_angles[channel] = angle;

    uint16_t pulse = 1000 + (angle * 1000 / 180); // 将角度转换为脉宽 (1ms到2ms)
    switch (channel)
    {
        case 1:
            TIM_SetCompare1(TIM2, pulse);
            break;
        case 2:
            TIM_SetCompare2(TIM2, pulse);
            break;
        case 3:
            TIM_SetCompare3(TIM2, pulse);
            break;
        case 4:
            TIM_SetCompare4(TIM2, pulse);
            break;
        default:
            // 无效通道
            break;
    }
}
