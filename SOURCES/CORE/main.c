#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>

static int delay = 5000000;
static int gpio_selected = 0;


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

static void gpio_display_digit(int digit)
{
	//   G  -  F  -  E  -  D  -  C  -  B  -  A 
	// GPIO0 GPIO1 GPIO9 GPIO8 GPIO7 GPIO5 GPIO4
 	switch (digit)
	{
	case (0): // 0111111	
		gpio_clear(GPIOB, GPIO0);
		gpio_set(GPIOB, GPIO1 | GPIO9 | GPIO8 | GPIO7 | GPIO5 |GPIO4);
		break;
	case (1): // 0000110
		gpio_clear(GPIOB, GPIO0 | GPIO1 | GPIO9 | GPIO8 | GPIO4);
		gpio_set(GPIOB, GPIO7 | GPIO5);
		break;
	case (2): // 1011011
		gpio_clear(GPIOB, GPIO1 | GPIO7);
		gpio_set(GPIOB, GPIO0 | GPIO9 | GPIO8 | GPIO5 | GPIO4);
		break;
	case (3): // 1001111
		gpio_clear(GPIOB, GPIO1 | GPIO9);
		gpio_set(GPIOB, GPIO0 | GPIO8 | GPIO7 | GPIO5 |GPIO4);	
		break;
	case (4): // 1100110
		gpio_clear(GPIOB, GPIO9 | GPIO8 | GPIO4);
		gpio_set(GPIOB, GPIO0 | GPIO1 | GPIO7 | GPIO5);
		break;
	case (5): // 1101101
		gpio_clear(GPIOB, GPIO9 | GPIO5);
		gpio_set(GPIOB, GPIO0 | GPIO1 | GPIO8 | GPIO7 |GPIO4);
		break;
	case (6): // 1111101
		gpio_clear(GPIOB, GPIO5);
		gpio_set(GPIOB, GPIO0 | GPIO1 | GPIO9 | GPIO8 | GPIO7 |GPIO4);	
		break;
	case (7): // 0000111
		gpio_clear(GPIOB, GPIO0 | GPIO1 | GPIO9 | GPIO8);
		gpio_set(GPIOB, GPIO7 | GPIO5 |GPIO4);
		break;
	case (8): // 1111111
		gpio_set(GPIOB, GPIO0 | GPIO1 | GPIO9 | GPIO8 | GPIO7 | GPIO5 |GPIO4);	
		break;
	case (9): // 1101111
		gpio_clear(GPIOB, GPIO9);
		gpio_set(GPIOB, GPIO0 | GPIO1 | GPIO8 | GPIO7 | GPIO5 |GPIO4);	
		break;
	default:
		break;
	}
}

static void gpio_led_off(void)
{
	gpio_clear(GPIOB, GPIO0 | GPIO1 | GPIO4 | GPIO5 | GPIO6 | GPIO7 |GPIO8 | GPIO9);
}

int main(void)
{
	clock_setup();
	gpio_setup();
	gpio_led_off();

	int number = 0;

	while (1) {
		gpio_display_digit(number);

		if (number > 9) {
			number = 0;
		} else {
			number++;
		}

		cpu_pause(delay);
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
