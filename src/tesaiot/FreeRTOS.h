#ifndef FREERTOS_H
#define FREERTOS_H
#include <stdint.h>
#define portENTER_CRITICAL()
#define portEXIT_CRITICAL()
#define taskENTER_CRITICAL()
#define taskEXIT_CRITICAL()
typedef void * TaskHandle_t;
typedef void * SemaphoreHandle_t;
typedef uint32_t TickType_t;
#define pdTRUE 1
#define pdFALSE 0
#define portMAX_DELAY 0xFFFFFFFF
#endif
