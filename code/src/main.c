/*
 * STM32 L152RE
 * ADC -> MAX187 -> STM32 (SPI protocol)
 * - 1–5 V → –55.0°C to 150.0°C
 * - Display error if voltage < 1V
 */

#include "stm32l1xx.h"
#include "nucleo152start.h"
#include "core_cm3.h"
#include <stdio.h>
#include <stdint.h>

#include "usart.h"
#include "timer.h"
#include "spi.h"

uint16_t read_ADC_median(void);

/* ===== Main ===== */
int main(void) {
	SetSysClock();
	SystemCoreClockUpdate();

	Timer3_Init();
	SPI1_Init();
	USART2_Init();

	USART2_WriteString("MAX187 Temp Reader Ready\r\n");

	while (1) {
	    uint16_t adc_val = read_ADC_median();

	    /* ===== Convert ADC to mV =====
	     * VREF = 4096 mV, 12-bit ADC (0–4095)
	     * voltage_mV = (adc_val * 4096) / 4095
	     */
	    int voltage_mV = (adc_val * 4096) / 4095;

	    char buf[32];

	    /* ===== Voltage below 0.8V → ERROR ===== */
	    if (voltage_mV < 750) { // set 750mV to keep reading minimum value
	        sprintf(buf, "ADC:%d ERR <0.8V   ", adc_val);

	        USART2_WriteString(buf);
	        USART2_WriteString("\r\n");

	        delay_ms(1000);
	        continue;
	    }

	    /* ===== Convert mV → Temperature (–40.0°C to +60.0°C)
	     * Linear map 0.8–4.0 V → –40.0°C to +60.0°C
	     *
	     * Span: 3200 mV → 1000 tenths of °C
	     * temp_x10 = ((voltage_mV - 800) * 1000) / 3200 - 400
	     */
	    int voltage_over_mV = voltage_mV - 800;   // mV over 0.8V

	    int temp_x10 = (voltage_over_mV * 1000) / 3200 - 400;

	    int temp_whole = temp_x10 / 10;
	    int temp_frac = temp_x10 % 10;
	    if (temp_frac < 0)
	        temp_frac = -temp_frac;

	    sprintf(buf, "ADC:%d %d.%dC   ", adc_val, temp_whole, temp_frac);

	    USART2_WriteString(buf);
	    USART2_WriteString("\r\n");

	    delay_ms(1000);
	}
}

/* bubble-style sort for 5 elements */
static void bubbleShort5(uint16_t a[5]) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4 - i; j++) {
            if (a[j] > a[j+1]) {
                uint16_t tmp = a[j];
                a[j] = a[j+1];
                a[j+1] = tmp;
            }
        }
    }
}

/* read 5 samples at 50 ms intervals, take median, print temperature */
uint16_t read_ADC_median(void) {
    uint16_t samples[5];

    /* 1) Collect five raw ADC readings */
    for (int i = 0; i < 5; i++) {
        samples[i] = MAX187_Read();
        delay_ms(50);
    }

    /* 2) Sort and pick median */
    bubbleShort5(samples);
    uint16_t med_adc = samples[2];

    return med_adc;
}
