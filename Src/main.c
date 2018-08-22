/**
 * @file    main.c
 * @author  Ferenc Nemeth
 * @date    12 Aug 2018
 * @brief   Main function with a demo application.
 *          -Try to write some data to a place where we shouldn't.
 *          -The system throws a Hard fault.
 *
 *          Copyright (c) 2018 Ferenc Nemeth - https://github.com/ferenc-nemeth
 */

#include "stm32f10x.h"
#include "Communication.h"

/* Main function. */
int main(void)
{
  /* Create a hard fault with writing data to the 'External Device' part (0xA0000000-0xDFFFFFFF). */
  (*((volatile uint32_t *)(0xAAAAAAAA))) = 0x1234567;

  /* Never going to reach it, because of Hard fault. */
  while(1);

  return 0;
}
