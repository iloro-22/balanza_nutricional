#include "xpt2046.h"


void XPT2046_init(void) {
    gpioInit(XPT2046_CS, GPIO_OUTPUT);
    gpioInit(XPT2046_IRQ, GPIO_INPUT);
    SSP1_change_format(8); // Seteo en 8 bits para la comunicación SPI
}

bool XPT2046_isPress(void) {
    return !gpioRead(XPT2046_IRQ); // Activo en bajo
}

uint16_t XPT2046_mapX(uint16_t rawX) {
    // Mapea el valor crudo de X a la resolución del display
    return (rawX - XPT2046_RAW_X_MIN) * LCD_WIDTH / (XPT2046_RAW_X_MAX - XPT2046_RAW_X_MIN);
}

uint16_t XPT2046_mapY(uint16_t rawY) {
    // Mapea el valor crudo de Y a la resolución del display
    return (rawY - XPT2046_RAW_Y_MIN) * LCD_HEIGHT / (XPT2046_RAW_Y_MAX - XPT2046_RAW_Y_MIN);
}

bool XPT2046_getTouch(uint16_t *x, uint16_t *y) {
    if (!XPT2046_isPress()) {
        return false; // No hay toque
    }

    uint8_t commandX = 0xD0; // Comando propio del XPT2046 para leer X
    uint8_t commandY = 0x90; // Comando propio del XPT2046 para leer Y

    gpioWrite(XPT2046_CS, 0); // Activar el chip select

    SSP1_transfer_byte(commandX); // Enviar comando para X
    uint16_t rawX = (SSP1_transfer_byte(0x00) << 8) | SSP1_transfer_byte(0x00); // Leer datos de X generando ciclos de reloj adicionales

    SSP1_transfer_byte(commandY); // Enviar comando para Y
    uint16_t rawY = (SSP1_transfer_byte(0x00) << 8) | SSP1_transfer_byte(0x00); // Leer datos de Y generando ciclos de reloj adicionales

    gpioWrite(XPT2046_CS, 1); // Desactivar el chip select

    rawX = rawX >> 3; // Ajustar el valor de X e Y, como el ADC es 12 bits, se hace un corrimiento a la derecha 
    rawY = rawY >> 3; // de 3 bits para obtener un valor de 0 a 4095

    uint16_t mappedX = (uint16_t)map(rawX, XPT2046_RAW_X_MIN, XPT2046_RAW_X_MAX, 0, LCD_WIDTH - 1); 
    /*Aplico la función que mapea las lecturas del ADC a los pixeles de la pantalla*/
    uint16_t mappedY = (uint16_t)map(rawY, XPT2046_RAW_Y_MIN, XPT2046_RAW_Y_MAX, 0, LCD_HEIGHT - 1);

    #if XPT2046_INVERT_X
    mappedX = (LCD_WIDTH - 1) - mappedX;
    #endif  

    #if XPT2046_INVERT_Y
    mappedY = (LCD_HEIGHT - 1) - mappedY;
    #endif

    if (XPT2046_SWAP_XY) {
        uint16_t temp = mappedX;
        mappedX = mappedY;
        mappedY = temp;
    }

    if (x) *x = mappedX;
    if (y) *y = mappedY;

    return true; // Toque detectado y coordenadas obtenidas
}