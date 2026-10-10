#ifndef ARBOL_MENU_H
#define ARBOL_MENU_H

#include <stdint.h>
#include <stdbool.h>

typedef struct NodoMenu {
    const char *nombre;               // Texto a mostrar en el botón o encabezado
    uint8_t id_alimento;              // ID único para la FSM (0 si es una categoría intermedia)
    bool es_hoja;                     // true: alimento final | false: categoría con submenú
    struct NodoMenu *padre;           // Puntero al nivel anterior (para el botón VOLVER)
    struct NodoMenu *hijos[1];        // Hasta 4 botones por pantalla (grilla 2x2)
    uint8_t cant_hijos;               // Cantidad real de opciones en este nivel
} NodoMenu_t;

// Puntero a la raíz del árbol
extern NodoMenu_t arbol_raiz;

void arbol_alimentos_init(void);

#endif