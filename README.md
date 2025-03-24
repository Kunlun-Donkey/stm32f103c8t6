<!--
 * @Author: Kunlun-Donkey 1298394344@qq.com
 * @Date: 2025-03-24 11:08:25
 * @LastEditors: Kunlun-Donkey 1298394344@qq.com
 * @LastEditTime: 2025-03-24 11:16:43
 * @FilePath: \stm32f103c8t6\README.md
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
-->
# stm32f103c8t6
destop dog

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

请确保正确连接舵机信号线到对应的引脚，并配置电源和地线。
请确保正确连接LED到PC13引脚，并配置限流电阻。
请确保正确连接I2C设备到PA4和PA5引脚，并配置上拉电阻。

## main函数说明
`main`函数是程序的入口，主要完成以下功能：
1. 初始化系统时钟和外设。
2. 配置TIM2的四个通道用于舵机的PWM输出。
3. 配置PC13作为灯光闪烁测试的GPIO输出。
4. 配置PA4和PA5用于模拟I2C通信。
5. 进入主循环，执行具体的控制逻辑。

请根据实际需求修改`main`函数的实现。
