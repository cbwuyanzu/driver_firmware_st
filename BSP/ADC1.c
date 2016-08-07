//=============================================================================
//adc for STM32F030
//=============================================================================

#include "ADC1.h"


uint16_t RegularConvData_Tab[6];


void ADC1_DMA_Init(void)
{
  GPIO_InitTypeDef    GPIO_InitStructure;
  ADC_InitTypeDef     ADC_InitStructure;

  /* ADC1 DeInit */  
  ADC_DeInit(ADC1);
 /* GPIOC Periph clock enable */
  RCC_AHBPeriphClockCmd(ADC_GPIO_CLC, ENABLE);
  RCC_AHBPeriphClockCmd(ADC_GPIO_CLC_2, ENABLE);
  /* ADC1 Periph clock enable */
  RCC_APB2PeriphClockCmd(ADC_CLC , ENABLE);

  GPIO_InitStructure.GPIO_Pin = ADC_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(ADC_PORT, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = ADC_PIN_2;
  GPIO_Init(ADC_PORT_2, &GPIO_InitStructure);

  /* Initialize ADC structure */
  ADC_StructInit(&ADC_InitStructure);
  
  /* Configure the ADC1 in continuous mode withe a resolution equal to 12 bits  */
  ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE; 
  ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
  ADC_InitStructure.ADC_ScanDirection = ADC_ScanDirection_Backward;
  ADC_Init(ADC1, &ADC_InitStructure); 

  ADC_ChannelConfig(ADC1, ADC_Vout , ADC_SampleTime_55_5Cycles); 
  ADC_ChannelConfig(ADC1, ADC_Vin, ADC_SampleTime_55_5Cycles); 
  ADC_ChannelConfig(ADC1, ADC_Iout, ADC_SampleTime_55_5Cycles); 
  ADC_ChannelConfig(ADC1, ADC_Iin, ADC_SampleTime_55_5Cycles); 
  ADC_ChannelConfig(ADC1, ADC_0_10v, ADC_SampleTime_55_5Cycles); 
  /* Convert the ADC1 temperature sensor  with 55.5 Cycles as sampling time */ 
  ADC_ChannelConfig(ADC1, ADC_Channel_TempSensor , ADC_SampleTime_55_5Cycles);  
  ADC_TempSensorCmd(ENABLE);
  
  /* ADC Calibration */
  ADC_GetCalibrationFactor(ADC1);
  
  /* ADC DMA request in circular mode */
  ADC_DMARequestModeConfig(ADC1, ADC_DMAMode_Circular);
  
  /* Enable ADC_DMA */
  ADC_DMACmd(ADC1, ENABLE);  
  
  /* Enable the ADC peripheral */
  ADC_Cmd(ADC1, ENABLE);     
  
  /* Wait the ADRDY flag */
  while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_ADRDY)); 
  
  /* ADC1 regular Software Start Conv */ 
  ADC_StartOfConversion(ADC1);

  DMA_InitTypeDef     DMA_InitStructure;
  RCC_AHBPeriphClockCmd(DMA_CLC , ENABLE);

  DMA_DeInit(DMA1_Channel1);	/* DMA1 Channel1 Config */
  DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)ADC1_DR_Address;//外设地址
  DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)RegularConvData_Tab;//内存地址
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;//外设作为数据传输的来源
  DMA_InitStructure.DMA_BufferSize = 6;//
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//外设地址寄存器不变
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;//内存地址寄存器不变
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;//数据宽度为16位
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;//数据宽度为16位
  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;//DMA_Priority设定DMA通道x的软件优先级
  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;//DMA通道x没有设置为内存到内存传输
  DMA_Init(DMA1_Channel1, &DMA_InitStructure);

  DMA_Cmd(DMA1_Channel1, ENABLE);/* DMA1 Channel1 enable */
}
