/**
 * Copyright (c) 2022 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"

const uint BTN_PIN = 20; //assigning BTN_PIN to pin no. 20

int main() {
    stdio_init_all(); //used to initialize all hardware components in a development environment

    gpio_set_dir(BTN_PIN, GPIO_IN); //sets the direction of the GPIO PIN, param1 = pin no.; param2 = input pin --> reads signals from pin
    //basically configure a gpio pin to receive input 

    gpio_set_pulls(BTN_PIN, true, false); //sets the pull-up or pull-down resistors on a GPIO pin
    //param1 = pin no.; param2 = enables pull-up resistor; param3 = disables pull-down resistor
    //what this means --> enable pull up --> pin defaults to high voltage 3.3V when button not pressed
    // disable pull down --> if button press --> pin connected to ground --> pin registers low signal (0V)

    if (cyw43_arch_init()) {
        printf("Wi-Fi init failed");
        return -1;
    }
    while (true) {
        if(gpio_get(BTN_PIN)){ //gpio_get --> returns state of pin --> if pin is high, voltage 1 --> it will run the code inside
            cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 1);
            sleep_ms(250);
            cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 0);
            sleep_ms(250);
        }
    }
}