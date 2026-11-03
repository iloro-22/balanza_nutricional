#ifndef SENSOR_PESO_H
#define SENSOR_PESO_H

#include <stdint.h>

void hx711_tara(void);
void hx711_CalibrarEscala(uint16_t peso_patron_gramos);

#endif