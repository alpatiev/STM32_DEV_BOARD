#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>

static int delay = 3000000;

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
	gpio_set_mode(GPIOB, 
					GPIO_MODE_OUTPUT_50_MHZ, 
					GPIO_CNF_OUTPUT_PUSHPULL,
					GPIO0 | GPIO1 | GPIO4 | GPIO5 | GPIO6 | GPIO7 |GPIO8 | GPIO9);

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

static void gpio_set_display(char code, bool flag) 
{
	int gpio_selected;
	switch (code)
	{
	case 'e':
		gpio_selected = GPIO9;
		break;
	case 'd':
		gpio_selected = GPIO8;
		break;
	case 'c':
		gpio_selected = GPIO7;
		break;
	case 'g':
		gpio_selected = GPIO0;
		break;
	case 'b':
		gpio_selected = GPIO5;
		break;
	case 'a':
		gpio_selected = GPIO4;
		break;
	case 'f':
		gpio_selected = GPIO1;
		break;
	case '.':
		gpio_selected = GPIO6;
		break;	
	default:
		return;
	}

	gpio_set(gpio_selected, flag);
	cpu_pause(delay);
}

int main(void)
{
	clock_setup();
	gpio_setup();


	while (1) {
		gpio_set_display('e', true);
		gpio_set_display('d', true);
		gpio_set_display('c', true);
		gpio_set_display('g', true);
		gpio_set_display('b', true);
		gpio_set_display('a', true);
		gpio_set_display('f', true);
		gpio_set_display('.', true);

		gpio_set_display('e', false);
		gpio_set_display('d', false);
		gpio_set_display('c', false);
		gpio_set_display('g', false);
		gpio_set_display('b', false);
		gpio_set_display('a', false);
		gpio_set_display('f', false);
		gpio_set_display('.', false);
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

