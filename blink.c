// //  minicom -D /dev/tty.usbmodem1101 -b 115200
// /**
//  * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
//  *
//  * SPDX-License-Identifier: BSD-3-Clause
//  */

// #include "pico/stdlib.h"

// // Pico W devices use a GPIO on the WIFI chip for the LED,
// // so when building for Pico W, CYW43_WL_GPIO_LED_PIN will be defined
// #ifdef CYW43_WL_GPIO_LED_PIN
// #include "pico/cyw43_arch.h"
// #endif

// #ifndef LED_DELAY_MS
// #define LED_DELAY_MS 1000
// #endif

// // Perform initialisation
// int pico_led_init(void) {
// #if defined(PICO_DEFAULT_LED_PIN)
//     // A device like Pico that uses a GPIO for the LED will define PICO_DEFAULT_LED_PIN
//     // so we can use normal GPIO functionality to turn the led on and off
//     gpio_init(PICO_DEFAULT_LED_PIN);
//     gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);
//     return PICO_OK;
// #elif defined(CYW43_WL_GPIO_LED_PIN)
//     // For Pico W devices we need to initialise the driver etc
//     return cyw43_arch_init();
// #endif
// }

// // Turn the led on or off
// void pico_set_led(bool led_on) {
// #if defined(PICO_DEFAULT_LED_PIN)
//     // Just set the GPIO on or off
//     gpio_put(PICO_DEFAULT_LED_PIN, led_on);
// #elif defined(CYW43_WL_GPIO_LED_PIN)
//     // Ask the wifi "driver" to set the GPIO on or off
//     cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, led_on);
// #endif
// }

// int main() {
// 	stdio_init_all();  // Initialize USB serial
//     int rc = pico_led_init();
//     hard_assert(rc == PICO_OK);

//     while (true) {
//         pico_set_led(true);
// 		printf("wow\n");
//         sleep_ms(LED_DELAY_MS);
//         pico_set_led(false);
//         sleep_ms(LED_DELAY_MS);
//     }
// }


// The following code works for the following
/* The modified code, picow_blink_button, configures GP20 as an input 
with a pull-up resistor. When the button connected to GP20 is not pressed,
the pin remains in a high state. Pressing the GP20 button will pull the 
pin low, causing the LED to stop blinking (as defined in the code). 
Releasing the button returns the pin to a high state, allowing the LED
to resume blinking.
*/

#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"

#include "hardware/uart.h"
#include "hardware/irq.h"

#define BAUD_RATE 115200
#define DATA_BITS 8
#define STOP_BITS 1
#define PARITY    UART_PARITY_NONE

// Pico W devices use a GPIO on the WIFI chip for the LED,
// so when building for Pico W, CYW43_WL_GPIO_LED_PIN will be defined
#ifdef CYW43_WL_GPIO_LED_PIN
#include "pico/cyw43_arch.h"
#endif

#ifndef LED_DELAY_MS
// #define LED_DELAY_MS 1000
#define LED_DELAY_MS 500
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


//button
void init_button(uint button_pin) {
	gpio_init(button_pin);
    gpio_set_dir(button_pin, GPIO_IN);
    gpio_pull_up(button_pin);
}
bool is_button_pressed(uint button_pin) {
    return gpio_get(button_pin) == 0;  // Button pressed when pin reads 0
}

// uart
// void uart_send(uart_inst_t *uart, char data) {
//     uart_putc(uart, data);
// }
void send_letter(uart_inst_t *uart, char letter) {
    uart_putc(uart, letter);  // Send the letter via UART
	// uart_putc_raw(uart, 'A');
    // printf("Button pressed, sending letter %c\n", letter);
}

char uart_read(uart_inst_t *uart) {
    return uart_getc(uart);
}

const uint BTN_PIN = 20;

#define UART_ID uart1
#define BAUD_RATE 115200
#define TX_PIN 8  // GP8
#define RX_PIN 9  // GP9
#define BUTTON_PIN 22

int main() {
    stdio_init_all();
	// stdio_usb_init();

	int rc = pico_led_init();
    hard_assert(rc == PICO_OK);

	// the next two line is newly added
    // gpio_set_dir(BTN_PIN, GPIO_IN);
    // gpio_set_pulls(BTN_PIN, true, false);

	uart_init(UART_ID, BAUD_RATE);
    gpio_set_function(TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(RX_PIN, GPIO_FUNC_UART);

	// Set the TX and RX pins by using the function select on the GPIO
    // Set datasheet for more information on function select
    // gpio_set_function(TX_PIN, UART_FUNCSEL_NUM(UART_ID, TX_PIN));
    // gpio_set_function(RX_PIN, UART_FUNCSEL_NUM(UART_ID, RX_PIN));


	// init_button(TX_PIN);
	// init_button(RX_PIN);


    // Set UART flow control CTS/RTS, we don't want these, so turn them off
    uart_set_hw_flow(UART_ID, false, false);

    // Set our data format
    uart_set_format(UART_ID, DATA_BITS, STOP_BITS, PARITY);

    // Turn off FIFO's - we want to do this character by character
    uart_set_fifo_enabled(UART_ID, false);

    // Lets send a basic string out, and then run a loop and wait for RX interrupts
    // The handler will count them, but also reflect the incoming data back with a slight change!
    uart_puts(UART_ID, "\nHello, uart interrupts\n");

    while (1)
        tight_loop_contents();


    // if (cyw43_arch_init()) {
    //     printf("Wi-Fi init failed.");
    //     return -1;
    // }
    
	// char letter = 'A';

	// while (true) 
	// {
	// 	// printf("running!!\n");
	// 	sleep_ms(1000);


	//   if (is_button_pressed(BUTTON_PIN))
	//   {
	// 		printf("pressed\n");
	//         // uart_send(UART_ID, letter);
	// 		send_letter(UART_ID, letter);
    //         letter = (letter == 'Z') ? 'A' : letter + 1;
	// 	} 
	// 	else {
    //         // uart_putc(UART_ID, '1');
	// 		uart_putc_raw(UART_ID, '1');

    //     }



	//  	if (uart_is_readable(UART_ID))
	// 	{
	// 		printf("readable!, %d", UART_ID);
    //         // char received = uart_read(UART_ID);
	// 		char received = uart_getc(UART_ID);
    //         if (received >= 'A' && received <= 'Z') 
	// 		{
	// 			printf("received something: %c\n", received);
    //             // uart_send(UART_ID, received + 32);  // Convert to lowercase
    //         } 
	// 		else if (received == '1') 
	// 		{
	// 			printf("received :%d\n", received);
    //         }
    //     }
	// 	else
	// 	{
	// 		printf("not readable, %d | %c\n", UART_ID, UART_ID);
	// 		// printf("not readable, %d\n", UART_ID);
	// 	}

    // }
}
