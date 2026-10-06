#ifndef SSP1_MOCK_H
#define SSP1_MOCK_H

#include <stdint.h>

void SSP1_Init(void);
uint8_t SSP1_transfer_byte(uint8_t byte);

#endif