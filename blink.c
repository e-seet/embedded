//  minicom -D /dev/tty.usbmodem1101 -b 115200
/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "pico/stdlib.h"

// Pico W devices use a GPIO on the WIFI chip for the LED,
// so when building for Pico W, CYW43_WL_GPIO_LED_PIN will be defined
#ifdef CYW43_WL_GPIO_LED_PIN
#include "pico/cyw43_arch.h"
#endif

// Perform initialisation
int pico_led_init(void) 
{
	#if defined(PICO_DEFAULT_LED_PIN)
		// A device like Pico that uses a GPIO for the LED will define PICO_DEFAULT_LED_PIN
		// so we can use normal GPIO functionality to turn the led on and off
		gpio_init(PICO_DEFAULT_LED_PIN);
		gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);
		return PICO_OK;
	#elif defined(CYW43_WL_GPIO_LED_PIN)
		// For Pico W devices we need to initialise the driver etc
		return cyw43_arch_init();
	#endif
}

// next exercise: on UART communiactions
#define UART_ID uart1
#define BAUD_RATE 115200

#define UART_TX_PIN 8  // GP8 (TX)
#define UART_RX_PIN 9  // GP9 (RX)
#define BTN_PIN 22     // GP22 (Button)

void setup_uart() 
{
    // Initialize UART1 at the desired baud rate
    uart_init(UART_ID, BAUD_RATE);
    
    // Set GPIO pins for UART1 TX (GP8) and RX (GP9)
    gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);
}

void setup_button() 
{
    // Set GP22 as input with pull-down
    gpio_init(BTN_PIN);
    gpio_set_dir(BTN_PIN, GPIO_IN);
    gpio_pull_down(BTN_PIN);
}

void send_data_via_uart(uint8_t data)
{
    // Send the data via UART
    uart_putc(UART_ID, data);
}

char receive_data_via_uart()
{
    // Read a character from UART
    if (uart_is_readable(UART_ID))
        return uart_getc(UART_ID);
	else
		printf("not readable\n");
    return 0;
}

void loop() {
    static char letter = 'A';
    static bool btn_state = false;
    uint32_t delay_ms = 1000;

    while (true)
	{
        // Check button state
        bool is_pressed = gpio_get(BTN_PIN);

        if (is_pressed && !btn_state) 
            btn_state = true;  // Button just pressed
		else if (!is_pressed && btn_state) 
            btn_state = false;  // Button just released

        // Transmit logic based on button state
        if (is_pressed) 
		{
    		// uart_putc(UART_ID, '1');
			send_data_via_uart('1');  // Send '1' every second when button is not pressed
        } 
		else 
		{
            send_data_via_uart(letter);  // Send current letter
    		// uart_putc(UART_ID, letter);

            letter++;  // Move to next letter
            if (letter > 'Z') {
                letter = 'A';  // Loop back to 'A' after 'Z'
            }
        }

        // Receive and process incoming data
        char received_char = receive_data_via_uart();
        if (received_char) 
		{
            if (received_char >= 'A' && received_char <= 'Z')
			{
                // Convert uppercase to lowercase
                char lowercase_char = received_char + ('a' - 'A');
                printf("%c\n", lowercase_char);
            } 
			else if (received_char == '1')
			{
				printf("2\n");
            }
        }
        // Delay for 1 second
        sleep_ms(delay_ms);
    }
}

// Send a character 'A' via UART every 2 sec
void loopback_test()
{
    while (true)
	{
        uart_putc(UART_ID, 'A');
        sleep_ms(100);
        if (uart_is_readable(UART_ID)) 
		{
            char received = uart_getc(UART_ID);
            printf("Received: %c\n", received);
        } 
		else 
            printf("No data received!\n");
        sleep_ms(1000);
    }
}

int main() {
	stdio_init_all();  // Initialize USB serial
    int rc = pico_led_init();
    hard_assert(rc == PICO_OK);

	//testing loopback
	// This sends and check if a is received.
    // setup_uart();       // Set up UART1 for communication
    // loopback_test();    // Enter the loopback test function
	// end of testing

 	setup_uart();       // Set up UART1 for communication
    setup_button();     // Set up the button (GP22)
    loop();  // Enter the main loop
    return 0;
}
