#ifndef __BSP_TIME_H
#define	__BSP_TIME_H

#include "stm32f0xx.h"
//CONFIG FOR PWM_OUT
#define TIM_PWM         TIM2
#define GPIO_CLC_PWM    RCC_AHBPeriph_GPIOA
#define GPIO_PIN_PWM    GPIO_Pin_2
#define GPIO_PORT_PWM   GPIOA
#define PIN_SOURCE_PWM  GPIO_PinSource2
#define AF_PWM          GPIO_AF_2
#define TIM_CLC_PWM     RCC_APB1Periph_TIM2
#define FREQUENCY_PWM   7800
#define INIT_DUTY       850

//CONFIG FOR flag_5ms
#define TIM_TIMER       TIM3
#define IRQ_TIMER       TIM3_IRQn
#define TIM_CLC_TIMER   RCC_APB1Periph_TIM3
#define FREQUENCY_TIMER 6000000
#define CCR1_Val        30000


void PWM_Init(void);
void TIM_Init(void);
void PWM_Set(uint16_t value);
#endif
