#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>

int main(void) {
	rcc_periph_clock_enable(RCC_GPIOC);

	gpio_set_mode (
		GPIOC,
		GPIO_MODE_OUTPUT_2_MHZ,
		GPIO_CNF_OUTPUT_PUSHPULL,
		GPIO13
	);

	while (1) {
		for (int i = 0; i < 1000000; i++) {
			__asm__("nop");
		}

		gpio_toggle(GPIOC, GPIO13);
	}

	return 0;
}

/*
	1. NOTE : BUILD

	Should work with make in project folder,
	but its better to configure code runner for this purpose

	2. NOTE : FLASHING

	st-flash --reset write STM32_DEV_BOARD.bin 0x8000000   
*/

