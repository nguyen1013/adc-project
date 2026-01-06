/*
 * spi.h
 *
 *  Created on: 5 thg 1, 2026
 *      Author: nguyen
 */

#ifndef SPI_H
#define SPI_H

#include <stdint.h>

/* ===== Public SPI API ===== */
void SPI1_Init(void);
uint16_t MAX187_Read(void);

#endif /* SPI_H */

