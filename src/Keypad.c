/*
 * Keypad.c
 *
 *  Created on: 19 Sep 2019
 *      Author: schwarzwald
 */

#include "Keypad.h"

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>

#include <stdbool.h>
#include <stdint.h>
#include <unistd.h>

typedef struct {
	uint32_t Pin;
	uint32_t Port;
} Keypad_Gpio_t;

/**
 * Pin definitions for the keypad row pins // TODO Set correct pins, add RCC calculation
 */
Keypad_Gpio_t Keypad_RowPins[4] = {
		{.Port = GPIOA, .Pin = 0},
		{.Port = GPIOA, .Pin = 1},
		{.Port = GPIOA, .Pin = 2},
		{.Port = GPIOA, .Pin = 3},
};

/**
 * Pin definitions for the keypad column pins
 */
Keypad_Gpio_t Keypad_ColPins[4] = {
		{.Port = GPIOA, .Pin = 4},
		{.Port = GPIOA, .Pin = 5},
		{.Port = GPIOA, .Pin = 6},
		{.Port = GPIOA, .Pin = 7},
};


/**
 * Currently selected row
 */
static int Keypad_ActiveCol = -1;

/**
 * Select one keypad row by pulling it low while pulling up all others
 *
 * @param row Row to be selected (1 to 4)
 */
void Keypad_SetActiveCol(int col) {
	// Save new active row
	Keypad_ActiveCol = col;

	// Set all col pins as high
	for(int i = 0; i < 4; i++) {
		gpio_set(Keypad_ColPins[i].Port, 1 << Keypad_ColPins[i].Pin);
	}
	// Set active col low
	gpio_clear(Keypad_ColPins[Keypad_ActiveCol].Port, 1 << Keypad_ColPins[Keypad_ActiveCol].Pin);
}

/**
 * All cols low to quickly detect the presence of a key press
 */
void Keypad_SetAllColsActive() {
	Keypad_ActiveCol = -1;

	// Set all col pins low
	for(int i = 0; i < 4; i++) {
		gpio_clear(Keypad_ColPins[i].Port, 1 << Keypad_ColPins[i].Pin);
	}
}


void Keypad_Init(void) {
	// Enable GPIO Peripheral clock
	rcc_periph_clock_enable(RCC_GPIOA);


	// Set all row pins as pull-up
	for(int i = 0; i < 4; i++) {
		uint32_t port = Keypad_RowPins[i].Port;
		uint32_t pin = 1 << Keypad_RowPins[i].Pin;
		gpio_set_mode(port, GPIO_MODE_INPUT, GPIO_CNF_INPUT_PULL_UPDOWN, pin);
		gpio_set(port, pin);
	}
	// Set all col pins low
	for(int i = 0; i < 4; i++) {
		uint32_t port = Keypad_ColPins[i].Port;
		uint32_t pin = 1 << Keypad_ColPins[i].Pin;
		gpio_clear(port, pin);
		gpio_set_mode(port, GPIO_MODE_OUTPUT_50_MHZ, GPIO_CNF_OUTPUT_PUSHPULL,
				pin);
	}

	Keypad_SetAllColsActive();
}

int Keypad_GetKey() {
	int key = -1;

	// Check if any key is pressed
	int pressed_row = -1;
	for(int row = 0; row < 4; row++) {
		uint32_t port = Keypad_RowPins[row].Port;
		uint32_t pin = 1 << Keypad_RowPins[row].Pin;

		uint32_t in = gpio_get(port, pin);
		if(in == 0) {
			pressed_row = row;
			break;
		}
	}

	int pressed_col = -1;
	if(pressed_row >= 0) {
		// switch rows and cols

		// Set all col pins as pull-up
		for(int i = 0; i < 4; i++) {
			uint32_t port = Keypad_ColPins[i].Port;
			uint32_t pin = 1 << Keypad_ColPins[i].Pin;
			gpio_set_mode(port, GPIO_MODE_INPUT, GPIO_CNF_INPUT_PULL_UPDOWN, pin);
			gpio_set(port, pin);
		}
		// Set all row pins low
		for(int i = 0; i < 4; i++) {
			uint32_t port = Keypad_RowPins[i].Port;
			uint32_t pin = 1 << Keypad_RowPins[i].Pin;
			gpio_set_mode(port, GPIO_MODE_OUTPUT_50_MHZ, GPIO_CNF_OUTPUT_PUSHPULL,
					pin);
			gpio_clear(port, pin);
		}

		for(int col = 0; col <= 3; col++) {
			uint32_t port = Keypad_ColPins[col].Port;
			uint32_t pin = 1 << Keypad_ColPins[col].Pin;

			uint32_t in = gpio_get(port, pin);
			if(in == 0) {
				pressed_col = col;
				key = (3-pressed_row)*4+(3-col);
				break;
			}
		}
	}

	// Reset all pins
	Keypad_Init();

	return key;
}


void Keypad_EnterInterruptMode(void) {
	// TODO Implement
}

void Keypad_ExitInterruptMode(void) {
	// TODO Implement
}
