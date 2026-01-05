#ifndef USART_H
#define USART_H

#include <stdint.h>
#include "stm32l1xx.h"

/* Initialize USART2 (PA2 = TX) at 115200 baud, 32 MHz clock */
void USART2_Init(void);

/* Write a null-terminated string over USART2 */
void USART2_WriteString(const char *s);

/* Write a single character */
void USART2_WriteChar(char c);

#endif /* USART_H */
