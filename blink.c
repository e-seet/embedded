#include <stdio.h>

#include "include/myRTOS.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#include "pico/stdlib.h"
#include "hardware/adc.h"

// Queue handles
QueueHandle_t tempQueue, printQueue;

// Structure for sending temperature data
typedef struct {
    float temp;
    float moving_avg;
    float simple_avg;
} PrintData_t;

// Task 1: Temperature Read Task
void TempReadTask(void *pvParameters) {
    const TickType_t xDelay = pdMS_TO_TICKS(1000);  // 1-second delay
    uint16_t raw_adc;
    float temp_c;
    
    // ADC initialization for temperature sensor
    adc_init();
    adc_select_input(4);  // Select temperature sensor
    
    while(1) {
        raw_adc = adc_read();
        temp_c = 27.0f - (raw_adc - 0.706f) / 0.001721f;  // Temperature formula
        
        // Send temperature data to Task 2 and Task 3
        xQueueSend(tempQueue, &temp_c, portMAX_DELAY);
        vTaskDelay(xDelay);  // 1-second delay
    }
}

// Task 2: Moving Average Task
void MovingAvgTask(void *pvParameters) {
    float temp;
    float buffer[10] = {0};
    int idx = 0;
    float sum = 0;
    float moving_avg;
    PrintData_t data;
    
    while(1) {
        // Receive temperature data from TempReadTask
        if(xQueueReceive(tempQueue, &temp, portMAX_DELAY) == pdTRUE) {
            sum -= buffer[idx];  // Subtract the old value
            buffer[idx] = temp;  // Add the new value to the buffer
            sum += temp;         // Add new temperature to sum
            
            idx = (idx + 1) % 10;  // Circular buffer
            moving_avg = sum / 10; // Compute moving average
            
            // Send the result to PrintTask
            data.temp = temp;
            data.moving_avg = moving_avg;
            data.simple_avg = 0;  // Simple average will be updated in another task
            xQueueSend(printQueue, &data, portMAX_DELAY);
        }
    }
}

// Task 3: Simple Average Task
void SimpleAvgTask(void *pvParameters) {
    float temp;
    float sum = 0;
    int count = 0;
    float simple_avg;
    PrintData_t data;
    
    while(1) {
        // Receive temperature data from TempReadTask
        if(xQueueReceive(tempQueue, &temp, portMAX_DELAY) == pdTRUE) {
            sum += temp;
            count++;
            simple_avg = sum / count;  // Compute simple average
            
            // Send the result to PrintTask
            data.temp = temp;
            data.moving_avg = 0;  // Moving average is computed in another task
            data.simple_avg = simple_avg;
            xQueueSend(printQueue, &data, portMAX_DELAY);
        }
    }
}

// Task 4: Printf Task (Handles all printf statements)
void PrintTask(void *pvParameters) {
    PrintData_t data;
    
    while(1) {
        // Receive data to print from other tasks
        if(xQueueReceive(printQueue, &data, portMAX_DELAY) == pdTRUE) {
            printf("Temperature: %.2f C | Moving Avg: %.2f C | Simple Avg: %.2f C\n", 
                data.temp, data.moving_avg, data.simple_avg);
        }
    }
}

int main() {
    stdio_init_all();  // Initialize serial output
    
    // Create queues
    tempQueue = xQueueCreate(5, sizeof(float));  // For temperature readings
    printQueue = xQueueCreate(5, sizeof(PrintData_t));  // For formatted print data
    
    // Create tasks
    xTaskCreate(TempReadTask, "Temperature Read Task", 256, NULL, 1, NULL);
    xTaskCreate(MovingAvgTask, "Moving Average Task", 256, NULL, 1, NULL);
    xTaskCreate(SimpleAvgTask, "Simple Average Task", 256, NULL, 1, NULL);
    xTaskCreate(PrintTask, "Print Task", 256, NULL, 1, NULL);
    
    // Start the FreeRTOS scheduler
    vTaskStartScheduler();
    
    // This point should never be reached
    while(1);
    return 0;
}