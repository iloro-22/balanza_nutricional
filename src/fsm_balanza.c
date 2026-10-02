

/**
 * Tarea principal de la Máquina de Estados (FSM).
 * Se bloquea esperando mensajes en la cola y ejecuta las transiciones
 * de la balanza basándose en los eventos del sensor y la interfaz.
 */


#include "fsm_balanza.h" 
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "pantalla_spi.h"

extern TaskHandle_t handle_sensor_peso;
extern QueueHandle_t cola_eventos;
extern QueueHandle_t cola_pantalla;

void task_fsm(void* taskParmPtr){
   MensajeFSM msj;
   uint8_t id_alimento = 0;
   EstadoBalanza estado_actual = REPOSO;
   informacion_alimento kcal_alimento;
   
   while(1){
      
      //La tarea se duerme aca hasta que alguien mande un msj
      if (xQueueReceive(cola_eventos, &msj, portMAX_DELAY) == pdTRUE){
         
         //maxima prioridad al error
         if (msj.evento == EV_SOBRECARGA){
            estado_actual = ESTADO_ERROR;
            }
      
         switch (estado_actual) {
            
               case REPOSO:
                  
                  if (msj.evento == EV_TOQUE_PANTALLA){
                        msj_pantalla msj_out;
                        msj_out.evento = CMD_MOSTRAR_SOLO_PESO;
                        msj_out.peso = msj.valor_peso;
                        xQueueSend(cola_pantalla,&msj_out,0);
                        vTaskResume(handle_sensor_peso);
                     estado_actual = PESAJE_NORMAL;
                     }
                  break;
                     
               case PESAJE_NORMAL:
                  if ((msj.evento == EV_CAMBIO_PESO) ){
                     
                      msj_pantalla msj_out;
                      msj_out.evento = CMD_MOSTRAR_SOLO_PESO;
                      msj_out.peso = msj.valor_peso;
                      xQueueSend(cola_pantalla,&msj_out,0);
                    
                     }
                  else if (msj.evento == EV_ALIMENTO){
                     //guardar_id_alimento_actual(msj.id_alimento);
                     estado_actual = MODO_NUTRICIONAL;
                     }
                  else if (msj.evento ==  EV_TARA ){
                           hx711_tara();
                           msj_pantalla msj_out;
                           informacion_alimento macros_cero = {0,0,0,0};
                           msj_out.kcal_alimento = macros_cero;
                           msj_out.evento = CMD_MOSTRAR_SOLO_PESO;
                           msj_out.peso = 0.0;
                           xQueueSend(cola_pantalla,&msj_out,0);
                  }
                   else if (msj.evento == EV_TIMEOUT){
                        offset_tara = 0.0;
                        msj_pantalla msj_out;
                        msj_out.evento = CMD_AHORRO;
                       xQueueSend(cola_pantalla,&msj_out,0);
                       vTaskSuspend(handle_sensor_peso);
                        estado_actual = REPOSO;
                     }
                     else if (msj.evento == EV_INICIAR_CALIBRACION) {
                        estado_actual = ESTADO_CALIBRACION;
                        
                        
                        // la pantalla dibuja "Por favor suelte la pantalla, tarando..."
                        msj_pantalla msj_out;
                        msj_out.evento = CMD_DIBUJAR_TARA;
                        xQueueSend(cola_pantalla, &msj_out, 0);
                         
                         // le da 2 segundos al usuario para que saque la mano y la balanza se estabilice
                         vTaskDelay(pdMS_TO_TICKS(2000));
                         
                         //tara automáticamente por defecto
                         hx711_tara(); 
                         
                         //le manda a la pantalla que pida el peso guia
                         msj_out.evento = CMD_PEDIR_PESO;
                         xQueueSend(cola_pantalla,&msj_out,0);
            }
                  break;
                     
               case MODO_NUTRICIONAL:
                  
                  if ((msj.evento == EV_CAMBIO_PESO) || (msj.evento == EV_ALIMENTO)){

                     if (msj.evento == EV_ALIMENTO) { 
                        id_alimento = msj.id_alimento);
                      }
                      kcal_alimento = calcular_macros(msj.id_alimento, msj.valor_peso);
                      msj_pantalla msj_out;
                      msj_out.evento = CMD_MOSTRAR_MACROSyPESO;
                      msj_out.peso = msj.valor_peso;
                      xQueueSend(cola_pantalla,&msj_out,0);
                      esp_enviar_macros(id_alimento,msj.valor_peso, kcal_alimento);
                  }
                  else if (msj.evento ==  EV_TARA ){
                          hx711_tara();
                          msj_pantalla msj_out;
                          msj_out.evento = CMD_MOSTRAR_MACROSyPESO;
                          msj_out.kcal_alimento = kcal_alimento;
                          msj_out.peso = 0;
                          xQueueSend(cola_pantalla,&msj_out,0);
                  }
                  else if (msj.evento == EV_VOLVER){
                     estado_actual = PESAJE_NORMAL;
                  } 
                  else if (msj.evento == EV_TIMEOUT){
                         msj_pantalla msj_out;
                        msj_out.evento = CMD_AHORRO;
                        xQueueSend(cola_pantalla,&msj_out,0);
                        vTaskSuspend(handle_sensor_peso);
                        estado_actual = REPOSO;
                        }
                  else if (msj.evento == EV_INICIAR_CALIBRACION) {
                     estado_actual = ESTADO_CALIBRACION;
                     
                     // la pantalla dibuja "Por favor suelte la pantalla, tarando..."
                      xQueueSend(cola_Pantalla, DIBUJAR_TARA, 0); 
                      
                      // le da 2 segundos al usuario para que saque la mano y la balanza se estabilice
                      vTaskDelay(pdMS_TO_TICKS(2000));
                      
                      //tara automáticamente por defecto
                      hx711_tara(); 
                      
                      //le manda a la pantalla que pida el peso guia
                      msj_pantalla msj_out;
                      msj_out.evento = CMD_PEDIR_PESO;
                      xQueueSend(cola_pantalla,&msj_out,0);
            }
            break;
               case  ESTADO_ERROR:
                       msj_pantalla msj_out;
                       msj_out.evento = CMD_ERROR;
                       xQueueSend(cola_pantalla,&msj_out,0);
                     if (msj.evento == EV_RETIRA_PESO){
                        msj_pantalla msj_out;
                        msj_out.evento = CMD_AHORRO;
                        xQueueSend(cola_pantalla,&msj_out,0);
                        estado_actual = REPOSO;
                     }
                  break;
                 case  CALIBRACION:
                         if (msj.evento == EV_BOTON_100G){
                           hx711_CalibrarEscala(100);
                        }
                        else if (msj.evento == EV_BOTON_500G){
                           hx711_CalibrarEscala(500);
                        }
                        else if (msj.evento == EV_BOTON_1000G){
                           hx711_CalibrarEscala(1000);
                        }
                        else {
                           break;//por si llega un ev_cambio_peso por ejemplo no te vas de calibracion.
                           } 
                        estado_actual = ESTADO_REPOSO;
                        msj_pantalla msj_out;
                        msj_out.evento = CMD_DIBUJAR_EXITO_AHORRO; //debe indicar que salio bien y apagarse
                        xQueueSend(cola_pantalla,&msj_out,0)         
                               
                  break;
                  
         }
      }
   }
}