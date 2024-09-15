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

int main() {
	stdio_init_all();  // Initialize USB serial
    int rc = pico_led_init();
    hard_assert(rc == PICO_OK);

    while (true) {
        pico_set_led(true);
		printf("wow\n");
        sleep_ms(LED_DELAY_MS);
        pico_set_led(false);
        sleep_ms(LED_DELAY_MS);
    }
}


// The following code works for the following
/* The modified code, picow_blink_button, configures GP20 as an input 
with a pull-up resistor. When the button connected to GP20 is not pressed,
the pin remains in a high state. Pressing the GP20 button will pull the 
pin low, causing the LED to stop blinking (as defined in the code). 
Releasing the button returns the pin to a high state, allowing the LED
to resume blinking.
*/

// #include <stdio.h>
// #include "pico/stdlib.h"
// #include "pico/cyw43_arch.h"


// const uint BTN_PIN = 20;

// int main() {
//     stdio_init_all();

// 	// the next two line is newly added
//     gpio_set_dir(BTN_PIN, GPIO_IN);
//     gpio_set_pulls(BTN_PIN, true, false);

//     if (cyw43_arch_init()) {
//         printf("Wi-Fi init failed.");
//         return -1;
//     }
//     while (true) {
//         if(gpio_get(BTN_PIN))
//         {
// 			printf("wow\n");
// 			// the next line and if statement is newly added
//             cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 1);
//             sleep_ms(250);
//             cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 0);
//             sleep_ms(250);
//         }
//     }
// }
