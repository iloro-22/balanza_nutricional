#include <ili9341.h>

void writeCommand(uint8_t cmd) {

    gpioWrite(ILI9341_DC, OFF); // Poner DC bajo para comando
    gpioWrite(ILI9341_CS, OFF); // Poner CS bajo para seleccionar la pantalla
    SSP1_transfer_byte(cmd);     // Enviar el byte de comando
    gpioWrite(ILI9341_CS, ON);  // Poner CS alto para deseleccionar la pantalla

}

void writeData(uint8_t data) {

    gpioWrite(ILI9341_DC, ON);  // Poner DC alto para datos
    gpioWrite(ILI9341_CS, OFF); // Poner CS bajo para seleccionar la pantalla
    SSP1_transfer_byte(data);    // Enviar el byte de datos
    gpioWrite(ILI9341_CS, ON);  // Poner CS alto para deseleccionar la pantalla

}

void ILI9341_init(void) {

    SSP1_Init(); // Inicializar el periférico SSP1
    gpioInit(ILI9341_CS, GPIO_OUTPUT);
    gpioInit(ILI9341_DC, GPIO_OUTPUT);
    gpioInit(ILI9341_RST, GPIO_OUTPUT);
    gpioWrite(ILI9341_RST, OFF); // Reiniciar la pantalla
    delay(10); // Esperar 10 ms    
    gpioWrite(ILI9341_RST, ON); // Liberar reset
    delay(120); // Esperar 120 ms
    writeCommand(0x01); // Reset Software
    delay(120); // Esperar 120 ms
    writeCommand(0x28); // Display OFF
    writeCommand(0x3A); // Definir el formato de pixel
    writeData(0x55);    // 16 bits por pixel
    writeCommand(0x11); // Enviar a dormir
    delay(120); // Esperar 120 ms
    writeCommand(0x29); // Display ON

}

void ILI9341_setAddressWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1) {

    writeCommand(0x2A); // Definir la columna de dirección
    writeData(x0 >> 8);
    writeData(x0 & 0xFF);
    writeData(x1 >> 8);
    writeData(x1 & 0xFF);

    writeCommand(0x2B); // Definir la fila de dirección
    writeData(y0 >> 8);
    writeData(y0 & 0xFF);
    writeData(y1 >> 8);
    writeData(y1 & 0xFF);

    writeCommand(0x2C); // Guardar en memoria 
    /*Ahora la pantalla está lista para recibir datos*/
}

void ILI9341_drawPixel(uint16_t x, uint16_t y, uint16_t color) {

    ILI9341_setAddressWindow(x, y, x, y); // Definir la ventana de dirección para un solo píxel
    writeData(color >> 8); // Enviar el byte alto del color
    writeData(color & 0xFF); // Enviar el byte bajo del color

}

void ILI9341_fillScreen(uint16_t color) {

    ILI9341_setAddressWindow(0, 0, ILI9341_WIDTH - 1, ILI9341_HEIGHT - 1); // Definir la ventana de dirección para toda la pantalla
    for (uint32_t i = 0; i < ILI9341_WIDTH * ILI9341_HEIGHT; i++) {
        writeData(color >> 8); // Enviar el byte alto del color
        writeData(color & 0xFF); // Enviar el byte bajo del color
    }
}

void ILI9341_fillRect (uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color) {

    ILI9341_setAddressWindow(x, y, x + w - 1, y + h - 1); // Defininis la ventana del rectángulo. x, y son las coordenadas de la esquina superior izquierda, w es el ancho y h es la altura del rectángulo
    for (uint32_t i = 0; i < w * h; i++) {
        writeData(color >> 8); // Enviar el byte alto del color
        writeData(color & 0xFF); // Enviar el byte bajo del color
    }
}

void ILI9341_drawRect (uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color) {

    // Dibujar la línea superior
    for (uint16_t i = 0; i < w; i++) {
        ILI9341_drawPixel(x + i, y, color);
    }
    // Dibujar la línea inferior
    for (uint16_t i = 0; i < w; i++) {
        ILI9341_drawPixel(x + i, y + h - 1, color);
    }
    // Dibujar la línea izquierda
    for (uint16_t i = 0; i < h; i++) {
        ILI9341_drawPixel(x, y + i, color);
    }
    // Dibujar la línea derecha
    for (uint16_t i = 0; i < h; i++) {
        ILI9341_drawPixel(x + w - 1, y + i, color);
    }
}

void ILI9341_drawChar(uint16_t x, uint16_t y, char c, uint16_t color, uint16_t bg) {

    if (c < 32 || c > 126) return; // Verificar si el carácter está en el rango válido

    for (uint8_t i = 0; i < 8; i++) { // Iterar sobre las filas del carácter
        uint8_t line = font8x8[c - 32][i]; // Obtener la línea correspondiente del carácter
        for (uint8_t j = 0; j < 8; j++) { // Iterar sobre los bits de la línea
            if (line & (1 << j)) {
                ILI9341_drawPixel(x + j, y + i, color); // Dibujar píxel del carácter
            } else {
                ILI9341_drawPixel(x + j, y + i, bg); // Dibujar píxel de fondo
            }
        }
    }
}

void ILI9341_drawString(uint16_t x, uint16_t y, const char *str, uint16_t color, uint16_t bg) {

    while (*str) { // Iterar sobre cada carácter de la cadena
        ILI9341_drawChar(x, y, *str, color, bg); // Dibujar el carácter actual
        x += 8; // Mover la posición x para el siguiente carácter
        str++; // Avanzar al siguiente carácter en la cadena
    }
}