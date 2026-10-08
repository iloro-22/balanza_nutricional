#ifndef FSM_PANTALLA_H
#define FSM_PANTALLA_H  

#include "ili9341.h"
#include "xpt2046.h"

typedef enum {
    UI_ESTADO_REPOSO,              // Pantalla apagada / modo ahorro
    UI_ESTADO_PESAJE_NORMAL,       // Peso en grande + botones TARA y ALIMENTOS
    UI_ESTADO_SELECCION_ALIMENTO,  // Grilla con los 6 alimentos + botón VOLVER
    UI_ESTADO_MODO_NUTRICIONAL,    // Peso + tabla de macros calculados
    UI_ESTADO_TARA,                // Cartel temporal "Tarando, por favor espere..."
    UI_ESTADO_ERROR                // Cartel de advertencia / sobrecarga
} EstadoUI;






#endif // FSM_PANTALLA_H