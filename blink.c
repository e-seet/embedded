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


// next exercise: on UART communiactions
// #define UART_ID uart1
#define BAUD_RATE 115200
// #define BTN_PIN 22     // GP22 (Button)

int pico_led_init(void) 
{
	#if defined(PICO_DEFAULT_LED_PIN)
		gpio_init(PICO_DEFAULT_LED_PIN);
		gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);
		return PICO_OK;
	#elif defined(CYW43_WL_GPIO_LED_PIN)
		// For Pico W devices we need to initialise the driver etc
		return cyw43_arch_init();
	#endif
}

// // lab3
// static char event_str[128];

// void gpio_event_string(char *buf, uint32_t events);

// static const char *gpio_irq_str[] = {
//         "LEVEL_LOW",  // 0x1
//         "LEVEL_HIGH", // 0x2
//         "EDGE_FALL",  // 0x4
//         "EDGE_RISE"   // 0x8
// };

// #define BTN_PIN_21 21     // GP22 (Button)

// volatile bool running = false;
// volatile uint32_t elapsed_time = 0;
// volatile bool button_pressed = false;
// volatile absolute_time_t last_debounce_time;
// const uint32_t debounce_delay_ms = 100;  // Debounce delay (in ms)

// // setup button
// void setup_button_21() 
// {
//     // Set GP21 as input with pull-down
//     gpio_init(BTN_PIN_21);
//     gpio_set_dir(BTN_PIN_21, GPIO_IN);
//     gpio_pull_down(BTN_PIN_21);
// }

// // Timer callback function to increment the elapsed time
// bool timer_callback(struct repeating_timer *t)
// {
//     if (running) 
// 	{
//         elapsed_time++;
//         printf("Elapsed: %d seconds\n", elapsed_time);
//     }
//     return true;  // Repeat the timer callback
// }

// Debounce the button and start/stop the timer
// void debounce_button() {
//     bool current_state = gpio_get(BTN_PIN_21);
	
// 	// get current time. Given function
//     absolute_time_t now = get_absolute_time();
    
//     // Check if button state has been stable for debounce delay
//     if (absolute_time_diff_us(last_debounce_time, now) >= debounce_delay_ms * 1000)
// 	 {
//         if (current_state && !button_pressed) 
// 		{
//             // Button was pressed (start the timer)
//             running = true;
//             button_pressed = true;
//             printf("Button pressed, starting timer.\n");
//         } 
// 		else if (!current_state && button_pressed) 
// 		{
//             // Button was released (stop and reset the timer)
//             running = false;
//             button_pressed = false;
//             elapsed_time = 0;
//             printf("Button released, resetting timer.\n");
//         }
//         last_debounce_time = now;
//     }
// }

// Interrupt Service Routine (ISR) for the button press
// void gpio_callback_2(uint gpio, uint32_t events) {
//     debounce_button();
// }


#define BUTTON_PIN 21

void setup_button() 
{
    // Set GP22 as input with pull-down
    // gpio_init(BTN_PIN);
    gpio_init(BUTTON_PIN);
	
    // gpio_set_dir(BTN_PIN, GPIO_IN);
    // gpio_pull_down(BTN_PIN);

    gpio_set_dir(BUTTON_PIN, GPIO_IN);
    gpio_pull_down(BUTTON_PIN);
}

// Global variables for time tracking
volatile uint32_t elapsed_time = 0;  // Elapsed time in seconds
volatile bool timer_active = false;  // Is the timer running?
volatile bool button_pressed = false;

// Timer callback function (called every second)
bool repeating_timer_callback(struct repeating_timer *t) {
    if (timer_active) {
        elapsed_time++;
        printf("Elapsed time: %d seconds\n", elapsed_time);
    }
    return true;
}

// Debounce logic for button press
bool debounce_button() {
    static uint32_t last_time = 0;
    uint32_t current_time = time_us_32();
    
    if (current_time - last_time > 200000) { // Debounce time: 200ms
        last_time = current_time;
        return true;
    }
    return false;
}

// GPIO interrupt handler for the button
void gpio_callback_set2(uint gpio, uint32_t events) {
    if (gpio == BUTTON_PIN && debounce_button()) {
        if (gpio_get(BUTTON_PIN) == 0) {  // Button pressed
            if (!button_pressed) 
			{
                timer_active = true;
                button_pressed = true;
                printf("Timer started!\n");
	        	printf("Elapsed time: %d seconds\n", elapsed_time);

            }
        } else {  // Button released
            timer_active = false;
            button_pressed = false;
            elapsed_time = 0;
            printf("Timer stopped and reset!\n");
        }
    }
}


int main() {
	stdio_init_all();  // Initialize USB serial
    int rc = pico_led_init();
    hard_assert(rc == PICO_OK);
	
	//exercise
	// The objective is to develop a simple stopwatch application. 
	// The stopwatch will be controlled by a single button on GP21. 
	
	// 1. Pressing and holding this button starts the timer, and the 
	// elapsed time in seconds will be continuously displayed on the
	//  Serial Monitor. 

	// 2. Releasing the button stops the timer and resets 
	// the displayed time to zero. 
	// To ensure a smooth user experience and accurate timekeeping, the GP21 button input will be debounced, 
	// and timer interrupts must be employed.

    setup_button();     // Set up the button (GP21)

//set 2
	  // Set up GPIO interrupt for button press
    gpio_set_irq_enabled_with_callback(BUTTON_PIN, GPIO_IRQ_EDGE_FALL | GPIO_IRQ_EDGE_RISE, true, &gpio_callback_set2);

    // Create a repeating timer (fires every 1000ms or 1 second)
    struct repeating_timer timer;
    add_repeating_timer_ms(1000, repeating_timer_callback, NULL, &timer);

    // Main loop
    while (true) {
        tight_loop_contents();  // Wait for events
    }

	
    return 0;
}


