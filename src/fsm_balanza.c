#include "fsm_balanza.h"
#include "FreeRTOS.h"
#include "queue.h"
#include "task.h"

extern TaskHandle_t handle_sensor_peso;
extern QueueHandle_t cola_eventos;
extern QueueHandle_t cola_pantalla;

static void enviar_comando_pantalla(ComandoPantalla_t evento,
                                    float peso,
                                    const informacion_alimento *macros)
{
    msj_pantalla mensaje = {0};
    mensaje.evento = evento;
    mensaje.peso = peso;

    if (macros != NULL) {
        mensaje.kcal_alimento = *macros;
    }

    xQueueSend(cola_pantalla, &mensaje, portMAX_DELAY);
}

static uint16_t peso_patron_calibracion(const MensajeFSM *mensaje)
{
    switch (mensaje->evento) {
        case EV_BOTON_100G:
            return 100;
        case EV_BOTON_500G:
            return 500;
        case EV_BOTON_1000G:
            return 1000;
        case EV_FIN_CALIBRACION:
            return (uint16_t)mensaje->valor_peso;
        default:
            return 0;
    }
}

void task_fsm(void *taskParmPtr)
{
    (void)taskParmPtr;

    MensajeFSM mensaje;
    EstadoBalanza estado_actual = REPOSO;
    uint8_t id_alimento = 0;
    informacion_alimento macros = {0};

    for (;;) {
        if (xQueueReceive(cola_eventos, &mensaje, portMAX_DELAY) != pdTRUE) {
            continue;
        }

        if (mensaje.evento == EV_SOBRECARGA) {
            estado_actual = ESTADO_ERROR;
        }

        switch (estado_actual) {
            case REPOSO:
                if (mensaje.evento == EV_TOQUE_PANTALLA) {
                    enviar_comando_pantalla(CMD_MOSTRAR_SOLO_PESO,
                                            mensaje.valor_peso, NULL);
                    vTaskResume(handle_sensor_peso);
                    estado_actual = PESAJE_NORMAL;
                }
                break;

            case PESAJE_NORMAL:
                if (mensaje.evento == EV_CAMBIO_PESO) {
                    enviar_comando_pantalla(CMD_MOSTRAR_SOLO_PESO,
                                            mensaje.valor_peso, NULL);
                } else if (mensaje.evento == EV_ALIMENTO) {
                    id_alimento = mensaje.id_alimento;
                    estado_actual = MODO_NUTRICIONAL;
                    macros = calcular_macros(id_alimento, mensaje.valor_peso);
                    enviar_comando_pantalla(CMD_MOSTRAR_MACROSyPESO,
                                            mensaje.valor_peso, &macros);
                } else if (mensaje.evento == EV_TARA) {
                    hx711_tara();
                    macros = (informacion_alimento){0};
                    enviar_comando_pantalla(CMD_MOSTRAR_SOLO_PESO, 0.0f,
                                            &macros);
                } else if (mensaje.evento == EV_TIMEOUT) {
                    enviar_comando_pantalla(CMD_AHORRO, 0.0f, NULL);
                    vTaskSuspend(handle_sensor_peso);
                    estado_actual = REPOSO;
                } else if (mensaje.evento == EV_INICIAR_CALIBRACION) {
                    estado_actual = CALIBRACION;
                    enviar_comando_pantalla(CMD_DIBUJAR_TARA, 0.0f, NULL);
                    vTaskDelay(pdMS_TO_TICKS(2000));
                    hx711_tara();
                    enviar_comando_pantalla(CMD_PEDIR_PESO, 0.0f, NULL);
                }
                break;

            case MODO_NUTRICIONAL:
                if (mensaje.evento == EV_CAMBIO_PESO ||
                    mensaje.evento == EV_ALIMENTO) {
                    if (mensaje.evento == EV_ALIMENTO) {
                        id_alimento = mensaje.id_alimento;
                    }
                    macros = calcular_macros(id_alimento, mensaje.valor_peso);
                    enviar_comando_pantalla(CMD_MOSTRAR_MACROSyPESO,
                                            mensaje.valor_peso, &macros);
                    esp_enviar_macros(id_alimento, mensaje.valor_peso, macros);
                } else if (mensaje.evento == EV_TARA) {
                    hx711_tara();
                    enviar_comando_pantalla(CMD_MOSTRAR_MACROSyPESO, 0.0f,
                                            &macros);
                } else if (mensaje.evento == EV_VOLVER) {
                    estado_actual = PESAJE_NORMAL;
                } else if (mensaje.evento == EV_TIMEOUT) {
                    enviar_comando_pantalla(CMD_AHORRO, 0.0f, NULL);
                    vTaskSuspend(handle_sensor_peso);
                    estado_actual = REPOSO;
                } else if (mensaje.evento == EV_INICIAR_CALIBRACION) {
                    estado_actual = CALIBRACION;
                    enviar_comando_pantalla(CMD_DIBUJAR_TARA, 0.0f, NULL);
                    vTaskDelay(pdMS_TO_TICKS(2000));
                    hx711_tara();
                    enviar_comando_pantalla(CMD_PEDIR_PESO, 0.0f, NULL);
                }
                break;

            case CALIBRACION:
                if (mensaje.evento == EV_FIN_CALIBRACION ||
                    mensaje.evento == EV_BOTON_100G ||
                    mensaje.evento == EV_BOTON_500G ||
                    mensaje.evento == EV_BOTON_1000G) {
                    uint16_t peso_patron = peso_patron_calibracion(&mensaje);
                    if (peso_patron > 0) {
                        hx711_CalibrarEscala(peso_patron);
                        enviar_comando_pantalla(CMD_DIBUJAR_EXITO_AHORRO,
                                                0.0f, NULL);
                        estado_actual = PESAJE_NORMAL;
                    }
                } else if (mensaje.evento == EV_VOLVER) {
                    estado_actual = PESAJE_NORMAL;
                    enviar_comando_pantalla(CMD_MOSTRAR_SOLO_PESO, 0.0f, NULL);
                }
                break;

            case ESTADO_ERROR:
                enviar_comando_pantalla(CMD_ERROR, 0.0f, NULL);
                if (mensaje.evento == EV_RETIRA_PESO) {
                    enviar_comando_pantalla(CMD_AHORRO, 0.0f, NULL);
                    estado_actual = REPOSO;
                }
                break;

            case TARA:
                break;
        }
    }
}
