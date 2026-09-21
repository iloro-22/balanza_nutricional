/** Parametrizacion de los datos para ser enviados
   mediante UART al esp32 
*/

#include "esp32_uart.h"
#include "alimentos.h"
#include "sapi.h" //uart
#include <stdio.h>

void esp_enviar_macros(uint8_t id_alimento, float peso, informacion_alimento datos){
   char buffer_salida[120];
   snprintf(buffer_salida,sizeof(buffer_salida),
      "ID:%d,K:%.1f,P:%.1f,C:%.1f,G:%.1f,\r\n",
      id_alimento,
      peso,
      datos.proteinas,
      datos.carbohidratos,
      datos.grasas);
   uartWriteString(UART_232,buffer_salida);
}