#ifndef XPT2046_H
#define XPT2046_H
#include <stdint.h>
#include <stdbool.h>
#include "sapi.h"
// Resolución del display ILI9341
#define LCD_WIDTH      240
#define LCD_HEIGHT     320

// Valores analógicos crudos (RAW) típicos del XPT2046 (a ajustar con la placa real)
#define XPT2046_RAW_X_MIN   200
#define XPT2046_RAW_X_MAX   3800

#define XPT2046_RAW_Y_MIN   200
#define XPT2046_RAW_Y_MAX   3800
/*Definicion de Pines*/
#define XPT2046_CS GPIO3
#define XPT2046_IRQ GPIO4
/*Como el HW que compremos puede tener soldado el flex en forma invertida defino estas constantes
    para en caso de ser asi solo cambiar un valor*/
// ============================================================================
// CONFIGURACIÓN DE ORIENTACIÓN DEL TÁCTIL
// ============================================================================
// 0 = Modo Directo (Normal)
// 1 = Modo Invertido (Espejado)

#define XPT2046_INVERT_X    0   // Cambiar a 1 si el eje X responde al revés
#define XPT2046_INVERT_Y    0   // Cambiar a 1 si el eje Y responde al revés
#define XPT2046_SWAP_XY     0   // Cambiar a 1 si la pantalla está rotada (X se vuelve Y)

/*Prototipos de funciones*/
uint16_t XPT2046_mapX(uint16_t rawX);
uint16_t XPT2046_mapY(uint16_t rawY);
void XPT2046_init(void);
bool XPT2046_isPress(void);
bool XPT2046_getTouch(uint16_t *x, uint16_t *y);

#endif /* XPT2046_H */
