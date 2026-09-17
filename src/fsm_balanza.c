

/**
 * Tarea principal de la Máquina de Estados (FSM).
 * Se bloquea esperando mensajes en la cola y ejecuta las transiciones
 * de la balanza basándose en los eventos del sensor y la interfaz.
 */


#include "fsm_balanza.h" 
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "pantalla.h"
 
extern QueueHandle_t cola_eventos;
extern QueueHandle_t cola_pantalla;

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
                  
                  if (msj.evento == EV_CAMBIO_PESO){
                     /** msj_pantalla msj_out;
                      * msj_out.evento = MOSTRAR_SOLO_PESO;
                      * msj_out.peso = msj.valor_peso - offset_tara;
                      * xQueueSend(cola_pantalla,&msj_out,0);
                      */
                     estado_actual = PESAJE_NORMAL;
                     }
                  else if (msj.evento ==  EV_TARA ){
                        offset_tara = msj.valor_peso;
                          /** msj_pantalla msj_out;
                        * msj_out.evento = MOSTRAR_SOLO_PESO;
                       * msj_out.peso = 0;
                       * xQueueSend(cola_pantalla,&msj_out,0);
                       */
                        estado_actual = PESAJE_NORMAL;
                     }
                     
                   else if (msj.evento ==  EV_ALIMENTO ){
                      /** 
                        guardar_id_alimento_actual(msj.id_alimento);
                      * msj_pantalla msj_out;
                      * msj_out.evento = MOSTRAR_MACROS_PESO;
                      * msj_out.peso = msj.valor_peso - offset_tara;;
                      * xQueueSend(cola_pantalla,&msj_out,0);
                      */
                        estado_actual = PESAJE_NORMAL;
                     }
                  break;
               case PESAJE_NORMAL:
                  if ((msj.evento == EV_CAMBIO_PESO) ){

                     /** 
                      * msj_pantalla msj_out;
                      * msj_out.evento = MOSTRAR_SOLO_PESO;
                      * msj_out.peso = msj.valor_peso - offset_tara;;
                      * xQueueSend(cola_pantalla,&msj_out,0);
                      */
                     }
                  else if (msj.evento == EV_ALIMENTO){
                     //guardar_id_alimento_actual(msj.id_alimento);
                     estado_actual = MODO_NUTRICIONAL;
                     }
                  else if (msj.evento ==  EV_TARA ){
                        offset_tara = msj.valor_peso;
                          /** msj_pantalla msj_out;
                          * informacion_alimento macros_cero = {0,0,0,0};
                           msj_out.kcal_alimento = macros_cero;
                         * msj_out.evento = MOSTRAR_SOLO_PESO;
                         * msj_out.peso = 0.0;
                         * xQueueSend(cola_pantalla,&msj_out,0);
                         */
                  }
                   else if (msj.evento == EV_TIMEOUT){
                        offset_tara = 0.0;
                      /** msj_pantalla msj_out;
                      * msj_out.evento = REPOSO;
                      * xQueueSend(cola_pantalla,&msj_out,0);
                      */
                     estado_actual = REPOSO;
                     }
                  break;
                     
               case MODO_NUTRICIONAL:
                  
                  if ((msj.evento == EV_CAMBIO_PESO) || (msj.evento == EV_ALIMENTO)){

                     /** if (msj.evento == EV_ALIMENTO) { 
                        guardar_id_alimento_actual(msj.id_alimento);
                      }
                      * kcal_alimento = calcular_macros(msj.id_alimento, msj.valor_peso - offset_tara);
                      * msj_pantalla msj_out;
                      * msj_out.evento = MOSTRAR_MACROS_PESO;
                      * msj_out.peso = msj.valor_peso - offset_tara;;
                      * xQueueSend(cola_pantalla,&msj_out,0);
                      */
                  }
                  else if (msj.evento ==  EV_TARA ){
                        offset_tara = msj.valor_peso;
                          /** msj_pantalla msj_out;
                        * msj_out.evento = MOSTRAR_MACROS_PESO;
                       * msj_out.kcal_alimento = kcal_alimento;
                       * msj_out.peso = 0;
                       * xQueueSend(cola_pantalla,&msj_out,0);
                       */
                  }
                  else if (msj.evento == EV_VOLVER){
                     estado_actual = PESAJE_NORMAL;
                  } 
                  else if (msj.evento == EV_TIMEOUT){
                        offset_tara = 0.0;
                         /** msj_pantalla msj_out;
                      * msj_out.evento = REPOSO;
                      * xQueueSend(cola_pantalla,&msj_out,0);
                      */
                        estado_actual = REPOSO;
                        }
                  break;
               case  ERROR:
                     /**msj_pantalla msj_out;
                      * msj_out.evento = ERROR;
                      * xQueueSend(cola_pantalla,&msj_out,0);
                     */
                     if (msj.evento == EV_RETIRA_PESO){
                        offset_tara = 0.0;
                         /** msj_pantalla msj_out;
                      * msj_out.evento = REPOSO;
                      * xQueueSend(cola_pantalla,&msj_out,0);
                      */
                        estado_actual = REPOSO;
                        }
                  break;
                  
         }
      }
   }
}