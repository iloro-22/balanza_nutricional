#ifndef FSM_BALANZA_H
#define FSM_BALANZA_H
#include <stdint.h>
#include "alimentos.h"
#include "calculo_nutricional.h"
typedef enum {
   REPOSO,
   PESAJE_NORMAL,
   TARA,
   MODO_NUTRICIONAL,
   ESTADO_ERROR,
   CALIBRACION
} EstadoBalanza;

typedef enum {
   EV_SOBRECARGA = 1,
   EV_ALIMENTO = 2,
   EV_CAMBIO_PESO = 3,
   EV_TARA = 4,
   EV_TIMEOUT = 5,
   EV_RETIRA_PESO = 6,
   EV_VOLVER = 7,
   EV_INICIAR_CALIBRACION = 8,
   EV_FIN_CALIBRACION = 9,
   EV_TOQUE_PANTALLA = 10
} EventoBalanza;

typedef enum {
    CMD_MOSTRAR_SOLO_PESO,       // Actualiza los gramos grandes en el centro
    CMD_MOSTRAR_MACROSyPESO,     // Dibuja la tabla nutricional + peso
    CMD_DIBUJAR_TARA,            // Muestra "Tarando, por favor espere..."
    CMD_PEDIR_PESO,              // Muestra los 3 botones de calibraci�n (100g, 500g, 1000g)
    CMD_DIBUJAR_EXITO_AHORRO,    // Tilde verde de calibraci�n exitosa y se apaga a los 3 seg
    CMD_AHORRO,                  // Apaga el backlight de una (Deep Sleep)
    CMD_ERROR                    // Pantalla roja gigante de SOBRECARGA
} ComandoPantalla_t;

typedef struct {
    ComandoPantalla_t evento;            
    float peso;                           
    informacion_alimento kcal_alimento; 
} msj_pantalla;

typedef struct {
   EventoBalanza evento;
   float valor_peso;
   uint8_t id_alimento;
} MensajeFSM;

void task_fsm(void* taskParmPtr);
#endif