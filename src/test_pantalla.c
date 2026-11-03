#include "ili9341.h"
#include "xpt2046.h"

// Definición de las dimensiones del botón en pantalla (320x240)
#define BTN_X 60
#define BTN_Y 90
#define BTN_W 200
#define BTN_H 60

int main(void) {
    uint16_t touch_x = 0;
    uint16_t touch_y = 0;
    uint8_t estado_color = 0; // 0 = Negro, 1 = Azul

    // 1. Inicializar periféricos y controladores
    ILI9341_init();
    XPT2046_init(); // O la función de init de tu librería táctil si aplica

    // 2. Renderizado inicial: Fondo Negro
    ILI9341_fillScreen(ILI9341_BLACK);

    // Dibujar Botón (Rectángulo Blanco)
    ILI9341_fillRect(BTN_X, BTN_Y, BTN_W, BTN_H, ILI9341_WHITE);
    // Texto en el centro del botón (Texto Negro, Fondo Blanco)
    ILI9341_drawString(BTN_X + 25, BTN_Y + 20, "CAMBIAR COLOR", ILI9341_BLACK, ILI9341_WHITE);

    // 3. Bucle principal de eventos
    while (1) {
        // Verificar si se detecta toque físico en el panel táctil
        if (XPT2046_isPress()) {

            // Leer las coordenadas (x, y) mapeadas a la pantalla
            XPT2046_getTouch(&touch_x, &touch_y);

            // Verificar si el toque cae dentro del área delimitada del botón
            if (touch_x >= BTN_X && touch_x <= (BTN_X + BTN_W) &&
                touch_y >= BTN_Y && touch_y <= (BTN_Y + BTN_H)) {

                // Alternar estado de color
                estado_color = !estado_color;

                if (estado_color == 1) {
                    // Cambiar fondo a AZUL (o VERDE/ROJO según los defines de tu librería)
                    ILI9341_fillScreen(ILI9341_BLUE);
                } else {
                    // Cambiar fondo a NEGRO
                    ILI9341_fillScreen(ILI9341_BLACK);
                }

                // Volver a redibujar el botón sobre el nuevo fondo
                ILI9341_fillRect(BTN_X, BTN_Y, BTN_W, BTN_H, ILI9341_WHITE);
                ILI9341_drawString(BTN_X + 25, BTN_Y + 20, "CAMBIAR COLOR", ILI9341_BLACK, ILI9341_WHITE);

                // Anti-rebote / Espera a que se libere el toque para no conmutar repetidamente
                while (XPT2046_isPress()) {
                    // Pequeña espera hasta soltar la pantalla
                }
            }
        }
    }

    return 0;
}