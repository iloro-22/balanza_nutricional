#include "sapi.h"
#include "ili9341.h"
#include "FreeRTOS.h"
#include "task.h"

/* Tarea de prueba para el Display */
void task_ili9341_test(void *pvParameters)
{
    /* 1. Inicializar controlador ILI9341 (SPI + GPIOs) */
    ILI9341_init();
    ILI9341_fillScreen(ILI9341_BLUE);
    while (1) {
        
        ILI9341_drawRect(10, 10, 100, 50, ILI9341_WHITE);
        ILI9341_fillRect(11, 11, 98, 48, ILI9341_RED);
        ILI9341_drawString(20, 30, "Hola Mundo!", ILI9341_WHITE, ILI9341_RED);
        vTaskDelay(pdMS_TO_TICKS(2000));
        
    }
}

int main(void)
{
    /* Inicialización de la EDU-CIAA */
    boardConfig();

    /* Crear la tarea del display en FreeRTOS */
    xTaskCreate(
        task_ili9341_test,
        "TaskTFT",
        configMINIMAL_STACK_SIZE * 2,
        NULL,
        tskIDLE_PRIORITY + 1,
        NULL
    );

    /* Iniciar el Scheduler de FreeRTOS */
    vTaskStartScheduler();

    /* Si llega acá, hubo un error de memoria en el Heap */
    while (1);
    return 0;
}