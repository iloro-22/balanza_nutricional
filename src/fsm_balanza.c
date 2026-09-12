#include "fsm_balanza.h" 
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"


/**
 * Tarea principal de la Máquina de Estados (FSM).
 * Se bloquea esperando mensajes en la cola y ejecuta las transiciones
 * de la balanza basándose en los eventos del sensor y la interfaz.
 */
 
extern QueueHandle_t cola_eventos;

void task_fsm(void* taskParmPtr){
   MensajeFSM msj;
   EstadoBalanza estado_actual = REPOSO;
   float offset_tara = 0.0;
   
   while(1){
      
      //La tarea se duerme aca hasta que alguien mande un msj
      if (xQueueReceive(cola_eventos, &msj, portMAX_DELAY) == pdTRUE){
         
         //maxima prioridad al error
         if (msj.evento == EV_SOBRECARGA){
            estado_actual = ESTADO_ERROR;
            }
      
         switch (estado_actual) {
               case REPOSO:
                  if (msj.evento ==EV_CAMBIO_PESO){
                     // actualizar_pantalla(msj.valor_peso)
                     estado_actual = PESAJE_NORMAL;
                     }
                  break;
               case PESAJE_NORMAL:
                  // actualizar_pantalla(msj.valor_peso)
                  if (msj.evento == EV_TARA){
                     estado_actual = TARA;
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
                  //acttualizar offset_tara
                  //avisar a pantalla que se taro
                  estado_actual = PESAJE_NORMAL;
                  break;
               
               case MODO_NUTRICIONAL:
                  
                  //logica para calcular calorias, mostrar pantalla y esperar boton
                  //timeout o boton volver
                  break;
               case  ESTADO_ERROR:
                     //mostrar error en pantalla
                     if (msj.evento == EV_RETIRA_PESO){
                        estado_actual = REPOSO;
                        }
                  break;
                  
         }
            
      }
      
      
      
   }

   
}