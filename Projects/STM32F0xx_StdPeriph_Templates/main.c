/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx.h"
#include "main.h"
#include "LED.h"
#include "USART1.h"
#include "ADC1.h"
#include "TIM.h"



/** @addtogroup STM32F0xx_StdPeriph_Examples
* @{
*/


/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/* Buffer used for transmission */
uint8_t aTxBuffer[BUFFER_SIZE] = "UART";
uint8_t aTxUser[BUFFER_SIZE] = "UART";

/* Buffer used for reception */
uint8_t aRxBuffer[BUFFER_SIZE];
uint8_t RxUser[BUFFER_SIZE];


uint8_t TxCounter = 0;
extern __IO uint8_t RxCounter,ReceiveState;

uint8_t flag_5ms = 0;
uint8_t toggle = 0;
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
    toggle = 1;
  }else if(rx_data[0] == 0x02){
    //    memset(aTxBuffer, 0x02, BUFFER_SIZE);
    toggle = 0;
  }else if(rx_data[0] == 0x03){
    // LED_OFF();
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
  uint16_t Vout = 0;
  uint16_t Vin = 0;
  uint16_t Iout = 0;
  uint16_t Iin = 0;
  uint16_t V_0_10v = 0;
  // LED_Init();
  USART1_Init();
  ADC1_DMA_Init();
  TIM_Init();
  PWM_Init();
  while(1)
  {
    if(flag_5ms == 1){
      static uint8_t cnt_flag = 0;
      cnt_flag ++;
      flag_5ms = 0;
      Vout = RegularConvData_Tab[0];
      Vin = RegularConvData_Tab[1];
      Iout = RegularConvData_Tab[2];
      Iin = RegularConvData_Tab[3];
      V_0_10v = RegularConvData_Tab[4];
      if (cnt_flag == 200){
        cnt_flag = 0;
        //reset RxBuffer
        memset(aRxBuffer, 0x00, BUFFER_SIZE);
        RxCounter = 0;
        /* Test DMA1 TC flag */
        while((DMA_GetFlagStatus(DMA1_FLAG_TC1)) == RESET ); 
        /* Clear DMA TC flag */
        DMA_ClearFlag(DMA1_FLAG_TC1);
        //set TxBuffer
        aTxUser[0] = (Vout>>8)&0xff;
        aTxUser[1] = Vout & 0xff;
        aTxUser[2] = (Vin>>8)&0xff;
        aTxUser[3] = Vin & 0xff;
        aTxUser[4] = (Iout>>8)&0xff;
        aTxUser[5] = Iout & 0xff;
        aTxUser[6] = (Iin>>8)&0xff;
        aTxUser[7] = Iin & 0xff;
        memcpy(aTxBuffer,aTxUser,8);
        buffptr = aTxBuffer;
        TxCounter = 0;
        while(TxCounter < BUFFER_SIZE)
        {
          /* Send one byte from USART1 to USARTx */
          USART_SendData(USART1, buffptr[TxCounter++]);
          /* Loop until USART1 DR register is empty */
          while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET)
          {}
        }
      }
    }
    //    printf("PA1 ADC %d\r\n",RegularConvData_Tab[0]);
    // if (toggle == 0)
    // {
    //   LED_OFF();
    // }
    // else 
    // {
    //   LED_ON();
    // }  
    if(ReceiveState == 1){
      memcpy(RxUser,aRxBuffer,BUFFER_SIZE);
      resolve(RxUser);
      ReceiveState = 0;
    }
  }
}

/*****END OF FILE****/
