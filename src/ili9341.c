#include <ili9341.h>

void writeCommand(uint8_t cmd) {
    gpioWrite(ILI9341_DC, OFF); // Set DC low for command
    gpioWrite(ILI9341_CS, OFF); // Set CS low to select the display
    SSP1_transfer_byte(cmd);     // Send command byte   
}

void writeData(uint8_t data) {
    gpioWrite(ILI9341_DC, ON);  // Set DC high for data
    gpioWrite(ILI9341_CS, OFF); // Set CS low to select the display
    SSP1_transfer_byte(data);    // Send data byte
    gpioWrite(ILI9341_CS, ON);  // Set CS high to deselect the display
}

void ILI9341_init(void) {
    SSP1_Init(); // Initialize SSP1 peripheral
    gpioInit(ILI9341_CS, GPIO_OUTPUT);
    gpioInit(ILI9341_DC, GPIO_OUTPUT);
    gpioInit(ILI9341_RST, GPIO_OUTPUT);
    gpioWrite(ILI9341_RST, OFF); // Reset the display
    delay(10); // Wait for 10 ms    
    gpioWrite(ILI9341_RST, ON); // Release reset
    delay(120); // Wait for 120 ms
    writeCommand(0x01); // Software Reset
    delay(120); // Wait for 120 ms
    writeCommand(0x28); // Display OFF
    writeCommand(0x3A); // Pixel Format Set
    writeData(0x55);    // 16 bits per pixel
    writeCommand(0x11); // Sleep Out
    delay(120); // Wait for 120 ms
    writeCommand(0x29); // Display ON
}

void ILI9341_setAdressWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1) {
    writeCommand(0x2A); // Column Address Set
    writeData(x0 >> 8);
    writeData(x0 & 0xFF);
    writeData(x1 >> 8);
    writeData(x1 & 0xFF);

    writeCommand(0x2B); // Line Address Set
    writeData(y0 >> 8);
    writeData(y0 & 0xFF);
    writeData(y1 >> 8);
    writeData(y1 & 0xFF);

    writeCommand(0x2C); // Memory Write
    /*Now the screen is ready to receive data*/
}

void ILI9341_drawPixel(uint16_t x, uint16_t y, uint16_t color) {
    ili9341_setAdressWindow(x, y, x, y); // Set the address window to the pixel
    writeData(color >> 8); // Send high byte of color
    writeData(color & 0xFF); // Send low byte of color
}

void ILI9341_fillScreen(uint16_t color) {
    ili9341_setAdressWindow(0, 0, ILI9341_WIDTH - 1, ILI9341_HEIGHT - 1); // Set the address window to the entire screen
    for (uint32_t i = 0; i < ILI9341_WIDTH * ILI9341_HEIGHT; i++) {
        writeData(color >> 8); // Send high byte of color
        writeData(color & 0xFF); // Send low byte of color
    }
}