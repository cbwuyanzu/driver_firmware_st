/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx.h"
#include "main.h"
#include "LED.h"
#include "USART1.h"
#include "ADC1.h"


/**
* @brief  Inserts a delay time.
* @param  nTime: specifies the delay time length, in milliseconds.
* @retval None
*/
void Delay(__IO uint32_t nTime)
{
  __IO uint32_t TimingDelay = nTime;
  while(TimingDelay != 0)
  {
    TimingDelay --;
  }
}




/** @addtogroup STM32F0xx_StdPeriph_Examples
* @{
*/


/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/* Buffer used for transmission */
uint8_t aTxBuffer[BUFFER_SIZE] = "UART";


/* Buffer used for reception */
uint8_t aRxBuffer[BUFFER_SIZE];
uint8_t RxUser[BUFFER_SIZE];


uint8_t TxCounter = 0;
extern __IO uint8_t ReceiveState;

uint8_t flag_1s = 0;

/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/
/**
* @brief  resolve the rx_data
* @param  RxUser: pointer to the rx_data.
* @retval None
*/
void resolve(uint8_t *rx_data)
{
  if(rx_data[0] == 0x01){
    //    memset(aTxBuffer, 0x01, BUFFER_SIZE);
    LED_ON();
  }else if(rx_data[0] == 0x02){
    //    memset(aTxBuffer, 0x02, BUFFER_SIZE);
    LED_OFF();
  }else if(rx_data[0] == 0x03){
    LED_OFF();
  }
}
/**
* @brief  Main program.
* @param  None
* @retval None
*/

int main(void)
{
  uint8_t *buffptr;
  LED_Init();
  USART1_Init();
  ADC1_DMA_Init();
  while(1)
  {
    if(flag_1s == 1){
      TxCounter = 0;
      ReceiveState = 0;
      buffptr = aTxBuffer;
      flag_1s = 0;
      while(TxCounter < BUFFER_SIZE)
      {
        /* Send one byte from USART1 to USARTx */
        USART_SendData(USART1, buffptr[TxCounter++]);
        /* Loop until USART1 DR register is empty */
        while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET)
        {}
      }
    }
    //    printf("PA1 ADC %d\r\n",RegularConvData_Tab[0]);
    
    if(ReceiveState == 1){
      memcpy(RxUser,aRxBuffer,BUFFER_SIZE);
      resolve(RxUser);
      ReceiveState = 0;
    }
    //    for (int i = 0;i < 10;i++)
    //    {
    //      Delay(0xfffff);
    //    }
  }
}

/*****END OF FILE****/
