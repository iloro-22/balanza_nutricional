#ifndef FREERTOS_MOCK_H
#define FREERTOS_MOCK_H

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

#define configMINIMAL_STACK_SIZE 128
#define tskIDLE_PRIORITY 0

#define pdMS_TO_TICKS(ms) (ms)

typedef void (*TaskFunction_t)(void *);
typedef void* TaskHandle_t;
typedef uint32_t TickType_t;

#endif