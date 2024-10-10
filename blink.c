#include <stdio.h>
#include "pico/stdlib.h"
#include "FreeRTOS.h"
#include "FreeRTOSConfig.h"
#include "task.h"
#include "semphr.h"
#include "hardware/adc.h"

#define NUM_DATA_POINTS 10

#define BAUD_RATE 115200

int currentIndex = 0;
float temperatureReadings[NUM_DATA_POINTS] = {0.0};

SemaphoreHandle_t dataMutex;

TaskHandle_t readTemperatureTaskHandle;
TaskHandle_t calculateMovingAverageTaskHandle;
TaskHandle_t calculateSimpleAverageTaskHandle;
TaskHandle_t printDataTaskHandle;

// Function prototypes with new names
void ReadTemperatureTask(void *pvParameters);
void CalculateMovingAverageTask(void *pvParameters);
void CalculateSimpleAverageTask(void *pvParameters);
void PrintDataTask(void *pvParameters);


// Application stack overflow hook
void vApplicationStackOverflowHook(TaskHandle_t xTask, char *pcTaskName) {
    printf("Stack overflow in task: %s\n", pcTaskName);
    // Handle the stack overflow, perhaps reset the system or halt
    for(;;); // Loop forever to halt the system (for debugging)
}

// Application malloc failed hook
void vApplicationMallocFailedHook(void) {
    printf("Memory allocation failed!\n");
    // Handle malloc failure, perhaps reset the system or halt
    for(;;); // Loop forever to halt the system (for debugging)
}

uint32_t ulSetInterruptMask(void) {
    // Disable interrupts
    __asm volatile ("cpsid i");
    return 0;
}

void vClearInterruptMask(uint32_t mask) {
    // Enable interrupts
    __asm volatile ("cpsie i");
}

void vStartFirstTask(void) {
    __asm volatile (
        "ldr r0, =0xE000ED08\n"  /* Load address of vector table into R0. */
        "ldr r0, [r0]\n"         /* Load the first entry of the vector table into R0 (initial stack pointer). */
        "msr msp, r0\n"          /* Set the stack pointer to the value of R0. */
        "cpsie i\n"              /* Enable interrupts. */
        "svc 0\n"                /* Start the first task by issuing a supervisor call. */
    );
}

void vRestoreContextOfFirstTask(void) {
    // Your implementation here
}

int main() {
    stdio_init_all();
    adc_init();
    adc_set_temp_sensor_enabled(true);
    dataMutex = xSemaphoreCreateMutex();
    
    // Create four tasks
    xTaskCreate(ReadTemperatureTask, "ReadTempTask", configMINIMAL_STACK_SIZE, NULL, 2, &readTemperatureTaskHandle);
    xTaskCreate(CalculateMovingAverageTask, "CalcMovAvgTask", configMINIMAL_STACK_SIZE, NULL, 2, &calculateMovingAverageTaskHandle);
    xTaskCreate(CalculateSimpleAverageTask, "CalcSimpleAvgTask", configMINIMAL_STACK_SIZE, NULL, 2, &calculateSimpleAverageTaskHandle);
    xTaskCreate(PrintDataTask, "PrintDataTask", configMINIMAL_STACK_SIZE, NULL, 1, &printDataTaskHandle);
    
    // Start the FreeRTOS scheduler
    vTaskStartScheduler();
    
    while (1) {}
    return 0;
}

// Task that reads temperature sensor data and updates the temperatureReadings array
void ReadTemperatureTask(void *pvParameters) {
    (void)pvParameters;
    while (1) {
        uint16_t rawTemperature = adc_read();
        float scalingFactor = 0.35;
        float offset = 0.0;
        float temperatureCelsius = (float)rawTemperature * scalingFactor + offset;
        
        // Take the dataMutex to protect the shared temperatureReadings array
        xSemaphoreTake(dataMutex, portMAX_DELAY);
        temperatureReadings[currentIndex] = temperatureCelsius;
        currentIndex = (currentIndex + 1) % NUM_DATA_POINTS;
        
        // Give back the dataMutex
        xSemaphoreGive(dataMutex);
        
        vTaskDelay(pdMS_TO_TICKS(1000)); // Wait for 1000 milliseconds (1 second)
    }
}

// Task that calculates and updates the moving average of temperatureReadings
void CalculateMovingAverageTask(void *pvParameters) {
    (void)pvParameters;
    while (1) {
        float movingAverage = 0.0;
        
        // Take the dataMutex to protect the shared temperatureReadings array
        xSemaphoreTake(dataMutex, portMAX_DELAY);
        for (int i = 0; i < NUM_DATA_POINTS; i++) {
            movingAverage += temperatureReadings[i];
        }
        movingAverage /= NUM_DATA_POINTS;
        
        // Give back the dataMutex
        xSemaphoreGive(dataMutex);
        
        vTaskDelay(pdMS_TO_TICKS(1000)); // Wait for 1000 milliseconds (1 second)
    }
}

// Task that calculates and updates the simple average of temperatureReadings
void CalculateSimpleAverageTask(void *pvParameters) {
    (void)pvParameters;
    while (1) {
        float simpleAverage = 0.0;
        
        // Take the dataMutex to protect the shared temperatureReadings array
        xSemaphoreTake(dataMutex, portMAX_DELAY);
        for (int i = 0; i < NUM_DATA_POINTS; i++) {
            simpleAverage += temperatureReadings[i];
        }
        simpleAverage /= NUM_DATA_POINTS;
        
        // Give back the dataMutex
        xSemaphoreGive(dataMutex);
        
        vTaskDelay(pdMS_TO_TICKS(1000)); // Wait for 1000 milliseconds (1 second)
    }
}

// Task that prints the temperatureReadings array
void PrintDataTask(void *pvParameters) {
    (void)pvParameters;
    while (1) {
        printf("Temperature Readings: ");
        
        // Take the dataMutex to protect the shared temperatureReadings array
        xSemaphoreTake(dataMutex, portMAX_DELAY);
        for (int i = 0; i < NUM_DATA_POINTS; i++) {
            printf("%.2f ", temperatureReadings[i]);
        }
        printf("\n");
        
        // Give back the dataMutex
        xSemaphoreGive(dataMutex);
        
        vTaskDelay(pdMS_TO_TICKS(1000)); // Wait for 1000 milliseconds (1 second)
    }
}
