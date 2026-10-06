#include <SDL2/SDL.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "sapi.h"
#include "ssp1.h"
#include "FreeRTOS.h"
#include "task.h"

#define SCREEN_WIDTH  240
#define SCREEN_HEIGHT 320
#define SCALE_FACTOR  2  // Escala x2 (480x640 px) para fácil lectura en PC

static uint32_t framebuffer[SCREEN_HEIGHT][SCREEN_WIDTH];
static pthread_mutex_t fb_mutex = PTHREAD_MUTEX_INITIALIZER;

// Estado del ILI9341 simulado
static uint8_t dc_pin_state = OFF;
static uint8_t current_cmd = 0;
static uint8_t cmd_byte_idx = 0;

static uint16_t window_x0 = 0, window_x1 = SCREEN_WIDTH - 1;
static uint16_t window_y0 = 0, window_y1 = SCREEN_HEIGHT - 1;
static uint16_t curr_x = 0, curr_y = 0;

static uint8_t color_high_byte = 0;
static bool waiting_low_byte = false;

// Mock de sAPI
void boardConfig(void) {
    printf("[SIM] Inicializando EDU-CIAA (Modo Simulador SDL2)...\n");
}

void gpioInit(uint8_t pin, uint8_t direction) {}

void gpioWrite(uint8_t pin, uint8_t value) {
    if (pin == GPIO1) { // ILI9341_DC
        dc_pin_state = value;
    }
}

void delay(uint32_t duration_ms) {
    usleep(duration_ms * 1000);
}

// Mock de SSP1
void SSP1_Init(void) {
    printf("[SIM] Bus SPI (SSP1) Inicializado.\n");
}

uint8_t SSP1_transfer_byte(uint8_t byte) {
    if (dc_pin_state == OFF) {
        // Es un COMANDO
        current_cmd = byte;
        cmd_byte_idx = 0;
        waiting_low_byte = false;

        if (current_cmd == 0x2C) { // RAMWR
            curr_x = window_x0;
            curr_y = window_y0;
        }
    } else {
        // Es un DATO
        switch (current_cmd) {
            case 0x2A: // CASET
                if (cmd_byte_idx == 0) window_x0 = (window_x0 & 0x00FF) | (byte << 8);
                else if (cmd_byte_idx == 1) window_x0 = (window_x0 & 0xFF00) | byte;
                else if (cmd_byte_idx == 2) window_x1 = (window_x1 & 0x00FF) | (byte << 8);
                else if (cmd_byte_idx == 3) window_x1 = (window_x1 & 0xFF00) | byte;
                cmd_byte_idx++;
                break;

            case 0x2B: // PASET
                if (cmd_byte_idx == 0) window_y0 = (window_y0 & 0x00FF) | (byte << 8);
                else if (cmd_byte_idx == 1) window_y0 = (window_y0 & 0xFF00) | byte;
                else if (cmd_byte_idx == 2) window_y1 = (window_y1 & 0x00FF) | (byte << 8);
                else if (cmd_byte_idx == 3) window_y1 = (window_y1 & 0xFF00) | byte;
                cmd_byte_idx++;
                break;

            case 0x2C: // RAMWR (Escritura de Píxeles)
                if (!waiting_low_byte) {
                    color_high_byte = byte;
                    waiting_low_byte = true;
                } else {
                    uint8_t color_low_byte = byte;
                    waiting_low_byte = false;

                    uint16_t rgb565 = (color_high_byte << 8) | color_low_byte;

                    // Convertir RGB565 a RGB888 (ARGB32 para SDL)
                    uint8_t r = ((rgb565 >> 11) & 0x1F) * 255 / 31;
                    uint8_t g = ((rgb565 >> 5)  & 0x3F) * 255 / 63;
                    uint8_t b = (rgb565 & 0x1F)        * 255 / 31;

                    uint32_t pixel32 = (0xFF << 24) | (r << 16) | (g << 8) | b;

                    pthread_mutex_lock(&fb_mutex);
                    if (curr_x < SCREEN_WIDTH && curr_y < SCREEN_HEIGHT) {
                        framebuffer[curr_y][curr_x] = pixel32;
                    }
                    pthread_mutex_unlock(&fb_mutex);

                    curr_x++;
                    if (curr_x > window_x1) {
                        curr_x = window_x0;
                        curr_y++;
                        if (curr_y > window_y1) {
                            curr_y = window_y0;
                        }
                    }
                }
                break;

            default:
                break;
        }
    }
    return 0;
}

// Mock de FreeRTOS
void vTaskDelay(TickType_t xTicksToDelay) {
    usleep(xTicksToDelay * 1000);
}

typedef struct {
    TaskFunction_t func;
    void *param;
} TaskWrapperArgs;

static void* task_pthread_wrapper(void* arg) {
    TaskWrapperArgs *targs = (TaskWrapperArgs*)arg;
    targs->func(targs->param);
    free(targs);
    return NULL;
}

int xTaskCreate(TaskFunction_t pvTaskCode, const char * const pcName, uint16_t usStackDepth, void *pvParameters, uint32_t uxPriority, TaskHandle_t *pxCreatedTask) {
    pthread_t thread;
    TaskWrapperArgs *targs = malloc(sizeof(TaskWrapperArgs));
    targs->func = pvTaskCode;
    targs->param = pvParameters;

    if (pthread_create(&thread, NULL, task_pthread_wrapper, targs) != 0) {
        printf("[SIM ERROR] Error al crear hilo para tarea: %s\n", pcName);
        return 0;
    }
    printf("[SIM] Tarea FreeRTOS iniciada en segundo plano: '%s'\n", pcName);
    return 1;
}

void vTaskStartScheduler(void) {
    printf("[SIM] Iniciando FreeRTOS Scheduler (Ventana SDL2 active)...\n");

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("[SIM ERROR] Error al inicializar SDL2: %s\n", SDL_GetError());
        return;
    }

    SDL_Window *window = SDL_CreateWindow(
        "Balanza Nutricional - Simulador ILI9341",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        SCREEN_WIDTH * SCALE_FACTOR,
        SCREEN_HEIGHT * SCALE_FACTOR,
        SDL_WINDOW_SHOWN
    );

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL_Texture *texture = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_ARGB8888,
        SDL_TEXTUREACCESS_STREAMING,
        SCREEN_WIDTH,
        SCREEN_HEIGHT
    );

    bool running = true;
    SDL_Event event;

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
        }

        pthread_mutex_lock(&fb_mutex);
        SDL_UpdateTexture(texture, NULL, framebuffer, SCREEN_WIDTH * sizeof(uint32_t));
        pthread_mutex_unlock(&fb_mutex);

        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, texture, NULL, NULL);
        SDL_RenderPresent(renderer);

        SDL_Delay(16); // ~60 FPS
    }

    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    printf("[SIM] Simulador cerrado.\n");
    exit(0);
}