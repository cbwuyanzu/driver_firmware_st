//=============================================================================
//UART FOR STM32F030
//=============================================================================

#include "USART1.h"


/* USART初始化 */
void USART1_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStruct;

	RCC_AHBPeriphClockCmd(GPIO_CLC, ENABLE);  //使能GPIOA的时钟
	RCC_APB2PeriphClockCmd(UART_CLC, ENABLE);//使能USART的时钟
	/* USART CONFIG */
	GPIO_PinAFConfig(UART_PORT, UART_TX_PIN_SOURCE, UART_AF);//PA9	TX
	GPIO_PinAFConfig(UART_PORT, UART_RX_PIN_SOURCE, UART_AF);//PA10 RX

	GPIO_InitStructure.GPIO_Pin = UART_TX_PIN | UART_RX_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(UART_PORT, &GPIO_InitStructure);


	/* USART1 CONFIG */
	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

	USART_Init(USART1, &USART_InitStructure);
	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);           //ENABLE INTERRUPT
	USART_Cmd(USART1, ENABLE);                             //ENABLE USART1

	/* USART1 NVIC CONFIG */
	NVIC_InitStruct.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPriority = 0x01;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStruct);

}



/**
  * @brief  Retargets the C library printf function to the USART.
  * @param  None
  * @retval None
  */
PUTCHAR_PROTOTYPE
{
  /* Place your implementation of fputc here */
  /* e.g. write a character to the USART */
  USART_SendData(USART1, (uint8_t) ch);

  /* Loop until the end of transmission */
  while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
  {}

  return ch;
}
