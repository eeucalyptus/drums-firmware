/*
 * Pot.c
 *
 *  Created on: Sep 23, 2022
 *      Author: eike
 */


#include "Pot.h"

#include <libopencm3/stm32/adc.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>

void Pot_Init() {
	rcc_periph_clock_enable(RCC_ADC1);
	adc_power_off(ADC1);
	adc_disable_scan_mode(ADC1);
	adc_set_single_conversion_mode(ADC1);
	adc_disable_external_trigger_regular(ADC1);
	adc_set_right_aligned(ADC1);
	adc_set_sample_time_on_all_channels(ADC1, ADC_SMPR_SMP_28DOT5CYC);
	adc_power_on(ADC1);

	for(int i=0; i<800000; i++) asm volatile("nop");

	adc_reset_calibration(ADC1);
	adc_calibrate(ADC1);
}

uint16_t Pot_Get() {
	uint8_t channel = 8;
	adc_set_regular_sequence(ADC1, 1, &channel);
	adc_start_conversion_direct(ADC1);
	while(!adc_eoc(ADC1));
	uint16_t pot = adc_read_regular(ADC1);

	return pot;
}
