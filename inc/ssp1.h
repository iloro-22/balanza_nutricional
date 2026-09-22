#ifndef DRIVERS_SPI_SSP1_H_
#define DRIVERS_SPI_SSP1_H_

#include "sapi.h" // Cabecera de LPCOpen para los tipos de datos del LPC4337

// Inicializa el periférico SSP1 del LPC4337 en modo SPI Maestro (~10 MHz, CPOL=0, CPHA=0)
void SSP1_Init(void);

// Transmite un byte por MOSI y retorna el byte recibido por MISO (Full-Duplex)
uint8_t SSP1_transfer_byte(uint8_t data);

// Permite cambiar el tamaño de la palabra entre 8 bits (comandos/touch) y 16 bits (píxeles)
void SSP1_change_format(uint8_t data_bits);

#endif /* DRIVERS_SPI_SSP1_H_ */