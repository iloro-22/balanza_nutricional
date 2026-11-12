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
#include <stdio.h>
#include "sensor_peso.h"
#include "fsm_balanza.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"
#include "sapi.h"


#define LIMITE_SOBRECARGA 5500.0

extern QueueHandle_t cola_eventos;
extern TimerHandle_t timer_inactividad;

static float offset_interno = 0.0;
static float factor_escala = -299.3;

// Retardo NOp LPC4337 a 204 MHz (~200 ns)
static inline void delay_200ns(void) {
    for (volatile int i = 0; i < 25; i++) {
        __asm__("nop");
    }
}

int32_t hx711_leer_crudo(void) {
    int32_t lectura_cruda = 0;

    taskENTER_CRITICAL();

    for (uint8_t i = 0; i < 24; i++) {
        lectura_cruda = (lectura_cruda << 1);

        gpioWrite(PIN_SCK, 1);
        delay_200ns(); // Ancho de pulso ALTO (>200ns)

        if (gpioRead(PIN_DOUT) == 1) {
            lectura_cruda |= 1;
        }

        gpioWrite(PIN_SCK, 0); // Bajar SCK inmediatamente tras leer DOUT
        delay_200ns(); // Ancho de pulso BAJO (>200ns)
    }

    // Pulso 25 (Ganancia 128, Canal A)
    gpioWrite(PIN_SCK, 1);
    delay_200ns();
    gpioWrite(PIN_SCK, 0);
    delay_200ns();

    taskEXIT_CRITICAL();

    // Extensión de signo de 24 a 32 bits
    if (lectura_cruda & 0x800000) {
        lectura_cruda |= 0xFF000000;
    }
    return lectura_cruda;
}

int32_t hx711_leer_promedio(uint8_t cantidad_muestras) {
    int64_t suma = 0;
    uint8_t muestras_validas = 0;

    for (uint8_t i = 0; i < cantidad_muestras; i++) {
        uint16_t timeout = 500; // Máximo 500 ms de espera por muestra

        while (gpioRead(PIN_DOUT) == 1 && timeout > 0) {
            vTaskDelay(pdMS_TO_TICKS(1));
            timeout--;
        }

        if (timeout > 0) {
            suma += hx711_leer_crudo();
            muestras_validas++;
        }
    }

    if (muestras_validas == 0) return 0;
    return (int32_t)(suma / muestras_validas);
}

void hx711_tara(void) {
    offset_interno = (float)hx711_leer_promedio(20);
}

void hx711_CalibrarEscala(float peso_conocido_g) {
    float cruda = (float)hx711_leer_promedio(20);
    factor_escala = (cruda - offset_interno) / peso_conocido_g;
}

void task_sensor_peso(void *taskParmPtr) {
    MensajeFSM msj;
    float peso_anterior = -999.0;
    bool en_sobrecarga = false;
   
    uartWriteString(UART_USB, "[SENSOR] Haciendo tara inicial, espere...\r\n");
    hx711_tara();
    uartWriteString(UART_USB, "[SENSOR] Tara lista. Leyendo peso...\r\n");
   vTaskDelay(pdMS_TO_TICKS(5000)); // Te da 5 segundos para poner el plato

      hx711_CalibrarEscala(577.0f);

int entero = (int)factor_escala;
int decimales = (int)((factor_escala - entero) * 100);
if (decimales < 0) decimales = -decimales;

char buf[60];
sprintf(buf, "Nuevo factor_escala = %d.%02d\r\n", entero, decimales);
uartWriteString(UART_USB, buf);
    while (1) {
        if (gpioRead(PIN_DOUT) == 0) {
            int32_t lectura_cruda = hx711_leer_promedio(3);
            float peso_actual = (lectura_cruda - offset_interno) / factor_escala;

            if (peso_actual >= LIMITE_SOBRECARGA && !en_sobrecarga) {
                en_sobrecarga = true;
                msj.evento = EV_SOBRECARGA;
                xQueueSend(cola_eventos, &msj, 0);
            } 
            else if (peso_actual < LIMITE_SOBRECARGA && en_sobrecarga) {
                en_sobrecarga = false;
                msj.evento = EV_RETIRA_PESO;
                xQueueSend(cola_eventos, &msj, 0);
            } 
            else if (!en_sobrecarga) {
                float diferencia = peso_actual - peso_anterior;
                if (diferencia < 0) {
                    diferencia = -diferencia;
                }
                if (diferencia > 1.0) {
                    msj.evento = EV_CAMBIO_PESO;
                    peso_anterior = peso_actual;
                    msj.valor_peso = peso_actual;
                    msj.id_alimento = 0;
                    xTimerReset(timer_inactividad, 0);
                    xQueueSend(cola_eventos, &msj, 0);
                }
            }
        }
        vTaskDelay(pdMS_TO_TICKS(50));
    }
}