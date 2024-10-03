//  minicom -D /dev/tty.usbmodem1101 -b 115200
/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "pico/stdlib.h"
#include <stdio.h>
#include "hardware/gpio.h"
#include <hardware/pwm.h>
#include <hardware/adc.h>
#include "hardware/irq.h"
#include "hardware/timer.h"
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

// lab4
#define PWM_PIN 0      // GP0 for PWM output : PWM channel: 0A
#define ADC_PIN 26     // GP26 for ADC input
#define SAMPLE_INTERVAL 25  // Sample every 25 ms

// Timer callback for ADC sampling
bool timer_callback(repeating_timer_t *rt) {
	// static uint64_t last_time_us = 0;  // Track the last timestamp

    // uint64_t current_time_us = to_us_since_boot(get_absolute_time());  // Get current time in microseconds
    // uint64_t elapsed_time_ms = (current_time_us - last_time_us) / 1000;  // Calculate elapsed time in milliseconds

    uint16_t adc_value = adc_read();  // Read ADC value
    // float voltage = adc_value * 3.3f / 4095.0f;  // Convert to voltage (assuming 12-bit ADC resolution)
	// printf("Current time:%llums,  Time since last sample: %llu ms, adc value :%d | ADC Voltage: %.2f V,\n", current_time_us/1000, elapsed_time_ms,adc_value ,voltage );
	printf("ADC Value :%d \n", adc_value);

    return true;
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

int main() {
	stdio_init_all();  // Initialize USB serial
    int rc = pico_led_init();
    hard_assert(rc == PICO_OK);
	
	 // Initialize PWM on GP0 (PWM_PIN)
	  // Tell GPIO 0 they are allocated to the PWM
    gpio_set_function(PWM_PIN, GPIO_FUNC_PWM);
	 // Find out which PWM slice is connected to GPIO 0 (it's slice 0)
    uint slice_num = pwm_gpio_to_slice_num(PWM_PIN);
    
    // Set PWM frequency to 20 Hz
    uint32_t clk_div = 125000000 / (20 * 65536);  // Calculation based on 125MHz clock
    pwm_set_clkdiv(slice_num, (float)clk_div);
    
    // Set PWM duty cycle to 50%
    pwm_set_wrap(slice_num, 65535);
    pwm_set_gpio_level(PWM_PIN, 32768);  // 50% duty cycle
	// Set the PWM running
    pwm_set_enabled(slice_num, true);    // Enable PWM

    // Initialize ADC on GP26 (ADC_PIN)
    adc_init();
    adc_gpio_init(ADC_PIN);
    adc_select_input(0);  // Select ADC channel for GP26

    // Set up a repeating timer to sample the ADC every 25 ms
    repeating_timer_t timer;
    add_repeating_timer_ms(SAMPLE_INTERVAL, timer_callback, NULL, &timer);

    // Main loop does nothing; the ADC sampling is handled by the timer interrupt
    while (true) {
        tight_loop_contents();  // Keep the main loop running
    }

	
    return 0;
}


