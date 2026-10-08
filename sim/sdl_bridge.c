#include <SDL2/SDL.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "sapi.h"
#include "ili9341.h"
#include "xpt2046.h"

#define DISPLAY_WIDTH   240
#define DISPLAY_HEIGHT  320
#define SCALE_FACTOR    2   // Escala para ver la pantalla 2x mas grande en la PC

static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;
static SDL_Texture *texture = NULL;
static uint32_t framebuffer[DISPLAY_WIDTH * DISPLAY_HEIGHT];

// Variables para simular el tactil
static bool sim_touch_pressed = false;
static uint16_t sim_touch_x = 0;
static uint16_t sim_touch_y = 0;

// Variables para emular el controlador grafico ILI9341 via SPI
static uint8_t sim_dc = 0;          // 0 = Comando, 1 = Datos
static uint8_t sim_cmd = 0;         // Ultimo comando recibido
static int sim_param_count = 0;     // Contador de parametros del comando

static uint16_t win_x1 = 0, win_x2 = DISPLAY_WIDTH - 1;
static uint16_t win_y1 = 0, win_y2 = DISPLAY_HEIGHT - 1;
static uint16_t cur_x = 0, cur_y = 0;

static uint8_t msb_byte = 0;
static bool is_msb = true;

// Inicializacion de la ventana SDL2
bool sdl_init(void) {
    if (window != NULL) return true;

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("[SDL ERROR] No se pudo inicializar SDL: %s\n", SDL_GetError());
        return false;
    }

    window = SDL_CreateWindow(
        "Simulador Balanza Nutricional (ILI9341 + XPT2046)",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        DISPLAY_WIDTH * SCALE_FACTOR,
        DISPLAY_HEIGHT * SCALE_FACTOR,
        SDL_WINDOW_SHOWN
    );

    if (!window) {
        printf("[SDL ERROR] No se pudo crear la ventana: %s\n", SDL_GetError());
        return false;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer) {
        renderer = SDL_CreateRenderer(window, -1, 0);
    }

    texture = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_ARGB8888,
        SDL_TEXTUREACCESS_STREAMING,
        DISPLAY_WIDTH,
        DISPLAY_HEIGHT
    );

    for (int i = 0; i < DISPLAY_WIDTH * DISPLAY_HEIGHT; i++) {
        framebuffer[i] = 0xFF000000;
    }

    printf("[SIMULADOR] Ventana de pantalla inicializada (%dx%d px)\n", DISPLAY_WIDTH * SCALE_FACTOR, DISPLAY_HEIGHT * SCALE_FACTOR);
    return true;
}

// Procesa eventos de SDL (mouse y ventana)
void sdl_poll_events(void) {
    if (!window) return;

    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            SDL_DestroyTexture(texture);
            SDL_DestroyRenderer(renderer);
            SDL_DestroyWindow(window);
            SDL_Quit();
            exit(0);
        }
        else if (event.type == SDL_MOUSEBUTTONDOWN) {
            if (event.button.button == SDL_BUTTON_LEFT) {
                sim_touch_pressed = true;
                sim_touch_x = event.button.x / SCALE_FACTOR;
                sim_touch_y = event.button.y / SCALE_FACTOR;
            }
        }
        else if (event.type == SDL_MOUSEMOTION) {
            if (sim_touch_pressed) {
                sim_touch_x = event.motion.x / SCALE_FACTOR;
                sim_touch_y = event.motion.y / SCALE_FACTOR;
            }
        }
        else if (event.type == SDL_MOUSEBUTTONUP) {
            if (event.button.button == SDL_BUTTON_LEFT) {
                sim_touch_pressed = false;
            }
        }
    }
}

// Coloca un pixel en el framebuffer de SDL2 (RGB565 -> ARGB8888)
void sdl_put_pixel(uint16_t x, uint16_t y, uint16_t color565) {
    if (x >= DISPLAY_WIDTH || y >= DISPLAY_HEIGHT) return;

    uint8_t r = (color565 >> 11) & 0x1F;
    uint8_t g = (color565 >> 5)  & 0x3F;
    uint8_t b = color565         & 0x1F;

    r = (r * 255) / 31;
    g = (g * 255) / 63;
    b = (b * 255) / 31;

    framebuffer[y * DISPLAY_WIDTH + x] = (0xFF << 24) | (r << 16) | (g << 8) | b;
}

// Refresca la pantalla en la PC
void sdl_refresh(void) {
    if (!window) return;
    sdl_poll_events();
    SDL_UpdateTexture(texture, NULL, framebuffer, DISPLAY_WIDTH * sizeof(uint32_t));
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);
}

// ============================================================================
//   EMULACION DEL BUS SPI Y PINES PARA EL CONTROLADOR ILI9341
// ============================================================================

void boardConfig(void) {
    sdl_init();
}

void gpioInit(uint8_t pin, uint8_t direction) {}

#ifndef ILI9341_DC
    #ifdef ILI9341_GPIO_DC
        #define ILI9341_DC ILI9341_GPIO_DC
    #elif defined(LCD_DC)
        #define ILI9341_DC LCD_DC
    #endif
#endif

void gpioWrite(uint8_t pin, uint8_t value) {
#ifdef ILI9341_DC
    if (pin == ILI9341_DC) {
        sim_dc = value;
    }
#else
    #ifdef ILI9341_CS
    if (pin != ILI9341_CS) {
        sim_dc = value;
    }
    #else
    sim_dc = value;
    #endif
#endif
}

bool gpioRead(uint8_t pin) {
    return false;
}

void delay(uint32_t ms) {
    sdl_refresh();
    SDL_Delay(ms);
}

void SSP1_Init(void) {}
void SSP1_change_format(uint8_t bits) {}

// Procesa cada byte transmitido por la libreria ili9341.c
uint8_t SSP1_transfer_byte(uint8_t byte) {
    if (sim_dc == 0) {
        // Es un Comando de la pantalla
        sim_cmd = byte;
        sim_param_count = 0;
        is_msb = true;
    } else {
        // Son Datos enviados a la pantalla
        switch (sim_cmd) {
            case 0x2A: // Setear ventana de columnas (X)
                if (sim_param_count == 0) win_x1 = (byte << 8);
                else if (sim_param_count == 1) { win_x1 |= byte; cur_x = win_x1; }
                else if (sim_param_count == 2) win_x2 = (byte << 8);
                else if (sim_param_count == 3) win_x2 |= byte;
                sim_param_count++;
                break;

            case 0x2B: // Setear ventana de filas (Y)
                if (sim_param_count == 0) win_y1 = (byte << 8);
                else if (sim_param_count == 1) { win_y1 |= byte; cur_y = win_y1; }
                else if (sim_param_count == 2) win_y2 = (byte << 8);
                else if (sim_param_count == 3) win_y2 |= byte;
                sim_param_count++;
                break;

            case 0x2C: // Escritura de pixeles en memoria (RAMWR)
            case 0x3C:
                if (is_msb) {
                    msb_byte = byte;
                    is_msb = false;
                } else {
                    uint16_t color565 = (msb_byte << 8) | byte;
                    sdl_put_pixel(cur_x, cur_y, color565);
                    cur_x++;
                    if (cur_x > win_x2) {
                        cur_x = win_x1;
                        cur_y++;
                        if (cur_y > win_y2) cur_y = win_y1;
                    }
                    is_msb = true;
                }
                sim_param_count++;
                break;

            default:
                break;
        }
    }
    return 0;
}

// ============================================================================
//   EMULACION DEL TACTIL XPT2046
// ============================================================================

bool XPT2046_isPress(void) {
    sdl_poll_events();
    return sim_touch_pressed;
}

bool XPT2046_getTouch(uint16_t *x, uint16_t *y) {
    sdl_poll_events();
    if (!sim_touch_pressed) return false;
    if (x) *x = sim_touch_x;
    if (y) *y = sim_touch_y;
    return true;
}

// ============================================================================
//   MOCK DE FREERTOS
// ============================================================================

static void (*saved_task)(void*) = NULL;
static void *saved_param = NULL;

void xTaskCreate(void (*task)(void*), const char *name, uint16_t stack, void *param, uint32_t priority, void *handle) {
    saved_task = task;
    saved_param = param;
}

void vTaskDelay(uint32_t ticks) {
    sdl_refresh();
    SDL_Delay(20);
}

void vTaskStartScheduler(void) {
    if (saved_task) {
        saved_task(saved_param);
    }
    while (1) {
        sdl_refresh();
        SDL_Delay(20);
    }
}