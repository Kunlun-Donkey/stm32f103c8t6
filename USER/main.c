/*
 * @Author: Kunlun-Donkey 1298394344@qq.com
 * @Date: 2025-03-24 11:20:24
 * @LastEditors: Kunlun-Donkey 1298394344@qq.com
 * @LastEditTime: 2025-03-25 09:34:25
 * @FilePath: \stm32f103c8t6\USER\main.c
 * @Description: 实现舵机控制的PWM输出、LED闪烁测试以及模拟I2C通信的初始化和主循环逻辑。
 */


#include "stm32f10x.h"
#include "stm32f10x_tim.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "motor.h" // 添加头文件引用
#include "delay.h" // 添加延时头文件
#include "oled.h" // 添加 OLED 头文件引用
#include "oled_faces.h" // 添加表情数据头文件
#include "uart.h" // 添加 UART 头文件
#include "types.h" // 添加类型定义头文件

// 函数声明
void GPIO_Configuration(void);

//=============================================================================
// 函数名称：main
// 功能概要：主函数
// 参数说明：无
// 返回值：int
//=============================================================================
int main(void)
{
    GPIO_Configuration();
    Motor_Init(); // 调用 motor.c 中的初始化函数
    OLED_Init();  // 初始化 OLED
    UART1_Init(115200); // 初始化 UART1，波特率为 115200

    UART1_SendString("System initialized.\r\n"); // 发送日志信息

    // 显示表情 "hello"
    OLED_ShowImage(Face_hello);

    // 设置舵机初始角度为90度
    Motor_SetAngle(MOTOR_LEFT_FRONT, 30);
    Motor_SetAngle(MOTOR_RIGHT_FRONT, 60);
    Motor_SetAngle(MOTOR_LEFT_REAR, 90);
    Motor_SetAngle(MOTOR_RIGHT_REAR, 120);

    while (1)
    {
        if (UART1_RxHead != UART1_RxTail) { // 检查是否有接收到的数据
            uint8_t received = UART1_GetChar();
            UART1_SendChar(received); // 回显接收到的数据
        }

        // Toggle PC13
        GPIOC->ODR ^= GPIO_Pin_13;
        UART1_SendString("Toggling LED.\r\n"); // 发送日志信息
        Delay_ms(500); // 使用毫秒级延时
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


