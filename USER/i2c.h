/*
 * @Author: Kunlun-Donkey 1298394344@qq.com
 * @Date: 2025-03-24 13:31:35
 * @LastEditors: Kunlun-Donkey 1298394344@qq.com
 * @LastEditTime: 2025-03-24 13:32:17
 * @FilePath: \stm32f103c8t6\USER\i2c.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#ifndef __I2C_H
#define __I2C_H

#include "stm32f10x.h"

// 初始化模拟I2C
void I2C_Init(void);

// 发送I2C起始信号
void I2C_Start(void);

// 发送I2C停止信号
void I2C_Stop(void);

// 等待从机应答
uint8_t I2C_WaitAck(void);

// 发送应答信号
void I2C_SendAck(void);

// 发送非应答信号
void I2C_SendNack(void);

// 发送一个字节
void I2C_SendByte(uint8_t byte);

// 读取一个字节
uint8_t I2C_ReadByte(uint8_t ack);

#endif // __I2C_H
