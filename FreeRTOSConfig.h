#ifndef FREERTOS_CONFIG_H
#define FREERTOS_CONFIG_H

/*-----------------------------------------------------------
 * Application-specific definitions.
 *
 * Adjust these definitions for your hardware and application requirements.
 * These parameters are described within the 'Configuration' section of the
 * FreeRTOS API documentation.
 *----------------------------------------------------------*/

/* Preemption and tick configuration */
#define configUSE_PREEMPTION                    1
#define configUSE_IDLE_HOOK                     0
#define configUSE_TICK_HOOK                     0
#define configCPU_CLOCK_HZ                      125000000  // 125 MHz for Raspberry Pi Pico
#define configTICK_RATE_HZ                      ((TickType_t) 1000)
#define configMAX_PRIORITIES                    5
#define configMINIMAL_STACK_SIZE                ((uint16_t) 128)
#define configTOTAL_HEAP_SIZE                   ((size_t) (16 * 1024))  // 16 KB heap for tasks
#define configMAX_TASK_NAME_LEN                 16
#define configUSE_TRACE_FACILITY                0
#define configUSE_16_BIT_TICKS                  0
#define configIDLE_SHOULD_YIELD                 1
#define configUSE_MUTEXES                       1

/* Memory allocation */
#define configSUPPORT_DYNAMIC_ALLOCATION        1
#define configSUPPORT_STATIC_ALLOCATION         0

/* Hook function related definitions */
#define configCHECK_FOR_STACK_OVERFLOW          2

/* Timer and software timers */
#define configUSE_TIMERS                        1
#define configTIMER_TASK_PRIORITY               2
#define configTIMER_QUEUE_LENGTH                10
#define configTIMER_TASK_STACK_DEPTH            configMINIMAL_STACK_SIZE

/* Co-routine definitions (disable by default) */
#define configUSE_CO_ROUTINES                   0
#define configMAX_CO_ROUTINE_PRIORITIES         2

/* Semaphore and Queue */
#define configQUEUE_REGISTRY_SIZE               10
#define configUSE_COUNTING_SEMAPHORES           1
#define configUSE_RECURSIVE_MUTEXES             1

/* Task priorities and API functions */
#define INCLUDE_vTaskPrioritySet                1
#define INCLUDE_uxTaskPriorityGet               1
#define INCLUDE_vTaskDelete                     1
#define INCLUDE_vTaskSuspend                    1
#define INCLUDE_xTaskDelayUntil                 1
#define INCLUDE_vTaskDelay                      1
#define INCLUDE_xTaskGetSchedulerState          1

/* Cortex-M specific definitions */
#ifdef __NVIC_PRIO_BITS
 #define configPRIO_BITS                        __NVIC_PRIO_BITS
#else
 #define configPRIO_BITS                        4        /* 15 priority levels */
#endif

/* The lowest priority. */
#define configLIBRARY_LOWEST_INTERRUPT_PRIORITY         0xf

/* The highest priority that can be used by any interrupt service routine that
   makes FreeRTOS API calls. */
#define configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY    5

/* Interrupt priorities used by the kernel port layer itself. */
#define configKERNEL_INTERRUPT_PRIORITY          ( configLIBRARY_LOWEST_INTERRUPT_PRIORITY << (8 - configPRIO_BITS) )
#define configMAX_SYSCALL_INTERRUPT_PRIORITY     ( configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY << (8 - configPRIO_BITS) )

/* Assert configuration */
#define configASSERT(x) if ((x) == 0) { taskDISABLE_INTERRUPTS(); for( ;; ); }

/* Map FreeRTOS port interrupt handlers to their CMSIS standard names. */
#define vPortSVCHandler    SVC_Handler
#define xPortPendSVHandler PendSV_Handler
#define xPortSysTickHandler SysTick_Handler

#endif /* FREERTOS_CONFIG_H */