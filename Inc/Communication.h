/**
 * @file    Communication.c
 * @author  Ferenc Nemeth
 * @date    22 Aug 2018
 * @brief   _write() function for printf() and tracer.
 *
 *          Copyright (c) 2018 Ferenc Nemeth - https://github.com/ferenc-nemeth
 */

#ifndef COMMUNICATION_H_
#define COMMUNICATION_H_

#include "stm32f10x.h"
#include <stdio.h>

int _write(int file, char *ptr, int len);

#endif /* COMMUNICATION_H_ */
