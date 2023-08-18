#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>

static int delay = 600000;

static void clock_setup(void)
{
	/* Set STM32 to 64 MHz. */
	rcc_clock_setup_pll(&rcc_hsi_configs[RCC_CLOCK_HSI_64MHZ]);

	/* Enable alternate function peripheral clock. */
	rcc_periph_clock_enable(RCC_AFIO);

	/* Enable GPIOB clock for led lamps. */
	rcc_periph_clock_enable(RCC_GPIOB);

	/* 
	Enable GPIOB clock for embedded led lamps (user led) 
	rcc_periph_clock_enable(RCC_GPIOC);
	*/
}

static void gpio_setup(void)
{
	/* Configure PB4 as GPIO. */
	AFIO_MAPR |= AFIO_MAPR_SWJ_CFG_FULL_SWJ_NO_JNTRST;
	
	/* Set GPIOs (in GPIO port B) to 'output push-pull'. */
	gpio_set_mode(GPIOB, GPIO_MODE_OUTPUT_50_MHZ, GPIO_CNF_OUTPUT_PUSHPULL, GPIO4 | GPIO5 | GPIO6 | GPIO7 | GPIO8);

	/* 
	Set GPIO13 to 'output push-pull' for embedded led. 
	gpio_set_mode(GPIOC, GPIO_MODE_OUTPUT_2_MHZ, GPIO_CNF_OUTPUT_PUSHPULL, GPIO13);
	*/
}

static void cpu_pause(int timeout)
{
	for (int i = 0; i < timeout; i++) {
		__asm__("nop");
	}
}


int main(void)
{
	clock_setup();
	gpio_setup();

	gpio_toggle(GPIOB, GPIO4);

	while (1) {
		gpio_toggle(GPIOB, GPIO4);

		cpu_pause(delay);

		//		gpio_toggle(GPIOB, GPIO4);

		gpio_toggle(GPIOB, GPIO5);

		cpu_pause(delay);

		//		gpio_toggle(GPIOB, GPIO5);


		gpio_toggle(GPIOB, GPIO6);

		cpu_pause(delay);

		//		gpio_toggle(GPIOB, GPIO6);


		gpio_toggle(GPIOB, GPIO7);

		cpu_pause(delay);

		//		gpio_toggle(GPIOB, GPIO7);


		gpio_toggle(GPIOB, GPIO8);

		cpu_pause(delay);

		//		gpio_toggle(GPIOB, GPIO8);

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

