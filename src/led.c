/*
 * led.c
 *
 *  Created on: Sep 23, 2022
 *      Author: eike
 */


#include "led.h"

#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/rcc.h>

void LED_Init(void) {
	rcc_periph_clock_enable(RCC_GPIOA);
	gpio_set_mode(GPIOA, GPIO_MODE_OUTPUT_50_MHZ, GPIO_CNF_OUTPUT_PUSHPULL,
			GPIO8|GPIO9|GPIO10|GPIO11);
	gpio_set(GPIOA, GPIO8|GPIO9|GPIO10|GPIO11);
}

void LED_Set(int n) {
	int reversed = 0;
	reversed |= (n&0x01)<<3;
	reversed |= (n&0x02)<<1;
	reversed |= (n&0x04)>>1;
	reversed |= (n&0x08)>>3;
	gpio_clear(GPIOA, reversed<<8);
	gpio_set(GPIOA, ~reversed<<8);
}
