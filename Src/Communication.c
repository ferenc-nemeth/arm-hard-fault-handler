/**
 * @file    Communication.c
 * @author  Ferenc Nemeth
 * @date    22 Aug 2018
 * @brief   _write() function for printf() and tracer.
 *
 *          Copyright (c) 2018 Ferenc Nemeth - https://github.com/ferenc-nemeth
 */

#include "Communication.h"

/**
 * @brief   This is needed by printf().
 * @param   file
 * @param   *data
 * @param   len
 * @return  len
 */
int _write(int file, char *data, int len)
{
  for (uint16_t i = 0u; i < len; i++)
  {
    ITM_SendChar(data[i]);
  }

  return len;
}
