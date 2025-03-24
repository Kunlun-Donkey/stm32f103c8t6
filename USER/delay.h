#ifndef __DELAY_H
#define __DELAY_H

#include "stm32f10x.h"

// 微秒级延时
void Delay_us(uint32_t us);

// 毫秒级延时
void Delay_ms(uint32_t ms);

#endif // __DELAY_H
