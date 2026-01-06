#include "usart.h"

void USART2_Init(void)
{
    /* Enable USART2 and GPIOA clocks */
    RCC->APB1ENR |= RCC_APB1ENR_USART2EN;
    RCC->AHBENR  |= RCC_AHBENR_GPIOAEN;

    /* Configure PA2 as alternate function (AF7 = USART2_TX) */
    GPIOA->MODER &= ~(3U << (2 * 2));
    GPIOA->MODER |=  (2U << (2 * 2));
    GPIOA->AFR[0] &= ~(0xF << (2 * 4));
    GPIOA->AFR[0] |=  (7U << (2 * 4));

    /* Baud rate: 115200 @ 32 MHz */
    USART2->BRR = 0x0116;

    /* Enable transmitter and USART */
    USART2->CR1 = USART_CR1_TE | USART_CR1_UE;
}

void USART2_WriteChar(char c)
{
    while (!(USART2->SR & USART_SR_TXE));  // wait until TX empty
    USART2->DR = c;
}

void USART2_WriteString(const char *s)
{
    while (*s)
    {
        USART2_WriteChar(*s++);
    }
}
