#include "ili9341.h"
#include "xpt2046.h"

#define BTN_X 60
#define BTN_Y 90
#define BTN_W 200
#define BTN_H 60

int main(void) {
    uint16_t touch_x = 0;
    uint16_t touch_y = 0;
    uint8_t estado_color = 0;

    ILI9341_init();
    XPT2046_init();
    ILI9341_fillScreen(ILI9341_BLACK);
    ILI9341_fillRect(BTN_X, BTN_Y, BTN_W, BTN_H, ILI9341_WHITE);
    ILI9341_drawString(BTN_X + 25, BTN_Y + 20, "CAMBIAR COLOR", ILI9341_BLACK, ILI9341_WHITE);

    while (1) {
        if (XPT2046_isPress()) {
            XPT2046_getTouch(&touch_x, &touch_y);

            if (touch_x >= BTN_X && touch_x <= (BTN_X + BTN_W) &&
                touch_y >= BTN_Y && touch_y <= (BTN_Y + BTN_H)) {
                estado_color = !estado_color;
                ILI9341_fillScreen(estado_color ? ILI9341_BLUE : ILI9341_BLACK);
                ILI9341_fillRect(BTN_X, BTN_Y, BTN_W, BTN_H, ILI9341_WHITE);
                ILI9341_drawString(BTN_X + 25, BTN_Y + 20, "CAMBIAR COLOR", ILI9341_BLACK, ILI9341_WHITE);

                while (XPT2046_isPress()) {
                }
            }
        }
    }

    return 0;
}
