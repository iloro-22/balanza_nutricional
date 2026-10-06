#ifndef SAPI_MOCK_H
#define SAPI_MOCK_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

#define OFF 0
#define ON  1

#define GPIO_OUTPUT 1
#define GPIO_INPUT  0

#define GPIO0 0
#define GPIO1 1
#define GPIO2 2
#define GPIO3 3
#define GPIO4 4

void boardConfig(void);
void gpioInit(uint8_t pin, uint8_t direction);
void gpioWrite(uint8_t pin, uint8_t value);
void delay(uint32_t duration_ms);

#endif