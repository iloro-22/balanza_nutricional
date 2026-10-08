





/*



    MAIN PARA SIMULAR LA PANTALLA Y PROBAR LOS MENUS



*/



#include "main.h"
/* Tarea de prueba para el Display y Táctil */
void task_ili9341_test(void *pvParameters)
{
    /* 1. Inicializar controlador ILI9341 */
    ILI9341_init();
    ILI9341_fillScreen(ILI9341_BLUE);
    /* 2. Dibujar el botón inicial (se dibuja una sola vez) */
    ILI9341_drawRect(70, 140, 100, 40, ILI9341_WHITE);
    ILI9341_fillRect(71, 141, 98, 38, ILI9341_RED);
    ILI9341_drawString(82, 156, "PRESIONAR", ILI9341_WHITE, ILI9341_RED);
    bool presionado = false;
    while (1) {
        /* 3. Evaluar estado del táctil */
        if (XPT2046_isPress()) {
            uint16_t x, y;
            if (XPT2046_getTouch(&x, &y)) {
                if (!presionado) {
                    // Si es un nuevo toque, cambiamos el botón a VERDE
                    ILI9341_fillRect(71, 141, 98, 38, ILI9341_GREEN);
                    ILI9341_drawString(88, 156, "TOCADO!", ILI9341_WHITE, ILI9341_GREEN);
                    presionado = true;
                }
            }
        } else {
            if (presionado) {
                // Al soltar el toque, vuelve a ROJO
                ILI9341_fillRect(71, 141, 98, 38, ILI9341_RED);
                ILI9341_drawString(82, 156, "PRESIONAR", ILI9341_WHITE, ILI9341_RED);
                presionado = false;
            }
        }

        /* CLAVE: Refresca el simulador de SDL y cede CPU a FreeRTOS */
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}

int main(void)
{
    /* Inicialización de la EDU-CIAA / Simulador */
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

    /* Iniciar el Scheduler */
    vTaskStartScheduler();

    while (1);
    return 0;
}