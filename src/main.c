#include "sapi.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"


#include "fsm_balanza.h"
#include "sensor_peso.h"
#include "timers.h"

QueueHandle_t cola_eventos = NULL;
QueueHandle_t cola_pantalla = NULL;

// Lo declarás globalmente
TaskHandle_t handle_sensor_peso = NULL;

//handle global para poder usarlo en todas las task
TimerHandle_t timer_inactividad;

void vApplicationIdleHook(void) {
    // Dormimos el Cortex-M4 para ahorrar batería
    __WFI(); 
}
void callback_timeout (TimerHandle_t xTimer){
   MensajeFSM flag;
   flag.evento = EV_TIMEOUT;
   xQueueSend(cola_eventos,&flag,0);
   }

int main(void){
   
   boardConfig(); //inicializa relojes,pines y uarts.
   uartConfig(UART_USB,115200);
   uartConfig(UART_232,115200);
   
   cola_eventos = xQueueCreate(10,sizeof(MensajeFSM));
   cola_pantalla = xQueueCreate(10,sizeof(msj_pantalla));
   
    gpioConfig(PIN_SCK, GPIO_OUTPUT); //celda de carga
    gpioConfig(PIN_DOUT, GPIO_INPUT);
   //verificacion ed que la RAM alcanzo para crear las colas
   if (cola_eventos == NULL || cola_pantalla == NULL){
      //encender led en rojo
      while(1);
      }
   timer_inactividad = xTimerCreate("TimerInactividad",pdMS_TO_TICKS(60000), /**60 segs*/ pdFALSE, /** modo one_shot*/ (void * ) 0, /**ide del timer por si fuese necesario*/ callback_timeout  /**funcion que va a ser llamada cuando llegue a los segs*/);
   xTaskCreate(task_fsm, "FSM_Central",512,NULL,2,NULL);  
   xTaskCreate(task_sensor_peso, "Sensor_HX711",256,NULL,1,&handle_sensor_peso);
   
   vTaskStartScheduler();  
   //si llega aca, significa que FreeRTOS se quedo sin memoria RAM para arrancar
   while(1){}
}