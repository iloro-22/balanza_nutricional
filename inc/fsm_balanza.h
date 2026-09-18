#ifndef FSM_BALANZA_H
#define FSM_BALANZA_H
#include <stdint.h>
typedef enum {
   REPOSO,
   PESAJE_NORMAL,
   TARA,
   MODO_NUTRICIONAL,
   ESTADO_ERROR
} EstadoBalanza;

typedef enum {
   EV_SOBRECARGA = 1,
   EV_ALIMENTO = 2,
   EV_CAMBIO_PESO = 3,
   EV_TARA = 4,
   EV_TIMEOUT = 5,
   EV_RETIRA_PESO = 6,
   EV_VOLVER = 7
} EventoBalanza;

typedef struct {
   EventoBalanza evento;
   float valor_peso;
   uint8_t id_alimento;
} MensajeFSM;

void task_fsm(void* taskParmPtr);
#endif