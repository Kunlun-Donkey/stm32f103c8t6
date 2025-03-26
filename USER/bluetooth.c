/*
 * @Author: Kunlun-Donkey 1298394344@qq.com
 * @Date: 2025-03-26 20:32:59
 * @LastEditors: Kunlun-Donkey 1298394344@qq.com
 * @LastEditTime: 2025-03-26 20:33:51
 * @FilePath: \stm32f103c8t6\USER\bluetooth.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include "bluetooth.h"
#include "uart.h"
#include "servo_control.h"
#include "stm32f10x_usart.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"

// 初始化蓝牙模块
void Bluetooth_Init(void) {
    // 开启 GPIOB 和 USART3 时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);

    // 配置 PB10 为 USART3_TX
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    // 配置 PB11 为 USART3_RX
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    // 配置 USART3
    USART_InitTypeDef USART_InitStructure;
    USART_InitStructure.USART_BaudRate = 9600; // 根据蓝牙模块要求设置波特率
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USART3, &USART_InitStructure);

    // 开启 USART3 接收中断
    USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);

    // 开启 USART3
    USART_Cmd(USART3, ENABLE);

    // 配置 NVIC
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

// 处理蓝牙接收到的数据
void Bluetooth_HandleData(uint8_t data) {
    switch (data) {
        case 0x01: // 指令 1：放松趴下
            if (actionCallbacks.RelaxedGetDown) {
                actionCallbacks.RelaxedGetDown();
            }
            break;
        case 0x02: // 指令 2：站立
            if (actionCallbacks.Upright) {
                actionCallbacks.Upright();
            }
            break;
        case 0x03: // 指令 3：趴下
            if (actionCallbacks.GetDown) {
                actionCallbacks.GetDown();
            }
            break;
        case 0x04: // 指令 4：坐下
            if (actionCallbacks.Sit) {
                actionCallbacks.Sit();
            }
            break;
        default:
            // 未定义的指令
            break;
    }
}

// 蓝牙中断处理函数
void Bluetooth_IRQHandler(void) {
    if (USART_GetITStatus(USART3, USART_IT_RXNE) != RESET) {
        uint8_t data = USART_ReceiveData(USART3); // 读取接收到的数据
        Bluetooth_HandleData(data); // 处理接收到的数据
    }
}
