#ifndef TASK_MOCK_H
#define TASK_MOCK_H

#include "FreeRTOS.h"

int xTaskCreate(TaskFunction_t pvTaskCode,
                const char * const pcName,
                uint16_t usStackDepth,
                void *pvParameters,
                uint32_t uxPriority,
                TaskHandle_t *pxCreatedTask);

void vTaskDelay(TickType_t xTicksToDelay);
void vTaskStartScheduler(void);

#endif