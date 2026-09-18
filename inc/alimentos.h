#ifndef ALIMENTOS_H
#define ALIMENTOS_H
#include <stdint.h>

typedef struct {
    float proteinas;
    float carbohidratos;
    float grasas;
    float calorias;
} informacion_alimento;

extern const informacion_alimento tabla_base[];

#endif