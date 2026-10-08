#ifndef ILI9341_H
#define ILI9341_H

#include <stdint.h>
#include "sapi.h"
#include "ssp1.h"
#include "font8x8.h"

/*Definicion de Pines*/
#define ILI9341_CS GPIO0
#define ILI9341_DC GPIO1
#define ILI9341_RST GPIO2
/*Dimensiones de Pantalla*/
#define ILI9341_WIDTH 240
#define ILI9341_HEIGHT 320
/*Definicion de coleres*/
#define ILI9341_BLACK 0x0000
#define ILI9341_WHITE 0xFFFF
#define ILI9341_RED 0xF800
#define ILI9341_GREEN 0x07E0
#define ILI9341_BLUE 0x001F
#define ILI9341_YELLOW 0xFFE0
#define ILI9341_NAVY 0x000F
/*Prototipos*/
void writeCommand(uint8_t cmd);
void writeData(uint8_t data);
void ILI9341_init(void);
void ILI9341_setAddressWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);
void ILI9341_drawPixel(uint16_t x, uint16_t y, uint16_t color);
void ILI9341_fillScreen(uint16_t color);
void ILI9341_fillRect (uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color);
void ILI9341_drawRect (uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color);
void ILI9341_drawChar(uint16_t x, uint16_t y, char c, uint16_t color, uint16_t bg);
void ILI9341_drawString(uint16_t x, uint16_t y, const char *str, uint16_t color, uint16_t bg);
#endif /* ILI9341_H */