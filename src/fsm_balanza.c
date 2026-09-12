

/**
 * Tarea principal de la Máquina de Estados (FSM).
 * Se bloquea esperando mensajes en la cola y ejecuta las transiciones
 * de la balanza basándose en los eventos del sensor y la interfaz.
 */


#include "fsm_balanza.h" 
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

 
extern QueueHandle_t cola_eventos;

void task_fsm(void* taskParmPtr){
   MensajeFSM msj;
   EstadoBalanza estado_actual = REPOSO;
   float offset_tara = 0.0;
   informacion_alimento kcal_alimento;
   
   while(1){
      
      //La tarea se duerme aca hasta que alguien mande un msj
      if (xQueueReceive(cola_eventos, &msj, portMAX_DELAY) == pdTRUE){
         
         //maxima prioridad al error
         if (msj.evento == EV_SOBRECARGA){
            estado_actual = ERROR;
            }
      
         switch (estado_actual) {
               case REPOSO:
                  if (msj.evento ==EV_CAMBIO_PESO){
                     // actualizar_pantalla(msj.valor_peso)
                     estado_actual = PESAJE_NORMAL;
                     }
                  break;
               case PESAJE_NORMAL:
                  if (msj.evento == EV_TARA){
                     estado_actual = TARA;
                     }
                  else if (msj.evento == EV_CAMBIO_PESO){
                      // actualizar_pantalla(msj.valor_peso - offset_tara)
                     }
                  else if (msj.evento == EV_ALIMENTO){
                     //guardar_id_alimento_actual(msj.id_alimento)
                     estado_actual = MODO_NUTRICIONAL;
                     }
                  else if (msj.evento == EV_TIMEOUT){
                     //apagar_pantalla();
                     estado_actual = REPOSO;
                     }
                  break;
               case  TARA:
                  offset_tara = msj.valor_peso;
                  //avisa_pantalla_tara();
                  estado_actual = PESAJE_NORMAL;
                  break;
               
               case MODO_NUTRICIONAL:
                  
                  if (msj.evento == EV_CAMBIO_PESO){
                     //kcal_alimento = calcular_calorias(msj.valor_peso);
                     //actualizar_pantalla(msj.valor_peso - offset_tara, kcal_alimento - offset_tara);
                  }
                  else if ((msj.evento == EV_TIMEOUT) || (msj.evento == EV_VOLVER)){
                        estado_actual = REPOSO;
                        }
                  break;
               case  ERROR:
                     //error_pantalla();
                     if (msj.evento == EV_RETIRA_PESO){
                        estado_actual = REPOSO;
                        }
                  break;
                  
         }
      }
   }
}