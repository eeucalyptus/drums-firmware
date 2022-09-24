#include "Keypad.h"
#include "led.h"
#include "AudioPWM.h"
#include "Buzzer.h"

#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/rcc.h>


// PB6 Buzzer
// PB7 Speaker
// PA0-7 numpad
// PA8-11 LEDs
// PB0 Pot



int main() {
	int phase = 0;
	int beat = 0;
	int lastKey = -1;
	int bpm = 0;
	int delay = 0;
	int pot = 0;
	int ms = 0;
	uint16_t beats = 0;
	rcc_clock_setup_in_hse_8mhz_out_72mhz();
	systick_set_reload((uint32_t)-1);
	systick_counter_enable();

	LED_Init();
	Keypad_Init();
	AudioPWM_Init();
	Pot_Init();
	Buzzer_Init();

	while(1) {
		int key = Keypad_GetKey();
		if (key != lastKey) {
			lastKey = key;
			if (key>=0) {
				beats ^= 1 << key;
			}
		}

		// Get Speed
		pot = Pot_Get();
		bpm = (pot*(180-60))/4096+60;
		delay = (60000000/bpm)/4; // delay in us, 4 sub-beats

		if (phase == 0 && 1000*ms > delay/2) {
			LED_Set(1 << (beat/4));
			if ((beats & (1<<beat)) != 0) {
				Buzzer_Set(true);
			}
			phase = 1;
			ms = 0;
		}
		else if (phase == 1 && 1000*ms > delay/2) {
			LED_Set(0);
			Buzzer_Set(false);
			if(++beat >= 16) beat = 0;

			phase = 0;
			ms = 0;
		}
		else {
			ms++;
			usleep(1000);
		}
	}
}
