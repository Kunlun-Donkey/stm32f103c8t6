/*
 * @Author: Kunlun-Donkey 1298394344@qq.com
 * @Date: 2025-03-24 13:46:14
 * @LastEditors: Kunlun-Donkey 1298394344@qq.com
 * @LastEditTime: 2025-03-25 09:25:47
 * @FilePath: \stm32f103c8t6\USER\oled.c
 * @Description: OLED显示驱动实现
 */


#include "stm32f10x.h"
#include "oled.h"
#include "i2c.h"
#include "delay.h"
#include "oled_faces.h" // 添加表情数据头文件

// OLED I2C地址
#define OLED_ADDRESS 0x78

// OLED命令模式和数据模式
#define OLED_CMD  0x00
#define OLED_DATA 0x40

//=============================================================================
// 函数名称：OLED_WriteByte
// 功能概要：向OLED写入一个字节
// 参数说明：data - 要写入的数据
//           cmd  - 0表示命令，1表示数据
// 返回值：无
//=============================================================================
void OLED_WriteByte(uint8_t data, uint8_t cmd)
{
    I2C_Start();
    I2C_SendByte(OLED_ADDRESS); // 发送OLED地址
    I2C_WaitAck();
    I2C_SendByte(cmd ? OLED_DATA : OLED_CMD); // 发送命令或数据标志
    I2C_WaitAck();
    I2C_SendByte(data); // 发送数据
    I2C_WaitAck();
    I2C_Stop();
}

//=============================================================================
// 函数名称：OLED_Init
// 功能概要：初始化OLED
// 参数说明：无
// 返回值：无
//=============================================================================
void OLED_Init(void)
{
    I2C_Init_Soft();
    Delay_ms(100); // 等待OLED上电稳定

    // 初始化命令
    OLED_WriteByte(0xAE, OLED_CMD); // 关闭显示
    OLED_WriteByte(0x20, OLED_CMD); // 设置内存寻址模式
    OLED_WriteByte(0x10, OLED_CMD); // 页地址模式
    OLED_WriteByte(0xB0, OLED_CMD); // 设置页起始地址
    OLED_WriteByte(0xC8, OLED_CMD); // 设置COM扫描方向
    OLED_WriteByte(0x00, OLED_CMD); // 设置低列地址
    OLED_WriteByte(0x10, OLED_CMD); // 设置高列地址
    OLED_WriteByte(0x40, OLED_CMD); // 设置起始行地址
    OLED_WriteByte(0x81, OLED_CMD); // 设置对比度
    OLED_WriteByte(0xFF, OLED_CMD); // 最大对比度
    OLED_WriteByte(0xA1, OLED_CMD); // 设置段重映射
    OLED_WriteByte(0xA6, OLED_CMD); // 正常显示
    OLED_WriteByte(0xA8, OLED_CMD); // 设置多路复用比率
    OLED_WriteByte(0x3F, OLED_CMD); // 1/64占空比
    OLED_WriteByte(0xA4, OLED_CMD); // 输出跟随RAM内容
    OLED_WriteByte(0xD3, OLED_CMD); // 设置显示偏移
    OLED_WriteByte(0x00, OLED_CMD); // 无偏移
    OLED_WriteByte(0xD5, OLED_CMD); // 设置显示时钟分频比/振荡器频率
    OLED_WriteByte(0xF0, OLED_CMD); // 设置分频比
    OLED_WriteByte(0xD9, OLED_CMD); // 设置预充电周期
    OLED_WriteByte(0x22, OLED_CMD); 
    OLED_WriteByte(0xDA, OLED_CMD); // 设置COM引脚硬件配置
    OLED_WriteByte(0x12, OLED_CMD); 
    OLED_WriteByte(0xDB, OLED_CMD); // 设置VCOMH电压倍率
    OLED_WriteByte(0x20, OLED_CMD); 
    OLED_WriteByte(0x8D, OLED_CMD); // 开启电荷泵
    OLED_WriteByte(0x14, OLED_CMD); 
    OLED_WriteByte(0xAF, OLED_CMD); // 打开显示
}

//=============================================================================
// 函数名称：OLED_Clear
// 功能概要：清屏
// 参数说明：无
// 返回值：无
//=============================================================================
void OLED_Clear(void)
{
    for (uint8_t page = 0; page < 8; page++)
    {
        OLED_WriteByte(0xB0 + page, OLED_CMD); // 设置页地址
        OLED_WriteByte(0x00, OLED_CMD);       // 设置低列地址
        OLED_WriteByte(0x10, OLED_CMD);       // 设置高列地址
        for (uint8_t col = 0; col < 128; col++)
        {
            OLED_WriteByte(0x00, OLED_DATA); // 清空数据
        }
    }
}

//=============================================================================
// 函数名称：OLED_ShowChar
// 功能概要：显示一个字符
// 参数说明：x - 起始列
//           y - 起始页
//           chr - 要显示的字符
// 返回值：无
//=============================================================================
void OLED_ShowChar(uint8_t x, uint8_t y, char chr)
{
    uint8_t c = chr - ' '; // ASCII偏移
    OLED_WriteByte(0xB0 + y, OLED_CMD); // 设置页地址
    OLED_WriteByte(((x & 0xF0) >> 4) | 0x10, OLED_CMD); // 设置高列地址
    OLED_WriteByte((x & 0x0F), OLED_CMD); // 设置低列地址
    for (uint8_t i = 0; i < 6; i++) // 假设使用6x8字体
    {
        OLED_WriteByte(OLED_F6x8[c][i], OLED_DATA); // 写入字体数据
    }
}

//=============================================================================
// 函数名称：OLED_ShowString
// 功能概要：显示字符串
// 参数说明：x - 起始列
//           y - 起始页
//           str - 要显示的字符串
// 返回值：无
//=============================================================================
void OLED_ShowString(uint8_t x, uint8_t y, const char *str)
{
    while (*str)
    {
        OLED_ShowChar(x, y, *str);
        x += 6; // 假设每个字符宽度为6像素
        if (x > 122) // 超过屏幕宽度则换行
        {
            x = 0;
            y++;
        }
        str++;
    }
}

//=============================================================================
// 函数名称：OLED_ShowChinese
// 功能概要：显示一个汉字
// 参数说明：x - 起始列
//           y - 起始页
//           index - 汉字索引
// 返回值：无
//=============================================================================
void OLED_ShowChinese(uint8_t x, uint8_t y, uint8_t index)
{
    for (uint8_t i = 0; i < 2; i++) // 假设使用16x16字体
    {
        OLED_WriteByte(0xB0 + y + i, OLED_CMD); // 设置页地址
        OLED_WriteByte(((x & 0xF0) >> 4) | 0x10, OLED_CMD); // 设置高列地址
        OLED_WriteByte((x & 0x0F), OLED_CMD); // 设置低列地址
        for (uint8_t j = 0; j < 16; j++)
        {
           //OLED_WriteByte(font16x16[index][i * 16 + j], OLED_DATA); // 写入字体数据
        }
    }
}

//=============================================================================
// 函数名称：OLED_ShowImage
// 功能概要：显示图片
// 参数说明：image - 图片数据指针
// 返回值：无
//=============================================================================
void OLED_ShowImage(const uint8_t *image)
{
    for (uint8_t page = 0; page < 8; page++)
    {
        OLED_WriteByte(0xB0 + page, OLED_CMD); // 设置页地址
        OLED_WriteByte(0x00, OLED_CMD);       // 设置低列地址
        OLED_WriteByte(0x10, OLED_CMD);       // 设置高列地址
        for (uint8_t col = 0; col < 128; col++)
        {
            OLED_WriteByte(image[page * 128 + col], OLED_DATA); // 写入图片数据
        }
    }
}
