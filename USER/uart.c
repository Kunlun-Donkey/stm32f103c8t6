#include "stm32f10x.h"
#include "uart.h"

#define UART1_RX_BUFFER_SIZE 128
static uint8_t UART1_RxBuffer[UART1_RX_BUFFER_SIZE];
static volatile uint16_t UART1_RxHead = 0;
static volatile uint16_t UART1_RxTail = 0;

void UART1_Init(uint32_t baudrate) {
    // 开启时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA, ENABLE);

    // 配置 PA9 (TX) 和 PA10 (RX)
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; // TX
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; // RX
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // 配置 USART1
    USART_InitTypeDef USART_InitStructure;
    USART_InitStructure.USART_BaudRate = baudrate;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USART1, &USART_InitStructure);

    // 启用 USART1
    USART_Cmd(USART1, ENABLE);

    // 配置 USART1 接收中断
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

void UART1_SendChar(char c) {
    while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
    USART_SendData(USART1, c);
}

void UART1_SendString(const char *str) {
    while (*str) {
        UART1_SendChar(*str++);
    }
}

uint8_t UART1_GetChar(void) {
    if (UART1_RxHead == UART1_RxTail) {
        return 0; // 缓冲区为空
    }
    uint8_t data = UART1_RxBuffer[UART1_RxTail];
    UART1_RxTail = (UART1_RxTail + 1) % UART1_RX_BUFFER_SIZE;
    return data;
}

void USART1_IRQHandler(void) {
    if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) {
        uint8_t data = USART_ReceiveData(USART1);
        uint16_t nextHead = (UART1_RxHead + 1) % UART1_RX_BUFFER_SIZE;
        if (nextHead != UART1_RxTail) { // 检查缓冲区是否溢出
            UART1_RxBuffer[UART1_RxHead] = data;
            UART1_RxHead = nextHead;
        }
    }
}
