/*
 * @file    i2c.c
 * @brief   模拟I2C驱动实现
 */

#include "stm32f10x.h"
#include "i2c.h"
#include "delay.h"

// 定义SDA和SCL引脚
#define I2C_SDA_PIN GPIO_Pin_4
#define I2C_SCL_PIN GPIO_Pin_5
#define I2C_GPIO_PORT GPIOA

// 设置SDA为输出模式
#define SDA_OUT() { GPIO_InitTypeDef GPIO_InitStructure; \
                    GPIO_InitStructure.GPIO_Pin = I2C_SDA_PIN; \
                    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; \
                    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; \
                    GPIO_Init(I2C_GPIO_PORT, &GPIO_InitStructure); }

// 设置SDA为输入模式
#define SDA_IN() { GPIO_InitTypeDef GPIO_InitStructure; \
                   GPIO_InitStructure.GPIO_Pin = I2C_SDA_PIN; \
                   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; \
                   GPIO_Init(I2C_GPIO_PORT, &GPIO_InitStructure); }

// 操作SDA和SCL引脚
#define I2C_SDA_HIGH() GPIO_SetBits(I2C_GPIO_PORT, I2C_SDA_PIN)
#define I2C_SDA_LOW()  GPIO_ResetBits(I2C_GPIO_PORT, I2C_SDA_PIN)
#define I2C_SCL_HIGH() GPIO_SetBits(I2C_GPIO_PORT, I2C_SCL_PIN)
#define I2C_SCL_LOW()  GPIO_ResetBits(I2C_GPIO_PORT, I2C_SCL_PIN)
#define I2C_READ_SDA() GPIO_ReadInputDataBit(I2C_GPIO_PORT, I2C_SDA_PIN)

//=============================================================================
// 函数名称：I2C_Init
// 功能概要：初始化模拟I2C引脚
// 参数说明：无
// 返回值：无
//=============================================================================
void I2C_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    // 配置SCL为推挽输出
    GPIO_InitStructure.GPIO_Pin = I2C_SCL_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(I2C_GPIO_PORT, &GPIO_InitStructure);

    // 配置SDA为推挽输出
    SDA_OUT();
    I2C_SDA_HIGH();
    I2C_SCL_HIGH();
}

//=============================================================================
// 函数名称：I2C_Start
// 功能概要：发送I2C起始信号
// 参数说明：无
// 返回值：无
//=============================================================================
void I2C_Start(void)
{
    SDA_OUT();
    I2C_SDA_HIGH();
    I2C_SCL_HIGH();
    Delay_us(4);
    I2C_SDA_LOW();
    Delay_us(4);
    I2C_SCL_LOW();
}

//=============================================================================
// 函数名称：I2C_Stop
// 功能概要：发送I2C停止信号
// 参数说明：无
// 返回值：无
//=============================================================================
void I2C_Stop(void)
{
    SDA_OUT();
    I2C_SCL_LOW();
    I2C_SDA_LOW();
    Delay_us(4);
    I2C_SCL_HIGH();
    I2C_SDA_HIGH();
    Delay_us(4);
}

//=============================================================================
// 函数名称：I2C_WaitAck
// 功能概要：等待从机应答
// 参数说明：无
// 返回值：0-应答成功，1-应答失败
//=============================================================================
uint8_t I2C_WaitAck(void)
{
    uint8_t timeout = 255;
    SDA_IN();
    I2C_SDA_HIGH();
    Delay_us(1);
    I2C_SCL_HIGH();
    Delay_us(1);
    while (I2C_READ_SDA())
    {
        if (--timeout == 0)
        {
            I2C_Stop();
            return 1;
        }
    }
    I2C_SCL_LOW();
    return 0;
}

//=============================================================================
// 函数名称：I2C_SendAck
// 功能概要：发送应答信号
// 参数说明：无
// 返回值：无
//=============================================================================
void I2C_SendAck(void)
{
    I2C_SCL_LOW();
    SDA_OUT();
    I2C_SDA_LOW();
    Delay_us(2);
    I2C_SCL_HIGH();
    Delay_us(2);
    I2C_SCL_LOW();
}

//=============================================================================
// 函数名称：I2C_SendNack
// 功能概要：发送非应答信号
// 参数说明：无
// 返回值：无
//=============================================================================
void I2C_SendNack(void)
{
    I2C_SCL_LOW();
    SDA_OUT();
    I2C_SDA_HIGH();
    Delay_us(2);
    I2C_SCL_HIGH();
    Delay_us(2);
    I2C_SCL_LOW();
}

//=============================================================================
// 函数名称：I2C_SendByte
// 功能概要：发送一个字节
// 参数说明：byte - 要发送的字节
// 返回值：无
//=============================================================================
void I2C_SendByte(uint8_t byte)
{
    SDA_OUT();
    I2C_SCL_LOW();
    for (uint8_t i = 0; i < 8; i++)
    {
        if (byte & 0x80)
            I2C_SDA_HIGH();
        else
            I2C_SDA_LOW();
        byte <<= 1;
        Delay_us(2);
        I2C_SCL_HIGH();
        Delay_us(2);
        I2C_SCL_LOW();
        Delay_us(2);
    }
}

//=============================================================================
// 函数名称：I2C_ReadByte
// 功能概要：读取一个字节
// 参数说明：ack - 1发送ACK，0发送NACK
// 返回值：读取的字节
//=============================================================================
uint8_t I2C_ReadByte(uint8_t ack)
{
    uint8_t byte = 0;
    SDA_IN();
    for (uint8_t i = 0; i < 8; i++)
    {
        I2C_SCL_LOW();
        Delay_us(2);
        I2C_SCL_HIGH();
        byte <<= 1;
        if (I2C_READ_SDA())
            byte++;
        Delay_us(1);
    }
    if (ack)
        I2C_SendAck();
    else
        I2C_SendNack();
    return byte;
}
