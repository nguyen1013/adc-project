/*
 * timer.c
 *
 *  Created on: 27 thg 11, 2025
 *      Author: nguyen
 */

#include "stm32l1xx.h"
#include "timer.h"

void Timer3_Init(void) {
	RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;
	TIM3->PSC = 32 - 1; /* 1 MHz */
	TIM3->ARR = 1000 - 1; /* 1 ms */
	TIM3->CNT = 0;
}

void delay_ms(int ms) {
	TIM3->CR1 = TIM_CR1_CEN;
	for (int i = 0; i < ms; i++) {
		while (!(TIM3->SR & TIM_SR_UIF))
			;
		TIM3->SR &= ~TIM_SR_UIF;
	}
	TIM3->CR1 = 0;
}

void delay_10us(void) {
	unsigned long i = 0;
	RCC->APB1ENR |= RCC_APB1ENR_TIM6EN;
	TIM6->PSC = 0;
	TIM6->ARR = 32 - 1;
	TIM6->CNT = 0;
	TIM6->CR1 = TIM_CR1_CEN;

	while (i < 5) {
		while (!(TIM6->SR & TIM_SR_UIF)) {
		}
		TIM6->SR &= ~TIM_SR_UIF;
		TIM6->CNT = 0;
		i++;
	}
	TIM6->CR1 = 0;
}
