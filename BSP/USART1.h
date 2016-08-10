
#ifndef __BSP_USART1_H
#define __BSP_USART1_H

#include "stm32f0xx.h"
#include <stdio.h>

#define UART_TX_PIN             GPIO_Pin_9
#define UART_RX_PIN             GPIO_Pin_10
#define UART_TX_PIN_SOURCE      GPIO_PinSource9
#define UART_RX_PIN_SOURCE      GPIO_PinSource10
#define UART_AF                 GPIO_AF_1
#define UART_PORT               GPIOA
#define GPIO_CLC                RCC_AHBPeriph_GPIOA
#define UART_CLC                RCC_APB2Periph_USART1


void USART1_Init(void);

#endif
