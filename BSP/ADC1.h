
#ifndef __ADC1_H
#define __ADC1_H

#include "stm32f0xx.h"

#define ADC_PIN         GPIO_Pin_1
#define ADC_PORT        GPIOA
#define ADC_GPIO_CLC    RCC_AHBPeriph_GPIOA
#define ADC_CLC         RCC_APB2Periph_ADC1
#define DMA_CLC         RCC_AHBPeriph_DMA1

#define ADC1_DR_Address                0x40012440

extern  __IO uint32_t TempSensVoltmv;
extern  __IO uint32_t VrefIntVoltmv ;
extern  __IO uint32_t PC3Voltmv ;
extern  __IO uint32_t VbatVoltmv ;
extern  uint16_t RegularConvData_Tab[4];

void ADC1_DMA_Init(void);

#endif
