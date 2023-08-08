#include <libopencm3/stm32/rcc.h>
#include <libopencm3/cm3/nvic.h>
#include <libopencm3/stm32/exti.h>
#include <libopencm3/stm32/f1/gpio.h>
#include <libopencm3/stm32/i2c.h>
#include <ssd1306_i2c.h>

void clock_setup(void) {
  rcc_clock_setup_in_hse_12mhz_out_72mhz();

  /* Enable GPIOs clock. */
  rcc_periph_clock_enable(RCC_GPIOA);
  rcc_periph_clock_enable(RCC_GPIOB);

  /* set clock for I2C */
  rcc_periph_clock_enable(RCC_I2C2);

  /* set clock for AFIO*/
  rcc_periph_clock_enable(RCC_AFIO);

  AFIO_MAPR |= AFIO_MAPR_SWJ_CFG_FULL_SWJ_NO_JNTRST;
}

static void i2c_setup(void) {
  /* Set alternate functions for the SCL and SDA pins of I2C2. */
  gpio_set_mode(GPIOB, GPIO_MODE_OUTPUT_50_MHZ,
                GPIO_CNF_OUTPUT_ALTFN_OPENDRAIN,
                GPIO_I2C2_SCL | GPIO_I2C2_SDA);

  /* Disable the I2C before changing any configuration. */
  i2c_peripheral_disable(I2C2);

  /* APB1 is running at 36MHz. */
  i2c_set_clock_frequency(I2C2, I2C_CR2_FREQ_36MHZ);

  /* 400KHz - I2C Fast Mode */
  i2c_set_fast_mode(I2C2);

  /*
	 * fclock for I2C is 36MHz APB2 -> cycle time 28ns, low time at 400kHz
	 * incl trise -> Thigh = 1600ns; CCR = tlow/tcycle = 0x1C,9;
	 * Datasheet suggests 0x1e.
	 */
  i2c_set_ccr(I2C2, 0x1e);

  /*
   * fclock for I2C is 36MHz -> cycle time 28ns, rise time for
   * 400kHz => 300ns and 100kHz => 1000ns; 300ns/28ns = 10;
   * Incremented by 1 -> 11.
   */
  i2c_set_trise(I2C2, 0x0b);

  /*
   * Enable ACK on I2C
   */
  i2c_enable_ack(I2C2);

  /*
   * This is our slave address - needed only if we want to receive from
   * other masters.
   */
  i2c_set_own_7bit_slave_address(I2C2, 0x32);

  /* If everything is configured -> enable the peripheral. */
  i2c_peripheral_enable(I2C2);
}

volatile int8_t step = 0;

void exti9_5_isr(void) {
  if (!gpio_get(GPIOA, GPIO8) && gpio_get(GPIOA, GPIO9))
      step += 1;
  else
      step -= 1;
  exti_reset_request(EXTI8); // we should clear flag manually
}

void board_setup(void) {
  // Debug setting for rotary encoder EC11 on (PA8, PA9) for make simple command
  gpio_set_mode(GPIOA, GPIO_MODE_INPUT,
                GPIO_CNF_INPUT_FLOAT,
                GPIO8 | GPIO9);

  nvic_enable_irq(NVIC_EXTI9_5_IRQ);
  exti_enable_request(EXTI8);
  exti_set_trigger(EXTI8, EXTI_TRIGGER_FALLING);
  exti_select_source(EXTI8, GPIOA);
}

int main(void) {
  /**
   * Brief delay to give the debugger a chance to stop the core before we
   * muck around with the chip's configuration.
   */
  for (uint32_t loop = 0; loop < 1000000; ++loop) {
    __asm__("nop");
  }

  clock_setup();
  i2c_setup();
  board_setup();

  ssd1306_init(I2C2, DEFAULT_7bit_OLED_SLAVE_ADDRESS, 128, 32);

  step = 1;
  int16_t y = 0;

  while (1) {
      if (step!=0) {
        for (int i =0; i<8; i++) {
          y += step;
          ssd1306_clear();
          ssd1306_drawWCharStr(0, y, white, wrapDisplay, L"Это текст с цифрами 01234567890 и " \
          "cимволами .!№;%:?*()), ну че, watsup my nigga??!.\n"  \
          "There is a lot text with ENGLISH or latin symbols!\n\n"\
          "Кажется у меня получилось отрисовать это дерьмо\n\n "
          );
          ssd1306_refresh();
          for (uint32_t loop = 0; loop < 1000000; ++loop) {
            __asm__("nop");
        }
      }
      if (step<0)
        step += 1;
      else
        step -= 1;
    }
  }

  /* We will never get here */
  return 0;
}
