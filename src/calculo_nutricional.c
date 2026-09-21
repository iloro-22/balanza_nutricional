/**
 * Lógica de procesamiento de datos nutricionales y escalado de macros.
 * 
 * ============================================================================
 * NOTAS DE ARQUITECTURA Y GESTIÓN DE MEMORIA
 * ============================================================================
 * 1. MEMORIA FLASH (ROM): La base de datos (tabla_base) se genera automáticamente
 *    vía script de Python y se declara con la directiva 'const'. Esto obliga al 
 *    compilador a "quemar" la matriz entera directamente en la memoria Flash 
 *    del microcontrolador, liberando la valiosa y limitada memoria RAM para 
 *    que FreeRTOS gestione las tareas y las colas de mensajes sin desbordes.
 * 
 * 2. DESACOPLAMIENTO DE INTERFAZ: La lógica de la balanza es "ciega" respecto 
 *    a la pantalla. La FSM no procesa cadenas de texto (ej: "Pollo"). Solo 
 *    recibe un 'id_alimento' (entero) desde la interfaz, lo usa como índice para 
 *    acceder a la memoria Flash en tiempo constante O(1), y devuelve la matemática.
 * 
 * ============================================================================
 */

#include "alimentos.h"

informacion_alimento calcular_macros(uint8_t id_alimento, float peso){
   informacion_alimento resultado = {0.0,0.0,0.0,0.0};
   
   if ( peso <= 0.0 ) {
      return resultado;
      }
   float factor = peso / 100.0;
   
   resultado.calorias = tabla_base[id_alimento].calorias * factor;
   resultado.proteinas = tabla_base[id_alimento].proteinas * factor;
   resultado.carbohidratos = tabla_base[id_alimento].carbohidratos * factor;
   resultado.grasas = tabla_base[id_alimento].grasas * factor;
   return resultado;
   }