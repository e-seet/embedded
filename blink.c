//  minicom -D /dev/tty.usbmodem1101 -b 115200
/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "pico/stdlib.h"
#include <stdio.h>

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

#define UART_TX_PIN_8 8  // GP8 (TX)
#define UART_RX_PIN_9 9  // GP9 (RX)
#define BTN_PIN 22     // GP22 (Button)

void setup_uart() 
{
    // Initialize UART1 at the desired baud rate
    uart_init(UART_ID, BAUD_RATE);
    
    // Set GPIO pins for UART1 TX (GP8) and RX (GP9)
    gpio_set_function(UART_TX_PIN_8, GPIO_FUNC_UART);
    gpio_set_function(UART_RX_PIN_9, GPIO_FUNC_UART);
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

// // Send a character 'A' via UART every 2 sec
// void loopback_test()
// {
//     while (true)
// 	{
//         uart_putc(UART_ID, 'A');
//         sleep_ms(500);
//         if (uart_is_readable(UART_ID)) 
// 		{
//             char received = uart_getc(UART_ID);
//             printf("Received: %c\n", received);
//         } 
// 		else 
//             printf("No data received!\n");
//         sleep_ms(1000);
//     }
// }

// lab3
static char event_str[128];

void gpio_event_string(char *buf, uint32_t events);

void gpio_callback(uint gpio, uint32_t events) {
    // Put the GPIO event(s) that just happened into event_str
    // so we can print it
    gpio_event_string(event_str, events);
    printf("GPIO %d %s\n", gpio, event_str);
}

static const char *gpio_irq_str[] = {
        "LEVEL_LOW",  // 0x1
        "LEVEL_HIGH", // 0x2
        "EDGE_FALL",  // 0x4
        "EDGE_RISE"   // 0x8
};

void gpio_event_string(char *buf, uint32_t events) {
    for (uint i = 0; i < 4; i++) {
        uint mask = (1 << i);
        if (events & mask) {
            // Copy this event string into the user string
            const char *event_str = gpio_irq_str[i];
            while (*event_str != '\0') {
                *buf++ = *event_str++;
            }
            events &= ~mask;

            // If more events add ", "
            if (events) {
                *buf++ = ',';
                *buf++ = ' ';
            }
        }
    }
    *buf++ = '\0';
}

// lab 3 ex 2
/// \tag::timer_example[]
volatile bool timer_fired = false;

int64_t alarm_callback(alarm_id_t id, __unused void *user_data) {
    printf("Timer %d fired!\n", (int) id);
    timer_fired = true;
    // Can return a value here in us to fire in the future
    return 0;
}

bool repeating_timer_callback(__unused struct repeating_timer *t) {
    printf("Repeat at %lld\n", time_us_64());
    return true;
}

int main() {
	stdio_init_all();  // Initialize USB serial
    int rc = pico_led_init();
    hard_assert(rc == PICO_OK);
	
	/*
	//set 1
	//testing loopback
	// This sends and check if a is received.
		// setup_uart();       // Set up UART1 for communication
		// loopback_test();    // Enter the loopback test function
	// end of testing
	*/ 

	/*
	//set2
 	// setup_uart();       // Set up UART1 for communication
    // setup_button();     // Set up the button (GP22)
    // loop();  // Enter the main loop
	*/

	/*
	printf("Hello GPIO IRQ\n");
	//  This will set up the interrupt to detect button presses on GPIO 20.
	// the onboard button on the Maker Pi Pico is physically connected to GP20,
	//  you do not need to connect any external wires for it to work. The button will trigger interrupts directly on GPIO 20 (GP20).
	// only for GP20
    gpio_set_irq_enabled_with_callback(20, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true, &gpio_callback);

	// For level low (trigger when the pin is held low):
	// gpio_set_irq_enabled_with_callback(20, GPIO_IRQ_LEVEL_LOW, true, &gpio_callback);
    
	// For level high (trigger when the pin is held high):
	// gpio_set_irq_enabled_with_callback(20, GPIO_IRQ_LEVEL_HIGH, true, &gpio_callback);
	
	// Wait forever
    while (1);
	*/

	/* lab 3 exercise 2 */
    // Call alarm_callback in 2 seconds
    add_alarm_in_ms(2000, alarm_callback, NULL, false);

    // Wait for alarm callback to set timer_fired
    while (!timer_fired) {
        tight_loop_contents();
    }

    // Create a repeating timer that calls repeating_timer_callback.
    // If the delay is > 0 then this is the delay between the previous callback ending and the next starting.
    // If the delay is negative (see below) then the next call to the callback will be exactly 500ms after the
    // start of the call to the last callback
    struct repeating_timer timer;
    add_repeating_timer_ms(500, repeating_timer_callback, NULL, &timer);
    sleep_ms(3000);
    bool cancelled = cancel_repeating_timer(&timer);
    printf("cancelled... %d\n", cancelled);
    sleep_ms(2000);

    // Negative delay so means we will call repeating_timer_callback, and call it again
    // 500ms later regardless of how long the callback took to execute
    add_repeating_timer_ms(-500, repeating_timer_callback, NULL, &timer);
    sleep_ms(3000);
    cancelled = cancel_repeating_timer(&timer);
    printf("cancelled... %d\n", cancelled);
    sleep_ms(2000);
    printf("Done\n");


    return 0;
}
