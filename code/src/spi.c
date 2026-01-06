/*
 * spi.c
 *
 *  Created on: 5 thg 1, 2026
 *      Author: nguyen
 */

#include "spi.h"
#include "stm32l1xx.h"
#include "timer.h"

/* ===== SPI1 Init ===== */
void SPI1_Init(void) {
    /* Enable clocks for SPI1 and GPIOA */
    RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;
    RCC->AHBENR  |= RCC_AHBENR_GPIOAEN;

    /* -------- Configure PA4 (CS), PA5 (SCK), PA6 (MISO), PA7 (MOSI) -------- */

    /* --- CS (PA4) as GPIO output --- */
    GPIOA->MODER &= ~GPIO_MODER_MODER4;
    GPIOA->MODER |= GPIO_MODER_MODER4_0;
    GPIOA->OTYPER &= ~GPIO_OTYPER_OT_4;
    GPIOA->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR4;
    GPIOA->BSRR = GPIO_BSRR_BS_4;           // CS high

    /* --- SCK (PA5) as AF5 --- */
    GPIOA->MODER &= ~GPIO_MODER_MODER5;
    GPIOA->MODER |= GPIO_MODER_MODER5_1;
    GPIOA->AFR[0] &= ~(0xF << (5 * 4));
    GPIOA->AFR[0] |=  (0x05 << (5 * 4));

    /* --- MISO (PA6) as AF5 --- */
    GPIOA->MODER &= ~GPIO_MODER_MODER6;
    GPIOA->MODER |= GPIO_MODER_MODER6_1;
    GPIOA->AFR[0] &= ~(0xF << (6 * 4));
    GPIOA->AFR[0] |=  (0x05 << (6 * 4));

    /* --- MOSI (PA7) as AF5 --- */
    GPIOA->MODER &= ~GPIO_MODER_MODER7;
    GPIOA->MODER |= GPIO_MODER_MODER7_1;
    GPIOA->AFR[0] &= ~(0xF << (7 * 4));
    GPIOA->AFR[0] |=  (0x05 << (7 * 4));
    GPIOA->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR7;
    GPIOA->OTYPER &= ~GPIO_OTYPER_OT_7;

    /* -------- Configure SPI1 peripheral -------- */
    SPI1->CR1 = 0;
    SPI1->CR1 |= SPI_CR1_MSTR;
    SPI1->CR1 |= SPI_CR1_SSM | SPI_CR1_SSI;
    SPI1->CR1 &= ~(SPI_CR1_CPOL | SPI_CR1_CPHA);  // Mode 0
    SPI1->CR1 &= ~SPI_CR1_DFF;                   // 8-bit
    SPI1->CR1 |= SPI_CR1_BR_2;                   // PCLK/32
    SPI1->CR1 |= SPI_CR1_SPE;

    /* Clear RX buffer */
    if (SPI1->SR & SPI_SR_RXNE)
        (void)SPI1->DR;
}

/* ===== MAX187 Read ===== */
uint16_t MAX187_Read(void) {
    uint8_t rx[2];

    /* ---- Shift out old data, CS high starts conversion ---- */
    GPIOA->BSRR = GPIO_BSRR_BR_4;   // CS LOW

    for (int i = 0; i < 2; i++) {
        while (!(SPI1->SR & SPI_SR_TXE));
        SPI1->DR = 0x00;
        while (!(SPI1->SR & SPI_SR_RXNE));
        rx[i] = (uint8_t)SPI1->DR;
    }

    while (SPI1->SR & SPI_SR_BSY);
    GPIOA->BSRR = GPIO_BSRR_BS_4;   // CS HIGH → start conversion

    delay_10us();                  // >= 8.5 µs conversion time

    /* ---- Read new conversion ---- */
    GPIOA->BSRR = GPIO_BSRR_BR_4;

    for (int i = 0; i < 2; i++) {
        while (!(SPI1->SR & SPI_SR_TXE));
        SPI1->DR = 0x00;
        while (!(SPI1->SR & SPI_SR_RXNE));
        rx[i] = (uint8_t)SPI1->DR;
    }

    while (SPI1->SR & SPI_SR_BSY);
    GPIOA->BSRR = GPIO_BSRR_BS_4;

    uint16_t raw = ((uint16_t)rx[0] << 8) | rx[1];
    return (raw >> 3) & 0x0FFF;    // remove marker + trailing zeros
}


