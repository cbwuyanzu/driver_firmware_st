//=============================================================================
//TIM DRIVER
//=============================================================================
#include "TIM.h"


uint16_t PrescalerValue = 0;

uint16_t TimerPeriod = 0;
uint16_t Channel3Pulse = 0;

//PWM INIT FOR STM32F030
void PWM_Init(void)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
  TIM_OCInitTypeDef  TIM_OCInitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;
  
  /* GPIOA Clocks enable */
  RCC_AHBPeriphClockCmd(GPIO_CLC_PWM, ENABLE);
  
  /* GPIOA Configuration: Channel 3 as alternate function push-pull */
  GPIO_InitStructure.GPIO_Pin = GPIO_PIN_PWM;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
  GPIO_Init(GPIO_PORT_PWM, &GPIO_InitStructure);
  
  GPIO_PinAFConfig(GPIO_PORT_PWM, PIN_SOURCE_PWM, AF_PWM);
  
  /* TIM1 Configuration ---------------------------------------------------
  Generate PWM signals:
  TIM1 input clock (TIM1CLK) is set to APB2 clock (PCLK2)    
  => TIM1CLK = PCLK2 = SystemCoreClock
  TIM1CLK = SystemCoreClock, Prescaler = 0, TIM1 counter clock = SystemCoreClock
  SystemCoreClock is set to 48 MHz for STM32F0xx devices
  
  The objective is to generate 7 PWM signal at 7.8 KHz:
  - TIM1_Period = (SystemCoreClock / 7800) - 1
  The channel 1 and channel 1N duty cycle is set to 50%

  The Timer pulse is calculated as follows:
  - ChannelxPulse = DutyCycle * (TIM1_Period - 1) / 100
  
  Note: 
  SystemCoreClock variable holds HCLK frequency and is defined in system_stm32f0xx.c file.
  Each time the core clock (HCLK) changes, user had to call SystemCoreClockUpdate()
  function to update SystemCoreClock variable value. Otherwise, any configuration
  based on this variable will be incorrect. 
  ----------------------------------------------------------------------- */
  /* Compute the value to be set in ARR regiter to generate signal frequency at 17.57 Khz */
  TimerPeriod = (SystemCoreClock / FREQUENCY_PWM ) - 1;
  /* Compute CCR1 value to generate a duty cycle at 50% for channel 1 and 1N */
  Channel3Pulse = (uint16_t) (((uint32_t) INIT_DUTY * (TimerPeriod - 1)) / 1000);
  
  /* TIM14 clock enable */
  RCC_APB1PeriphClockCmd(TIM_CLC_PWM , ENABLE);
  
  /* Time Base configuration */
  TIM_TimeBaseStructure.TIM_Prescaler = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseStructure.TIM_Period = TimerPeriod;
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
  
  TIM_TimeBaseInit(TIM_PWM, &TIM_TimeBaseStructure);
  
  /* Channel 3 Configuration in PWM mode */
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;
  TIM_OCInitStructure.TIM_Pulse = Channel3Pulse;
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
  TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;
  TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
  TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset;
  
  TIM_OC3Init(TIM_PWM, &TIM_OCInitStructure);
  
  /* TIM14 counter enable */
  TIM_Cmd(TIM_PWM, ENABLE);
  
  /* TIM14 Main Output Enable */
  TIM_CtrlPWMOutputs(TIM_PWM, ENABLE);
}

//CONFIG PWM DUTY FOR STM32F030
void PWM_Set(uint16_t value){
  Channel3Pulse = (uint16_t) (((uint32_t) value * (TimerPeriod - 1)) / 1000);
  TIM_SetCompare3(TIM_PWM,Channel3Pulse);
}


void TIM_Init(void)
{
  TIM_TimeBaseInitTypeDef TIM_BaseInitStructure;
  TIM_OCInitTypeDef TIM_OCInitStructure;
  // TIM_BDTRInitTypeDef TIM_BDTRInitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;
  
  RCC_APB1PeriphClockCmd(TIM_CLC_TIMER, ENABLE); 
  NVIC_InitStructure.NVIC_IRQChannel = IRQ_TIMER;
  NVIC_InitStructure.NVIC_IRQChannelPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  
  PrescalerValue = (uint16_t) (SystemCoreClock  / FREQUENCY_TIMER) - 1;
  TIM_BaseInitStructure.TIM_Period = 65535;
  TIM_BaseInitStructure.TIM_Prescaler = 0;
  TIM_BaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
  TIM_BaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_BaseInitStructure.TIM_RepetitionCounter = 0;
  TIM_TimeBaseInit(TIM_TIMER, &TIM_BaseInitStructure);
  TIM_PrescalerConfig(TIM_TIMER,PrescalerValue,TIM_PSCReloadMode_Immediate);
  /* Output Compare Timing Mode configuration: Channel1 */
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_Timing;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = CCR1_Val;
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
  
  TIM_OC1Init(TIM_TIMER, &TIM_OCInitStructure);
  
  TIM_OC1PreloadConfig(TIM_TIMER, TIM_OCPreload_Disable);
  
  /* TIM Interrupts enable */
  TIM_ITConfig(TIM_TIMER, TIM_IT_CC1, ENABLE);
  
  /* TIM3 enable counter */
  TIM_Cmd(TIM_TIMER, ENABLE);
}




/******************************************************************************/
/*                 STM32F0xx Peripherals Interrupt Handlers                   */
/******************************************************************************/
//moved to stm32f0xx_it.c