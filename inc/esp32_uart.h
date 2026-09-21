#ifndef ESP32_UART_H
#define ESP32_UART_H

#include <stdint.h>
#include "alimentos.h"

void esp_enviar_macros(uint8_t id_alimento, float peso, informacion_alimento datos);

#endif