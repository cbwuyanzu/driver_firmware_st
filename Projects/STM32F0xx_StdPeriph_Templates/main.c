/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx.h"
#include "main.h"
#include "LED.h"
#include "USART1.h"
#include "ADC1.h"
#include "TIM.h"
#include "FLASH.h"


/** @addtogroup STM32F0xx_StdPeriph_Examples
* @{
*/


/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
//PWM parameter
#define MAX_PWM         800//800/1000=80%
#define MIN_PWM         50//50/1000=50%
#define MAX_0_10V_ADC   0xBE0
#define MIN_0_10V_ADC   0x0B7


/* Private variables ---------------------------------------------------------*/

/* Buffer used for transmission */
uint8_t aTxBuffer[BUFFER_SIZE] = "UART";
uint8_t TxUser[BUFFER_SIZE] = "UART";

/* Buffer used for reception */
uint8_t aRxBuffer[BUFFER_SIZE];
uint8_t RxUser[BUFFER_SIZE];


uint8_t TxCounter = 0;
uint8_t RxCounter= 0;

//flag for certain task 
uint8_t flag_5ms = 0;
uint8_t flag_1s = 0;
uint8_t flag_send = 0;
uint8_t flag_receive = 0;
uint8_t flag_program = 0;
uint8_t flag_send_flash = 0;
uint8_t toggle = 0;//debugging
//data from 12bit-adc
uint16_t Vout = 0;
uint16_t Vin = 0;
uint16_t Iout = 0;
uint16_t Iin = 0;
uint16_t V_0_10v = 0;

uint16_t PWM_pulse = 0;
//data from flash
uint16_t current_program = 0;
uint16_t current_max = 0;
uint16_t current_min = 0;
//data to program flash
uint16_t current_program_uart = 0;
uint16_t current_max_uart = 0;
uint16_t current_min_uart = 0;
/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/
/**
* @brief  resolve the rx_data
* @param  RxUser: pointer to the rx_data.
* @retval None
*/
void resolve(uint8_t *rx_data)
{
  if(rx_data[0] == 0xf0)
  {
    flag_program = 1;
    current_program_uart = rx_data[1]<<8 |rx_data[2];
    current_max_uart = rx_data[3]<<8 |rx_data[4];
    current_min_uart = rx_data[5]<<8 |rx_data[6];
  }
  else if(rx_data[0] == 0xf1)
  {
    flag_send_flash = 1;
  }
}
void read_status(void)
{
  /* Test DMA1 TC flag */
  while((DMA_GetFlagStatus(DMA1_FLAG_TC1)) == RESET ); 
  /* Clear DMA TC flag */
  DMA_ClearFlag(DMA1_FLAG_TC1);
  Vout = RegularConvData_Tab[0];
  Vin = RegularConvData_Tab[1];
  Iout = RegularConvData_Tab[2];
  Iin = RegularConvData_Tab[3];
  V_0_10v = RegularConvData_Tab[4];
}

void dim_0_10V(void)
{
  if (V_0_10v < MIN_0_10V_ADC)
  {
    PWM_pulse = MIN_PWM;//MIN_PWM/1000
  }
  else if (V_0_10v > MAX_0_10V_ADC){
    PWM_pulse = MAX_PWM;
  }
  else {
    PWM_pulse = (MAX_PWM - MIN_PWM)*(V_0_10v - MIN_0_10V_ADC)/(MAX_0_10V_ADC - MIN_0_10V_ADC) + MIN_PWM;
  }
  if (PWM_pulse * current_program / current_max < MIN_PWM)
  {
    PWM_pulse = MIN_PWM;
  }
  else
  {
    PWM_pulse = PWM_pulse * current_program / current_max;
  }
  PWM_Set(PWM_pulse);
}
void send_data(void)
{
  //set TxUser
  TxUser[0] = 0xe1;
  TxUser[1] = (Vout>>8)&0xff;
  TxUser[2] = Vout & 0xff;
  TxUser[3] = (Vin>>8)&0xff;
  TxUser[4] = Vin & 0xff;
  TxUser[5] = (Iout>>8)&0xff;
  TxUser[6] = Iout & 0xff;
  TxUser[7] = (Iin>>8)&0xff;
  TxUser[8] = Iin & 0xff;
  TxUser[9] = 0xe1;
  memcpy(aTxBuffer,TxUser,10);
  uint8_t *buffptr = aTxBuffer;
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
void send_flash_data()
{
  //set TxUser
  memset(TxUser,0x00,10);
  TxUser[0] = 0xe0;
  TxUser[1] = (current_program>>8)&0xff;
  TxUser[2] = current_program & 0xff;
  TxUser[3] = (current_max>>8)&0xff;
  TxUser[4] = current_max & 0xff;
  TxUser[5] = (current_min>>8)&0xff;
  TxUser[6] = current_min & 0xff;
  TxUser[9] = 0xe0;
  memcpy(aTxBuffer,TxUser,10);
  uint8_t *buffptr = aTxBuffer;
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

void clear_rx_buffer()
{
  //reset RxBuffer
  memset(aRxBuffer, 0x00, BUFFER_SIZE);
  RxCounter = 0;
}

void update_from_flash()
{
  current_program = read_flash(FLASH_USER_ADDR_I_PROG);
  current_max =read_flash(FLASH_USER_ADDR_I_MAX);
  current_min =read_flash(FLASH_USER_ADDR_I_MIN);
}
/**
* @brief  Main program.
* @param  None
* @retval None
*/

int main(void)
{
  // LED_Init();
  TIM_Init();
  PWM_Init();
  ADC1_DMA_Init();
  USART1_Init();
  update_from_flash();
  while(1)
  {
    if(flag_5ms == 1)
    {
      flag_5ms = 0;
      read_status();
      dim_0_10V();
    }
    if (flag_1s == 1)
    {
      flag_1s = 0;
      clear_rx_buffer();
      flag_send = 1;
    }
    if(flag_send == 1)
    {
      flag_send = 0;
      send_data();
    }
    if (flag_send_flash == 1)
    {
      flag_send_flash = 0;
      send_flash_data();
    }
    if(flag_receive == 1)
    {
      flag_receive = 0;
      memcpy(RxUser,aRxBuffer,BUFFER_SIZE);
      resolve(RxUser);
    }
    if (flag_program == 1)
    {
      flag_program = 0;
      erase_write_flash(FLASH_USER_ADDR_I_PROG,current_program_uart);
      erase_write_flash(FLASH_USER_ADDR_I_MAX,current_max_uart);
      erase_write_flash(FLASH_USER_ADDR_I_MIN,current_min_uart);
      update_from_flash();
    }
  }
}

/*****END OF FILE****/
