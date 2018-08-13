/**
 * @file    UART.c
 * @author  Ferenc Nemeth
 * @date    12 Aug 2018
 * @brief   UART related functions. Initialization, char and string transmission
 *          and __write for printf().
 *
 *          Copyright (c) 2018 Ferenc Nemeth - https://github.com/iamferenc/
 */

#include "UART.h"

/**
 * @brief   Initialization for UART: clock, GPIO, UART, baud rate.
 * @param   baud_rate: The baud rate of the UART.
 * @return  void
 */
void UART_Init(uint32_t baud_rate)
{
  RCC->APB2ENR = RCC_APB2ENR_IOPAEN | RCC_APB2ENR_USART1EN;
  GPIOA->CRH   = GPIO_CRH_CNF9_1 | GPIO_CRH_MODE9_1;
  USART1->CR1  = USART_CR1_TE | USART_CR1_UE;
  USART1->BRR  = (SystemCoreClock / baud_rate);
}

/**
 * @brief   Transmits a char on UART interface.
 * @param   ch: Char to send.
 * @return  void
 */
void UART_SendCh(uint8_t ch)
{
  while(!(USART1->SR & USART_SR_TXE));
  USART1->DR = ch;
}

/**
 * @brief   Transmits an array of chars on UART interface.
 * @param   *ch: Pointer the the array.
 * @param   len: Length of the array.
 * @return  void
 */
void UART_Send(uint8_t *ch, uint16_t len)
{
  for (uint16_t i = 0; i < len; i++)
  {
    UART_SendCh(ch[i]);
  }
}

/**
 * @brief   This is needed by printf() It should be more complex with error report and check,
 *          but I didn't want to overcomplicate it.
 * @param   file
 * @param   *data
 * @param   len
 * @return  int
 */
int _write(int file, char *data, int len)
{
  UART_Send((uint8_t *)data, len);

  return len;
}
