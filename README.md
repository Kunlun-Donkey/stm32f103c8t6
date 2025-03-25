<!--
 * @Author: Kunlun-Donkey 1298394344@qq.com
 * @Date: 2025-03-24 11:08:25
 * @LastEditors: Kunlun-Donkey 1298394344@qq.com
 * @LastEditTime: 2025-03-24 14:49:49
 * @FilePath: \stm32f103c8t6\README.md
 * @Description: 
-->
- [简介/Introduction](#简介introduction)
- [本项目说明](#本项目说明)
  - [IO说明](#io说明)
  - [时钟说明](#时钟说明)
  - [main函数说明](#main函数说明)
  - [使用材料](#使用材料)
  - [UART1 接口使用说明](#uart1-接口使用说明)
    - [UART1 IO 说明](#uart1-io-说明)
    - [初始化 UART1](#初始化-uart1)
    - [发送数据](#发送数据)
    - [接收数据](#接收数据)
    - [中断处理](#中断处理)
- [附录](#附录)
  - [stm32f103c8t6 IO 功能表](#stm32f103c8t6-io-功能表)
  - [常用外设](#常用外设)



# 简介/Introduction
本项目 是一个基于stm32f103c8t6的桌面机器人项目，使用一片单片机，配合四个mg90舵机和一个OLED屏幕实现多种动作和表情。本项目从初始代码搭建，可以作为其他使用stm32f103芯片的项目的工程模板，使用标准库函数版本去开发.
后期考虑扩展添加串口模块和语音模块，和wifi模块，等待持续更新。





# 本项目说明
## IO说明
使用TIM2的四个通道作为舵机的PWM输出口，具体配置如下：
- TIM2_CH1 (PA0): 用于舵机控制的PWM信号输出
- TIM2_CH2 (PA1): 用于舵机控制的PWM信号输出
- TIM2_CH3 (PA2): 用于舵机控制的PWM信号输出
- TIM2_CH4 (PA3): 用于舵机控制的PWM信号输出
- PWM频率：50Hz
- 占空比范围：5% - 10%（对应舵机的最小和最大角度）
- PC13: 用于灯光闪烁测试的GPIO输出口
- PA4: 用于模拟I2C的SDA（数据线）
- PA5: 用于模拟I2C的SCL（时钟线）
- PA9: **TX (发送)**
- PA10: **RX (接收)**


请确保正确连接舵机信号线到对应的引脚，并配置电源和地线。
请确保正确连接LED到PC13引脚，并配置限流电阻。
请确保正确连接I2C设备到PA4和PA5引脚，并配置上拉电阻。

## 时钟说明
系统时钟配置为72MHz，使用外部8MHz晶振作为时钟源，并通过PLL倍频到72MHz。  
- HCLK（AHB时钟）：72MHz  
- PCLK1（APB1时钟）：36MHz  
- PCLK2（APB2时钟）：72MHz  

请确保外部晶振电路正确连接并稳定工作。

## main函数说明
`main`函数是程序的入口，主要完成以下功能：
1. 初始化系统时钟和外设。
2. 配置TIM2的四个通道用于舵机的PWM输出。
3. 配置PC13作为灯光闪烁测试的GPIO输出。
4. 配置PA4和PA5用于模拟I2C通信。
5. 进入主循环，执行具体的控制逻辑。

请根据实际需求修改`main`函数的实现。

## 使用材料
| 材料名称             | 数量 | 描述                     |
|----------------------|------|--------------------------|
| SG90舵机             | 4    | 用于控制运动的微型舵机   |
| STM32F103C8T6芯片     | 1    | 项目核心控制单元         |
| 1.3寸OLED            | 1    | 用于显示信息的小型屏幕   |
| LED灯                | 2    | 用于指示状态的发光二极管 |
| 复位按键             | 1    | 用于复位系统的按键       |
| 六角开关             | 1    | 用于控制电路的开关       |
| 杜邦线               | 若干 | 用于连接电路的导线       |
| AMS1117-3.3V电源芯片 | 1    | 用于提供3.3V稳压电源     |
| 5V充放电一体模块     | 1    | 用于锂电池的充放电管理   |
| 3.7V锂电池           | 1    | 提供电源的锂电池         |

## UART1 接口使用说明

### UART1 IO 说明


### 初始化 UART1
在主程序中调用 `UART1_Init` 函数初始化 UART1，指定波特率。例如：
```c
UART1_Init(115200); // 初始化 UART1，波特率为 115200
```

### 发送数据
使用 `UART1_SendChar` 或 `UART1_SendString` 函数发送数据：
```c
UART1_SendChar('A'); // 发送单个字符
UART1_SendString("Hello, UART1!\r\n"); // 发送字符串
```

### 接收数据
通过 `UART1_GetChar` 函数获取接收到的数据：
```c
uint8_t received = UART1_GetChar();
if (received) {
    // 处理接收到的数据
}
```

### 中断处理
UART1 接收数据通过中断方式实现，接收到的数据会存储在内部缓冲区中，用户可以通过 `UART1_GetChar` 函数读取。

---
# 附录
## stm32f103c8t6 IO 功能表
| 引脚      | 功能描述     | 引脚功能 |
| ---       | ---           | --- |
|PA0	    |通用输入输出	|ADC1_IN0, TIM2_CH1, EXTI0
|PA1	    |通用输入输出	|ADC1_IN1, TIM2_CH2, EXTI1
|PA2	    |通用输入输出	|ADC1_IN2, TIM2_CH3, EXTI2
|PA3	    |通用输入输出	|ADC1_IN3, TIM2_CH4, EXTI3
|PA4	    |通用输入输出	|ADC1_IN4, I2C1_SDA, EXTI4
|PA5	    |通用输入输出	|ADC1_IN5, SPI1_SCK, TIM2_CH1
|PA6	    |通用输入输出	|ADC1_IN6, SPI1_MISO
|PA7	    |通用输入输出	|ADC1_IN7, SPI1_MOSI
|PA8	    |通用输入输出	|MCO, TIM1_CH1
|PA9	    |通用输入输出	|USART1_TX
|PA10	    |通用输入输出	|USART1_RX
|PA11	    |通用输入输出	|USB_DM
|PA12	    |通用输入输出	|USB_DP
|PA13	    |通用输入输出	|SWDIO
|PA14	    |通用输入输出	|SWCLK
|PA15	    |通用输入输出	|JTDI, TIM2_CH1
|PB0	    |通用输入输出	|ADC1_IN8, SPI1_NSS
|PB1	    |通用输入输出	|ADC1_IN9, SPI1_SCK
|PB2	    |通用输入输出	|I2C1_SCL, SPI1_MISO
|PB3	    |通用输入输出	|I2C1_SDA, SPI1_MOSI
|PB4	    |通用输入输出	|USART2_TX
|PB5	    |通用输入输出	|USART2_RX
|PB6	    |通用输入输出	|SPI1_SCK
|PB7	    |通用输入输出	|SPI1_MISO
|PB8	    |通用输入输出	|USART1_TX
|PB9	    |通用输入输出	|USART1_RX
|PB10	    |通用输入输出	|SPI1_NSS
|PB11	    |通用输入输出	|SPI1_MISO
|PB12	    |通用输入输出	|SPI1_MOSI
|PB13	    |通用输入输出	|SWDIO
|PB14	    |通用输入输出	|SWCLK
|PB15	    |通用输入输出	|JTDO

## 常用外设
USART：串口通信，常见于 PA9 (TX), PA10 (RX), PB6 (TX), PB7 (RX)。

SPI：串行外设接口，常见于 PA5 (SCK), PA6 (MISO), PA7 (MOSI)，以及 PB3 (SCK), PB4 (MISO), PB5 (MOSI)。

I2C：串行外设接口，PA6 (SCL), PA7 (SDA)，以及 PB6 (SCL), PB7 (SDA)。

ADC：模拟输入引脚，PA0 至 PA7, PB0, PB1，均为 ADC 通道。

TIM：定时器功能，PA0 至 PA7 以及 PB6, PB7 等。

调试与调试接口
SWDIO (PA13) 和 SWCLK (PA14) 为调试接口（Serial Wire Debug）。

JTDI (PA15)，JTDO (PB15) 是用于 JTAG 调试的接口。
