/*
 * Buzzer.c
 *
 *  Created on: Sep 23, 2022
 *      Author: eike
 */



#include "Buzzer.h"

#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/rcc.h>

void Buzzer_Init() {
	rcc_periph_clock_enable(RCC_GPIOB);
	gpio_set_mode(GPIOB, GPIO_MODE_OUTPUT_50_MHZ, GPIO_CNF_OUTPUT_PUSHPULL,
			GPIO6);
	gpio_clear(GPIOB, GPIO6);
}

void Buzzer_Set(bool on) {
	if(on) {
		gpio_set(GPIOB, GPIO6);
	}
	else {
		gpio_clear(GPIOB, GPIO6);
	}
}
