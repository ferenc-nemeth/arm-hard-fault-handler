/**
 * @file    UART.h
 * @author  Ferenc Nemeth
 * @date    12 Aug 2018
 * @brief   UART related functions. Initialization, char and string transmission
 *          and __write for printf().
 *
 *          Copyright (c) 2018 Ferenc Nemeth - https://github.com/ferenc-nemeth
 */

#ifndef UART_H_
#define UART_H_

#include "stm32f10x.h"
#include <stdio.h>

void UART_Init(uint32_t baud_rate);
void UART_SendCh(uint8_t ch);
void UART_Send(uint8_t *ch, uint16_t len);
int _write(int file, char *data, int len);

#endif /* UART_H_ */
