/******************************************************************************
 * @file    main.c
 * @author  Kunlun-Donkey 1298394344@qq.com
 * @date    2025-03-24
 * @brief   STM32F103C8 主程序文件
 * @details 实现舵机控制的PWM输出、LED闪烁测试以及模拟I2C通信的初始化和主循环逻辑。
 ******************************************************************************/

#include "stm32f10x.h"
#include "stm32f10x_tim.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"

// 函数声明
void TIM2_PWM_Init(void);
void GPIO_Configuration(void);
void Delay(uint32_t nCount);

//=============================================================================
// 函数名称：Delay
// 功能概要：延时函数
// 参数说明：nCount - 延时时间
// 返回值：无
//=============================================================================
void Delay(uint32_t nCount)
{
  for(; nCount != 0; nCount--);
}

//=============================================================================
// 函数名称：TIM2_PWM_Init
// 功能概要：TIM2 PWM 初始化
// 参数说明：无
// 返回值：无
//=============================================================================
void TIM2_PWM_Init(void)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_OCInitTypeDef TIM_OCInitStructure;
    
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
    
    // Time base configuration
    TIM_TimeBaseStructure.TIM_Period = 20000 - 1; // 20ms period (50Hz)
    TIM_TimeBaseStructure.TIM_Prescaler = 72 - 1; // 72MHz / 72 = 1MHz
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
    
    // PWM1 Mode configuration: Channel1
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse = 1500; // 1.5ms pulse width (neutral position for servo)
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OC1Init(TIM2, &TIM_OCInitStructure);
    TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Enable);
    
    // PWM1 Mode configuration: Channel2
    TIM_OCInitStructure.TIM_Pulse = 1500; // 1.5ms pulse width
    TIM_OC2Init(TIM2, &TIM_OCInitStructure);
    TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Enable);
    
    // PWM1 Mode configuration: Channel3
    TIM_OCInitStructure.TIM_Pulse = 1500; // 1.5ms pulse width
    TIM_OC3Init(TIM2, &TIM_OCInitStructure);
    TIM_OC3PreloadConfig(TIM2, TIM_OCPreload_Enable);
    
    // PWM1 Mode configuration: Channel4
    TIM_OCInitStructure.TIM_Pulse = 1500; // 1.5ms pulse width
    TIM_OC4Init(TIM2, &TIM_OCInitStructure);
    TIM_OC4PreloadConfig(TIM2, TIM_OCPreload_Enable);
    
    TIM_ARRPreloadConfig(TIM2, ENABLE);
    
    // TIM2 enable counter
    TIM_Cmd(TIM2, ENABLE);
}

//=============================================================================
// 函数名称：main
// 功能概要：主函数
// 参数说明：无
// 返回值：int
//=============================================================================
int main(void)
{
    GPIO_Configuration();
    TIM2_PWM_Init();
    // Set PWM to 50% duty cycle
    TIM_SetCompare1(TIM2, 10000); // Assuming 20ms period, 50% duty cycle is 10ms
    TIM_SetCompare2(TIM2, 10000);
    TIM_SetCompare3(TIM2, 10000);
    TIM_SetCompare4(TIM2, 10000);
    while (1)
    {
        // Toggle PC13
        GPIOC->ODR ^= GPIO_Pin_13;
        Delay(7200000); // Adjust delay for desired blink rate
    }
}

//=============================================================================
// 函数名称：GPIO_Configuration
// 功能概要：GPIO 初始化
// 参数说明：无
// 返回值：无
//=============================================================================
void GPIO_Configuration(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
  
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE); 						 
    //=============================================================================
    // LED -> PC13
    //=============================================================================			 
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    // PWM -> PA0, PA1, PA2, PA3 (TIM2 CH1, CH2, CH3, CH4)
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
}


