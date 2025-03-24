#ifndef __UART_H
#define __UART_H

#include "stm32f10x.h"

void UART1_Init(uint32_t baudrate);
void UART1_SendChar(char c);
void UART1_SendString(const char *str);
uint8_t UART1_GetChar(void); // 新增：获取接收到的字符

#endif // __UART_H
