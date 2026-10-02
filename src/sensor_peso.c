/**
 * Tarea que se encarga de medir el peso del amplificador HX711
 * Funcionamiento: Se envian 24 pulsos de forma manual
 * para recibir los 24 bits por el pin DOUT. Luego de estos se pueden 
 * enviar desde 1 a 3 pulsos mas para seleccionar el canal
 * y tambien la ganancia. Por defecto se envia solo 1 pulso.
 * OFFSET = numero que tira la celda/amplificador cuando no
 * hay ningun elemento sobre la celda. Este se calcula y se le 
 * resta al peso "crudo"
 * Mediante un peso conocido que se coloca sobre la celda 
 * se calcula el factor de escala que te indica cuanto aumenta
 * aumenta (en decimal) el peso crudo por cada gramo.
 * Se calcula de la siguiente forma:
 * FE = (LECTURA_CRUDA - OFFSET)/(PESO CONOCIDO ACTUAL)
 * Para luego con el siguiente calculo obtener el peso real
 * PESO = (LECTURA_CRUDA - OFFSET) / FE
 */ 

#include <stdint.h>
#include "sensor_peso.h"
#include "fsm_balanza.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#define LIMITE_SOBRECARGA 5500.0
extern QueueHandle_t cola_eventos;

int32_t hx711_leer_crudo();
int32_t hx711_leer_promedio(uint8_t cantidad_muestras);

static float offset_interno = 0.0;
static float factor_escala = 1.0;

void task_sensor_peso(void *taskParmPtr){
   MensajeFSM msj;
   float peso_anterior = -999.0;
   bool en_sobrecarga = false;
   
   while(1){
      
      if(gpioRead(PIN_DOUT) == 0){
      
         int32_t lectura_cruda = hx711_leer_crudo();
      
         float peso_actual =  (lectura_cruda - offset_interno )/ factor_escala;
        
         
         if (peso_actual >= LIMITE_SOBRECARGA && en_sobrecarga == false){
            
               en_sobrecarga = true;
               MensajeFSM msj;
               msj.evento = EV_SOBRECARGA;
               xQueueSend(cola_eventos,&msj,0);
            
            }
         else if (peso_actual < LIMITE_SOBRECARGA && en_sobrecarga == true){
            en_sobrecarga = false;
            MensajeFSM msj;
            msj.evento = EV_RETIRA_PESO;
            xQueueSend(cola_eventos,&msj,0);
            }
         
         else if (en_sobrecarga == false){
            
            float diferencia = peso_actual - peso_anterior;
            if (diferencia < 0) {
               diferencia = -diferencia; //valor absoluto
               }
               
            if (diferencia > 1.0){
               msj.evento = EV_CAMBIO_PESO;
               peso_anterior = peso_actual;
               msj.valor_peso = peso_actual;   
               msj.id_alimento = 0;
               xQueueSend(cola_eventos,&msj,0);
            }
      }
   }
    vTaskDelay(pdMS_TO_TICKS(50));
   }
}

int32_t hx711_leer_crudo(){
    int32_t lectura_cruda = 0;
      
      //NO SE PUEDE INTERRUMPIR LA LECTURA YA QUE ES TEMPORIZADA Y SE DESINCRONIZARIA
      taskENTER_CRITICAL();
      
      for(uint8_t i=0;i<24;i++){
         gpioWrite(PIN_SCK,1);
          lectura_cruda = (lectura_cruda << 1);
        if(gpioRead(PIN_DOUT) == 1){
           lectura_cruda = lectura_cruda | 1;
           }
         gpioWrite(PIN_SCK,0);
         }
         //ganancia 128 y canal A
      gpioWrite(PIN_SCK,1);
      gpioWrite(PIN_SCK,0);
      taskEXIT_CRITICAL(); 
         
      // Si el bit 24 (0x800000) es un 1 (o sea, es negativo para el sensor)
      if (lectura_cruda & 0x800000) {
            // Rellená a la fuerza con unos (1) los 8 bits de más arriba (0xFF000000). ya que esto indica que es un numero negativo
            lectura_cruda |= 0xFF000000;
      }
      return lectura_cruda;
   }
int32_t hx711_leer_promedio(uint8_t cantidad_muestras){
   int64_t suma = 0;
   for(uint8_t i=0;i<cantidad_muestras;i++){
      // Esperar a que el dato esté listo (DOUT en bajo)
        while(gpioRead(PIN_DOUT) == 1) {
            vTaskDelay(pdMS_TO_TICKS(1)); // Cede el control a otras tareas mientras espera
         }
         suma += hx711_leer_crudo();
   }
   return (int32_t)(suma/ cantidad_muestras);
}
void hx711_tara(void){
   offset_interno = hx711_leer_promedio(20);
}
void hx711_CalibrarEscala(float peso_conocido_g) {
    // Lee el valor crudo promedio con el peso puesto
    float cruda = hx711_leer_promedio(20); 
    
    factor_escala = (cruda - offset_interno) / peso_conocido_g; 
}