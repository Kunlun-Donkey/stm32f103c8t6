#ifndef __OLED_H
#define __OLED_H

#include "stm32f10x.h"

// 初始化OLED
void OLED_Init(void);

// 清屏
void OLED_Clear(void);

// 显示一个字符
void OLED_ShowChar(uint8_t x, uint8_t y, char chr);

// 显示一个汉字
void OLED_ShowChinese(uint8_t x, uint8_t y, uint8_t index);

// 显示图片
void OLED_ShowImage(const uint8_t *image);

#endif // __OLED_H
