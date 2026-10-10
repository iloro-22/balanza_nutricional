#ifndef TASK_PANTALLA_H
#define TASK_PANTALLA_H 

/*Includes*/

#include "sapi.h"
#include "ili9341.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "fsm_balanza.h"
#include "xpt2046.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
/*Constantes*/
// 1. Encabezado Superior (Y: 0 a 30)
#define HEADER_X                0
#define HEADER_Y                0
#define HEADER_W                320
#define HEADER_H                30
#define HEADER_TEXT_X           15
#define HEADER_TEXT_Y           8

// 2. Sector Izquierdo - Tarjeta de Peso (X: 5 a 105, Y: 35 a 190)
#define PESO_BOX_X              5
#define PESO_BOX_Y              35
#define PESO_BOX_W              100
#define PESO_BOX_H              155

#define PESO_DIGITOS_X          10
#define PESO_DIGITOS_Y          80
#define PESO_DIGITOS_W          90
#define PESO_DIGITOS_H          35

#define PESO_TITULO_Y           48
#define PESO_UNIDAD_Y           135

// 3. Sector Central Derecho - Menú y Vistas (X: 110 a 315, Y: 35 a 190)
#define MENU_DER_X              110
#define MENU_DER_Y              35
#define MENU_DER_W              205
#define MENU_DER_H              155

// Selección de Alimentos (Grilla 2x2: Ancho 95 px, Alto 55 px)
#define BTN_ALIM_W              95
#define BTN_ALIM_H              55

// Fila Superior
#define BTN_ALIM1_X             115
#define BTN_ALIM1_Y             45
#define TXT_ALIM1_X             125
#define TXT_ALIM1_Y             64
#define BTN_ALIM2_X             215
#define BTN_ALIM2_Y             45
#define TXT_ALIM2_X             225
#define TXT_ALIM2_Y             64
// Fila Inferior
#define BTN_ALIM3_X             115
#define BTN_ALIM3_Y             115
#define TXT_ALIM3_X             125
#define TXT_ALIM3_Y             134
#define BTN_ALIM4_X             215
#define BTN_ALIM4_Y             115
#define TXT_ALIM4_X             225
#define TXT_ALIM4_Y             134

// Modo Nutricional (Filas de texto dentro del sector derecho)
#define MACRO_FILA_KCAL_Y       45
#define MACRO_FILA_PROT_Y       80
#define MACRO_FILA_CARB_Y       115
#define MACRO_FILA_GRAS_Y       150

// 4. Sector Inferior - Botoneras (Y: 198 a 238, Alto: 40 px)
// Modo Pesaje Normal (2 botones anchos de 145 px)
#define BTN_NORMAL_Y            198
#define BTN_NORMAL_H            40
#define BTN_NORMAL_W            95

// Botón 1: ALIMENTOS
#define BTN_NORMAL_ALIM_X       10
#define TXT_NORMAL_ALIM_X       20      // "ALIMENTOS"

// Botón 2: TARA
#define BTN_NORMAL_TARA_X       112
#define TXT_NORMAL_TARA_X       140     // "TARA"

// Botón 3: CALIBRAR
#define BTN_NORMAL_CALIB_X      215
#define TXT_NORMAL_CALIB_X      225     // "CALIBRAR"

#define TXT_NORMAL_Y            211     // Altura común de texto en barra inferior


// Modo Nutricional (3 botones cómodos de 95 px cada uno)
#define BTN_NUTRI_Y             198
#define BTN_NUTRI_H             40
#define BTN_NUTRI_W             95

#define BTN_NUTRI_VOLVER_X      10
#define BTN_NUTRI_TARA_X        112
#define BTN_NUTRI_ENVIAR_X      215
#define TXT_BOTONERA_Y          211

// Modo Pesaje Normal (2 botones)
#define TXT_NORMAL_ALIM_X       40      // "ALIMENTOS"
#define TXT_NORMAL_TARA_X       210     // "TARA"

// Modo Selección (1 botón ancho)
#define TXT_SEL_VOLVER_X        135     // "VOLVER"

// Modo Nutricional (3 botones)
#define TXT_NUTRI_VOLVER_X      30      // "VOLVER"
#define TXT_NUTRI_TARA_X        140     // "TARA"
#define TXT_NUTRI_ENVIAR_X      235     // "ENVIAR"

// 3. Textos del Modo Nutricional (Sector Derecho)
#define TXT_MACRO_ETIQUETA_X    115     // Columna donde arrancan las palabras ("Kcal", "Prot:", etc.)
#define TXT_MACRO_VALOR_X       200     // Columna donde arrancan los valores numéricos calculados
#define TARA_BOX_X              115
#define TARA_BOX_Y              45
#define TARA_BOX_W              195
#define TARA_BOX_H              135

// Coordenadas de los textos dentro del cartel de Tara
#define TXT_TARA_AVISO_X        140     // "TARANDO..."
#define TXT_TARA_AVISO_Y        90

/** PANTALLA DE CALIBRACIÓN - PESOS PATRÓN (CMD_PEDIR_PESO)*/
 
// 3 Botones para elegir el peso patrón (Sector Central: Y: 80, Alto: 50 px)
#define BTN_PATRON_Y            80
#define BTN_PATRON_W            90
#define BTN_PATRON_H            50

#define BTN_PATRON_100G_X       15
#define TXT_PATRON_100G_X       35      // "100 g"

#define BTN_PATRON_500G_X       115
#define TXT_PATRON_500G_X       135     // "500 g"

#define BTN_PATRON_1000G_X      215
#define TXT_PATRON_1000G_X      230     // "1000 g"

#define TXT_PATRON_Y            98      // Centrado vertical dentro de los 50 px

// Botón Inferior para Cancelar / Salir de Calibración
#define BTN_CALIB_CANCELAR_X    10
#define BTN_CALIB_CANCELAR_Y    198
#define BTN_CALIB_CANCELAR_W    300
#define BTN_CALIB_CANCELAR_H    40
#define TXT_CALIB_CANCELAR_X    120     // "CANCELAR"
#define TXT_CALIB_CANCELAR_Y    211
// Modo Selección (1 botón ancho de cancelación de 300 px)
#define BTN_SEL_VOLVER_X        10
#define BTN_SEL_VOLVER_Y        198
#define BTN_SEL_VOLVER_W        300
#define BTN_SEL_VOLVER_H        40
/*Enumerativo de los estados de la pantalla*/


typedef enum {
    UI_ESTADO_REPOSO,              // Pantalla apagada / modo ahorro
    UI_ESTADO_PESAJE_NORMAL,       // Peso en grande + botones TARA y ALIMENTOS
    UI_ESTADO_SELECCION_ALIMENTO,  // Grilla con los alimentos + botón VOLVER
    UI_ESTADO_MODO_NUTRICIONAL,    // Peso + tabla de macros calculados
    UI_ESTADO_TARA,                // Cartel temporal "Tarando, por favor espere..."
    UI_ESTADO_ERROR,             // Cartel de advertencia / sobrecarga
    UI_ESTADO_CALIBRACION            // Pantalla de calibración: botones de 100g, 500g, 1000g
} EstadoUI;

/*Prototipos de funciones*/

void task_pantalla(void *pvParameters);
void ui_marco_izq(void);
void ui_peso(float peso);
void ui_marco_der(void);
void ui_menu_derecha_pesaje_normal(void);
void ui_menu_derecha_seleccion_alimentos(void);
void ui_menu_derecha_macros(const uint16_t macros); /*Aca habría que poner el struct que corresponde a las macros*/
void ui_dibujar_encabezado(const char *titulo);
void ui_dibujar_botonera(EstadoUI estado);
void ui_pantalla_reposo(void);
void ui_pantalla_pesaje_base(float peso_actual);
void ui_pantalla_calibracion();
#endif // TASK_PANTALLA_H