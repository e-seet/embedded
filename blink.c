// #include <stdio.h>
// #include <stdint.h>  // For standard integer types
// #include "FreeRTOS.h"
// #include "task.h"
// #include "queue.h"


// #include "hardware/gpio.h"
// #include <hardware/pwm.h>
// #include <hardware/adc.h>
// #include "hardware/irq.h"
// #include "hardware/timer.h"
// #include "pico/stdlib.h"


// #include <stdio.h>

// #include "pico/cyw43_arch.h"
// #include "pico/stdlib.h"
// #include "lwip/ip4_addr.h"
// // #include "message_buffer.h"


// #define mbaTASK_MESSAGE_BUFFER_SIZE       ( 60 )

// #ifndef PING_ADDR
// #define PING_ADDR "142.251.35.196"
// #endif
// #ifndef RUN_FREERTOS_ON_CORE
// #define RUN_FREERTOS_ON_CORE 0
// #endif

// #define TEST_TASK_PRIORITY				( tskIDLE_PRIORITY + 1UL )



// #define BAUD_RATE 115200

// // Queue handles
// QueueHandle_t tempQueue, printQueue;

// // Structure for sending temperature data
// typedef struct {
//     float temp;
//     float moving_avg;
//     float simple_avg;
// } PrintData_t;

// // Task 1: Temperature Read Task
// void TempReadTask(void *pvParameters) {
//     const TickType_t xDelay = pdMS_TO_TICKS(1000);  // 1-second delay
//     uint16_t raw_adc;
//     float temp_c;
    
//     // ADC initialization for temperature sensor
//     adc_init();
//     adc_select_input(4);  // Select temperature sensor
    
//     while(1) {
//         raw_adc = adc_read();
//         temp_c = 27.0f - (raw_adc - 0.706f) / 0.001721f;  // Temperature formula
        
//         // Send temperature data to Task 2 and Task 3
//         xQueueSend(tempQueue, &temp_c, portMAX_DELAY);
//         vTaskDelay(xDelay);  // 1-second delay
//     }
// }

// // Task 2: Moving Average Task
// void MovingAvgTask(void *pvParameters) {
//     float temp;
//     float buffer[10] = {0};
//     int idx = 0;
//     float sum = 0;
//     float moving_avg;
//     PrintData_t data;
    
//     while(1) {
//         // Receive temperature data from TempReadTask
//         if(xQueueReceive(tempQueue, &temp, portMAX_DELAY) == pdTRUE) {
//             sum -= buffer[idx];  // Subtract the old value
//             buffer[idx] = temp;  // Add the new value to the buffer
//             sum += temp;         // Add new temperature to sum
            
//             idx = (idx + 1) % 10;  // Circular buffer
//             moving_avg = sum / 10; // Compute moving average
            
//             // Send the result to PrintTask
//             data.temp = temp;
//             data.moving_avg = moving_avg;
//             data.simple_avg = 0;  // Simple average will be updated in another task
//             xQueueSend(printQueue, &data, portMAX_DELAY);
//         }
//     }
// }

// // Task 3: Simple Average Task
// void SimpleAvgTask(void *pvParameters) {
//     float temp;
//     float sum = 0;
//     int count = 0;
//     float simple_avg;
//     PrintData_t data;
    
//     while(1) {
//         // Receive temperature data from TempReadTask
//         if(xQueueReceive(tempQueue, &temp, portMAX_DELAY) == pdTRUE) {
//             sum += temp;
//             count++;
//             simple_avg = sum / count;  // Compute simple average
            
//             // Send the result to PrintTask
//             data.temp = temp;
//             data.moving_avg = 0;  // Moving average is computed in another task
//             data.simple_avg = simple_avg;
//             xQueueSend(printQueue, &data, portMAX_DELAY);
//         }
//     }
// }

// // Task 4: Printf Task (Handles all printf statements)
// void PrintTask(void *pvParameters) {
//     PrintData_t data;
    
//     while(1) {
//         // Receive data to print from other tasks
//         if(xQueueReceive(printQueue, &data, portMAX_DELAY) == pdTRUE) {
//             printf("Temperature: %.2f C | Moving Avg: %.2f C | Simple Avg: %.2f C\n", 
//                 data.temp, data.moving_avg, data.simple_avg);
//         }
//     }
// }

// int main() {
//     stdio_init_all();  // Initialize serial output
    
//     // Create queues
//     tempQueue = xQueueCreate(5, sizeof(float));  // For temperature readings
//     printQueue = xQueueCreate(5, sizeof(PrintData_t));  // For formatted print data
    
//     // Create tasks
//     xTaskCreate(TempReadTask, "Temperature Read Task", 256, NULL, 1, NULL);
//     xTaskCreate(MovingAvgTask, "Moving Average Task", 256, NULL, 1, NULL);
//     xTaskCreate(SimpleAvgTask, "Simple Average Task", 256, NULL, 1, NULL);
//     xTaskCreate(PrintTask, "Print Task", 256, NULL, 1, NULL);
    
//     // Start the FreeRTOS scheduler
//     vTaskStartScheduler();
    
//     // This point should never be reached
//     while(1);
//     return 0;
// }














































#include <stdio.h>

#include "pico/cyw43_arch.h"
#include "pico/stdlib.h"

#include "lwip/ip4_addr.h"

#include "FreeRTOS.h"
#include "task.h"
// #include "ping.h"
#include "message_buffer.h"

#include "hardware/gpio.h"
#include "hardware/adc.h"

#define mbaTASK_MESSAGE_BUFFER_SIZE       ( 60 )

#ifndef PING_ADDR
#define PING_ADDR "142.251.35.196"
#endif
#ifndef RUN_FREERTOS_ON_CORE
#define RUN_FREERTOS_ON_CORE 0
#endif

#define TEST_TASK_PRIORITY				( tskIDLE_PRIORITY + 1UL )

static MessageBufferHandle_t xControlMessageBuffer;

float read_onboard_temperature() {
    
    /* 12-bit conversion, assume max value == ADC_VREF == 3.3 V */
    const float conversionFactor = 3.3f / (1 << 12);

    float adc = (float)adc_read() * conversionFactor;
    float tempC = 27.0f - (adc - 0.706f) / 0.001721f;

    return tempC;
}

void main_task(__unused void *params) {
    if (cyw43_arch_init()) {
        printf("failed to initialise\n");
        return;
    }
    const char *WIFI_SSID = "SINGTEL-TD8A";
    const char *WIFI_PASSWORD = "3fr3fca2uh";
	
    cyw43_arch_enable_sta_mode();
    printf("Connecting to Wi-Fi...\n");
    if (cyw43_arch_wifi_connect_timeout_ms(WIFI_SSID, WIFI_PASSWORD, CYW43_AUTH_WPA2_AES_PSK, 30000)) {
        printf("failed to connect.\n");
        exit(1);
    } else {
        printf("Connected.\n");
    }

    ip_addr_t ping_addr;
    ipaddr_aton(PING_ADDR, &ping_addr);
    ping_init(&ping_addr);

    while(true) {
        // not much to do as LED is in another task, and we're using RAW (callback) lwIP API
        vTaskDelay(100);
    }

    cyw43_arch_deinit();
}

/* A Task that blinks the LED for 3000 ticks continuously */
void led_task(__unused void *params) {
    while(true) {
        vTaskDelay(3000);
        cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 1);
        vTaskDelay(3000);
        cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 0);
    }
}

/* A Task that obtains the data every 1000 ticks from the inbuilt temperature sensor (RP2040), prints it out and sends it to avg_task via message buffer */
void temp_task(__unused void *params) {
    float temperature = 0.0;

    adc_init();
    adc_set_temp_sensor_enabled(true);
    adc_select_input(4);

    while(true) {
        vTaskDelay(1000);
        temperature = read_onboard_temperature();
        printf("Onboard temperature = %.02f C\n", temperature);
        xMessageBufferSend( 
            xControlMessageBuffer,    /* The message buffer to write to. */
            (void *) &temperature,    /* The source of the data to send. */
            sizeof( temperature ),    /* The length of the data to send. */
            0 );                      /* Do not block, should the buffer be full. */
    }
}

/* A Task that indefinitely waits for data from temp_task via message buffer. Once received, it will calculate the moving average and prints out the result. */
void avg_task(__unused void *params) {
    float fReceivedData;
    float sum = 0;
    size_t xReceivedBytes;
    
    static float data[4] = {0};
    static int index = 0;
    static int count = 0;

    while(true) {
        xReceivedBytes = xMessageBufferReceive( 
            xControlMessageBuffer,        /* The message buffer to receive from. */
            (void *) &fReceivedData,      /* Location to store received data. */
            sizeof( fReceivedData ),      /* Maximum number of bytes to receive. */
            portMAX_DELAY );              /* Wait indefinitely */

            sum -= data[index];            // Subtract the oldest element from sum
            data[index] = fReceivedData;   // Assign the new element to the data
            sum += data[index];            // Add the new element to sum
            index = (index + 1) % 4;       // Update the index - make it circular
            
            if (count < 4) count++;        // Increment count till it reaches 4

            printf("Average Temperature = %0.2f C\n", sum / count);
    }
}

void vLaunch( void) {
    TaskHandle_t task;
    xTaskCreate(main_task, "TestMainThread", configMINIMAL_STACK_SIZE, NULL, TEST_TASK_PRIORITY, &task);
    TaskHandle_t ledtask;
    xTaskCreate(led_task, "TestLedThread", configMINIMAL_STACK_SIZE, NULL, 7, &ledtask);
    TaskHandle_t temptask;
    xTaskCreate(temp_task, "TestTempThread", configMINIMAL_STACK_SIZE, NULL, 8, &temptask);
    TaskHandle_t avgtask;
    xTaskCreate(avg_task, "TestAvgThread", configMINIMAL_STACK_SIZE, NULL, 5, &avgtask);

    xControlMessageBuffer = xMessageBufferCreate(mbaTASK_MESSAGE_BUFFER_SIZE);

#if NO_SYS && configUSE_CORE_AFFINITY && configNUM_CORES > 1
    // we must bind the main task to one core (well at least while the init is called)
    // (note we only do this in NO_SYS mode, because cyw43_arch_freertos
    // takes care of it otherwise)
    vTaskCoreAffinitySet(task, 1);
#endif

    /* Start the tasks and timer running. */
    vTaskStartScheduler();
}

int main( void )
{
    stdio_init_all();

    /* Configure the hardware ready to run the demo. */
    const char *rtos_name;
#if ( portSUPPORT_SMP == 1 )
    rtos_name = "FreeRTOS SMP";
#else
    rtos_name = "FreeRTOS";
#endif

#if ( portSUPPORT_SMP == 1 ) && ( configNUM_CORES == 2 )
    printf("Starting %s on both cores:\n", rtos_name);
    vLaunch();
#elif ( RUN_FREERTOS_ON_CORE == 1 )
    printf("Starting %s on core 1:\n", rtos_name);
    multicore_launch_core1(vLaunch);
    while (true);
#else
    printf("Starting %s on core 0:\n", rtos_name);
    vLaunch();
#endif
    return 0;
}