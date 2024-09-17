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

#ifndef LED_DELAY_MS
#define LED_DELAY_MS 2000
// #define LED_DELAY_MS 500
#endif

// Perform initialisation
int pico_led_init(void) {
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

// Turn the led on or off
void pico_set_led(bool led_on) {
	#if defined(PICO_DEFAULT_LED_PIN)
		// Just set the GPIO on or off
		gpio_put(PICO_DEFAULT_LED_PIN, led_on);
	#elif defined(CYW43_WL_GPIO_LED_PIN)
		// Ask the wifi "driver" to set the GPIO on or off
		cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, led_on);
	#endif
}

const uint BTN_PIN = 20;

// int main() {
// 	stdio_init_all();  // Initialize USB serial
//     int rc = pico_led_init();
//     hard_assert(rc == PICO_OK);

//     // while (true) {
//     //     pico_set_led(true);
// 	// 	printf("wow\n");
//     //     sleep_ms(LED_DELAY_MS);
//     //     pico_set_led(false);
//     //     sleep_ms(LED_DELAY_MS);
//     // }


//     gpio_set_dir(BTN_PIN, GPIO_IN);
//     gpio_set_pulls(BTN_PIN, true, false);

//     if (cyw43_arch_init()) 
// 	{
//         printf("Wi-Fi init failed.");
//         return -1;
//     }
// 	// 	Initializes the USB serial, onboard LED, button, and Wi-Fi.
// 	// •	Enters an infinite loop where it checks if a button is pressed.
// 	// •	When the button is pressed, it prints “wow” and makes an LED blink on and off every 250 milliseconds.
//     while (true) {
//         if(gpio_get(BTN_PIN))
//         {
// 			printf("wow\n");
//             cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 1);
//             sleep_ms(250);
//             cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 0);
//             sleep_ms(250);
//         }
//     }
// }

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

// void send_data_via_uart(uint8_t data) {
//     // Send the data via UART
//     uart_putc(UART_ID, data);
// }

char receive_data_via_uart()
{
    // Read a character from UART
    if (uart_is_readable(UART_ID))
	{
        return uart_getc(UART_ID);
    }
	else
	{
		printf("not readable\n");
	}
    return 0;
}

void loop() {
    static char letter = 'A';
    static bool btn_state = false;
    uint32_t delay_ms = 1000;

    while (true) {
        // Check button state
        bool is_pressed = gpio_get(BTN_PIN);

        if (is_pressed && !btn_state) 
		{
            btn_state = true;  // Button just pressed
			printf("button pressed\n");
		} else if (!is_pressed && btn_state) 
		{
            btn_state = false;  // Button just released
			// printf("button false\n");
        }

        // Transmit logic based on button state
        if (!is_pressed) 
		{
			// printf("not pressed\n");
    		uart_putc(UART_ID, '1');
		    
			// send_data_via_uart('1');  // Send '1' every second when button is not pressed
        } 
		else 
		{
			printf("send something\n");
            // send_data_via_uart(letter);  // Send current letter
    		uart_putc(UART_ID, letter);

            letter++;  // Move to next letter
            if (letter > 'Z') {
                letter = 'A';  // Loop back to 'A' after 'Z'
            }
        }

        // Receive and process incoming data
        char received_char = receive_data_via_uart();
        if (received_char) {
            if (received_char >= 'A' && received_char <= 'Z')
			{
                // Convert uppercase to lowercase
                char lowercase_char = received_char + ('a' - 'A');
                printf("%c\n", lowercase_char);
            } else if (received_char == '1')
			{
                // If '1' is received, print '2'
                printf("2\n");
            }
        }

        // Delay for 1 second
        sleep_ms(delay_ms);
    }
}

int main() {
	stdio_init_all();  // Initialize USB serial
    int rc = pico_led_init();
    hard_assert(rc == PICO_OK);

 	setup_uart();       // Set up UART1 for communication
    setup_button();     // Set up the button (GP22)
    loop();  // Enter the main loop
    return 0;
}
