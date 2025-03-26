#ifndef __BLUETOOTH_H
#define __BLUETOOTH_H

#include "stm32f10x.h"

// 初始化蓝牙模块
void Bluetooth_Init(void);

// 处理蓝牙接收到的数据
void Bluetooth_HandleData(uint8_t data);

// 蓝牙中断处理函数
void Bluetooth_IRQHandler(void);

#endif // __BLUETOOTH_H
