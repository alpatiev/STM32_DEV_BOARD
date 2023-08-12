#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/usart.h>
#include <string.h>

#define LED_PORT GPIOC
#define LED_PIN GPIO13
#define RX_BUFFER_SIZE 64

char rx_buffer[RX_BUFFER_SIZE];
char tx_buffer[RX_BUFFER_SIZE];

void usart1_setup(void);
void toggle_red_light(bool flag);
void send_command(const char* command);

void usart1_setup(void) {
    rcc_periph_clock_enable(RCC_GPIOA);
    rcc_periph_clock_enable(RCC_USART1);

    gpio_set_mode(GPIOA, 
                  GPIO_MODE_OUTPUT_50_MHZ,
                  GPIO_CNF_OUTPUT_ALTFN_PUSHPULL,
                  GPIO_USART1_TX);
    gpio_set_mode(GPIOA,
                  GPIO_MODE_INPUT,
                  GPIO_CNF_INPUT_FLOAT,
                  GPIO_USART1_RX);

    usart_set_baudrate(USART1, 9600);
    usart_set_databits(USART1, 8);
    usart_set_stopbits(USART1, USART_STOPBITS_1);

    usart_set_mode(USART1, USART_MODE_TX_RX);
    usart_set_parity(USART1, USART_PARITY_NONE);
    usart_set_flow_control(USART1, USART_FLOWCONTROL_NONE);

    usart_enable(USART1);
}

void toggle_red_light(bool flag) {
    if (flag == true) {
        gpio_clear(LED_PORT, LED_PIN); 
    } else {
        gpio_set(LED_PORT, LED_PIN); 
    }
}

void send_command(const char* command) {
    memset(rx_buffer, 0, RX_BUFFER_SIZE);
    memset(tx_buffer, 0, RX_BUFFER_SIZE);

    sprintf(tx_buffer, "%s\r\n", command);
    int command_length = strlen(tx_buffer);

    for (int i = 0; i < command_length; i++) {
        usart_send_blocking(USART1, tx_buffer[i]);
    }

    int rx_index = 0;

    if (usart_get_flag(USART1, USART_FLAG_RXNE)) {
        char received_char = usart_recv(USART1);
		toggle_red_light(true);
		for (int i = 0; i < 100000; i++) {
            __asm__("nop");
    } 
}

int main(void) {
    rcc_periph_clock_enable(RCC_GPIOC);

    gpio_set_mode(LED_PORT, 
                  GPIO_MODE_OUTPUT_2_MHZ,
                  GPIO_CNF_OUTPUT_PUSHPULL, 
                  LED_PIN);

    for (int i = 0; i < 1000000; i++) {
        __asm__("nop");
    }

    toggle_red_light(false);

    for (int i = 0; i < 1000000; i++) {
        __asm__("nop");
    }

    toggle_red_light(true);

    for (int i = 0; i < 1000000; i++) {
        __asm__("nop");
    }

    toggle_red_light(false);

    usart1_setup();

    while (1) {
        send_command("AT");

		for (int i = 0; i < 1000000; i++) {
            __asm__("nop");
        }

		toggle_red_light(false);
    }

    return 0;
}

