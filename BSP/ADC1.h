
#ifndef __ADC1_H
#define __ADC1_H

#include "stm32f0xx.h"

#define ADC_PIN         GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7
#define ADC_PORT        GPIOA
#define ADC_GPIO_CLC    RCC_AHBPeriph_GPIOA

#define ADC_PIN_2       GPIO_Pin_0
#define ADC_PORT_2      GPIOB
#define ADC_GPIO_CLC_2  RCC_AHBPeriph_GPIOB

#define ADC_CLC         RCC_APB2Periph_ADC1
#define ADC_Vout		ADC_Channel_4
#define ADC_Vin			ADC_Channel_5
#define ADC_Iout		ADC_Channel_6
#define ADC_Iin			ADC_Channel_7
#define ADC_0_10v		ADC_Channel_8

#define DMA_CLC         RCC_AHBPeriph_DMA1

#define ADC1_DR_Address                0x40012440

extern  uint16_t RegularConvData_Tab[6];

void ADC1_DMA_Init(void);

#endif
