#include "task_pantalla.h"
#include "arbol_menu.h"

void ui_peso(float peso) {
    char buffer[20];
    ILI9341_fillRect(PESO_DIGITOS_X, PESO_DIGITOS_Y, PESO_DIGITOS_W, PESO_DIGITOS_H, ILI9341_COLOR_BLACK);
    snprintf(buffer, sizeof(buffer), "%.2f", peso);
    ILI9341_drawString(PESO_DIGITOS_X, PESO_DIGITOS_Y, buffer, ILI9341_COLOR_WHITE, ILI9341_COLOR_BLACK);
}

void ui_pantalla_reposo(void) {
    ILI9341_fillScreen(ILI9341_COLOR_BLACK);
}

void ui_pantalla_pesaje_base(float peso_actual) {
    ILI9341_fillRect(0, 0, ILI9341_WIDTH, ILI9341_HEIGHT, ILI9341_COLOR_BLACK);
    ui_marco_izq();
    ILI9341_drawString(HEADER_TEXT_X, PESO_TITULO_Y, "PESO", ILI9341_COLOR_WHITE, ILI9341_COLOR_BLACK);
    ILI9341_drawString(HEADER_TEXT_X, PESO_UNIDAD_Y, "g", ILI9341_COLOR_WHITE, ILI9341_COLOR_BLACK);
    ui_peso(peso_actual);
}

void ui_dibujar_encabezado(const char *titulo) {
    ILI9341_fillRect(HEADER_X, HEADER_Y, HEADER_W, HEADER_H, ILI9341_COLOR_WHITE);
    ILI9341_drawString(HEADER_TEXT_X, HEADER_TEXT_Y, titulo, ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE);
}

void ui_dibujar_botonera(EstadoUI estado) {
    switch (estado) {
        case UI_ESTADO_PESAJE_NORMAL:
            ILI9341_fillRect(BTN_NORMAL_ALIM_X, BTN_NORMAL_Y, BTN_NORMAL_W, BTN_NORMAL_H, ILI9341_COLOR_WHITE);
            ILI9341_drawString(TXT_NORMAL_ALIM_X, TXT_NORMAL_Y, "ALIMENTOS", ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE);

            ILI9341_fillRect(BTN_NORMAL_TARA_X, BTN_NORMAL_Y, BTN_NORMAL_W, BTN_NORMAL_H, ILI9341_COLOR_WHITE);
            ILI9341_drawString(TXT_NORMAL_TARA_X, TXT_NORMAL_Y, "TARA", ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE);

            ILI9341_fillRect(BTN_NORMAL_CALIB_X, BTN_NORMAL_Y, BTN_NORMAL_W, BTN_NORMAL_H, ILI9341_COLOR_WHITE);
            ILI9341_drawString(TXT_NORMAL_CALIB_X, TXT_NORMAL_Y, "CALIBRAR", ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE);
            break;

        case UI_ESTADO_SELECCION_ALIMENTO:
            ILI9341_fillRect(BTN_SEL_VOLVER_X, BTN_SEL_VOLVER_Y, BTN_SEL_VOLVER_W, BTN_SEL_VOLVER_H, ILI9341_COLOR_WHITE);
            ILI9341_drawString(TXT_SEL_VOLVER_X, TXT_BOTONERA_Y, "VOLVER", ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE);
            break;

        case UI_ESTADO_MODO_NUTRICIONAL:
            ILI9341_fillRect(BTN_NUTRI_VOLVER_X, BTN_NUTRI_Y, BTN_NUTRI_W, BTN_NUTRI_H, ILI9341_COLOR_WHITE);
            ILI9341_drawString(TXT_NUTRI_VOLVER_X, TXT_BOTONERA_Y, "VOLVER", ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE);

            ILI9341_fillRect(BTN_NUTRI_TARA_X, BTN_NUTRI_Y, BTN_NUTRI_W, BTN_NUTRI_H, ILI9341_COLOR_WHITE);
            ILI9341_drawString(TXT_NUTRI_TARA_X, TXT_BOTONERA_Y, "TARA", ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE);

            ILI9341_fillRect(BTN_NUTRI_ENVIAR_X, BTN_NUTRI_Y, BTN_NUTRI_W, BTN_NUTRI_H, ILI9341_COLOR_WHITE);
            ILI9341_drawString(TXT_NUTRI_ENVIAR_X, TXT_BOTONERA_Y, "ENVIAR", ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE);
            break;

        default:
            break;
    }
}

void ui_marco_izq(void) {
    ILI9341_fillRect(PESO_BOX_X, PESO_BOX_Y, PESO_BOX_W, PESO_BOX_H, ILI9341_COLOR_WHITE);
}

void ui_marco_der(void) {
    ILI9341_fillRect(MENU_DER_X, MENU_DER_Y, MENU_DER_W, MENU_DER_H, ILI9341_COLOR_WHITE);
}

void ui_menu_derecha_seleccion_alimentos(const NodoMenu_t *nodo, uint8_t offset) {
    const uint16_t bx[] = { BTN_ALIM1_X, BTN_ALIM2_X, BTN_ALIM3_X, BTN_ALIM4_X };
    const uint16_t by[] = { BTN_ALIM1_Y, BTN_ALIM2_Y, BTN_ALIM3_Y, BTN_ALIM4_Y };
    const uint16_t tx[] = { TXT_ALIM1_X, TXT_ALIM2_X, TXT_ALIM3_X, TXT_ALIM4_X };
    const uint16_t ty[] = { TXT_ALIM1_Y, TXT_ALIM2_Y, TXT_ALIM3_Y, TXT_ALIM4_Y };

    if (nodo->cant_hijos <= 4) {
        for (int i = 0; i < 4; i++) {
            if (i < nodo->cant_hijos) {
                ILI9341_fillRect(bx[i], by[i], BTN_ALIM_W, BTN_ALIM_H, ILI9341_COLOR_WHITE);
                ILI9341_drawString(tx[i], ty[i], nodo->hijos[i]->nombre, ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE);
            } else {
                ILI9341_fillRect(bx[i], by[i], BTN_ALIM_W, BTN_ALIM_H, ILI9341_COLOR_BLACK);
            }
        }
    } else {
        for (int i = 0; i < 3; i++) {
            uint8_t idx = offset + i;
            if (idx < nodo->cant_hijos) {
                ILI9341_fillRect(bx[i], by[i], BTN_ALIM_W, BTN_ALIM_H, ILI9341_COLOR_WHITE);
                ILI9341_drawString(tx[i], ty[i], nodo->hijos[idx]->nombre, ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE);
            } else {
                ILI9341_fillRect(bx[i], by[i], BTN_ALIM_W, BTN_ALIM_H, ILI9341_COLOR_BLACK);
            }
        }
        ILI9341_fillRect(bx[3], by[3], BTN_ALIM_W, BTN_ALIM_H, ILI9341_COLOR_WHITE);
        ILI9341_drawString(tx[3], ty[3], "MAS >>", ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE);
    }
}

void ui_menu_derecha_macros(informacion_alimento macros) {
    char buffer[16];
    ILI9341_fillRect(TXT_MACRO_VALOR_X, 40, 100, 140, ILI9341_COLOR_WHITE);

    snprintf(buffer, sizeof(buffer), "%.1f kcal", macros.calorias);
    ILI9341_drawString(TXT_MACRO_VALOR_X, MACRO_FILA_KCAL_Y, buffer, ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE);

    snprintf(buffer, sizeof(buffer), "%.1f g", macros.proteinas);
    ILI9341_drawString(TXT_MACRO_VALOR_X, MACRO_FILA_PROT_Y, buffer, ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE);

    snprintf(buffer, sizeof(buffer), "%.1f g", macros.grasas);
    ILI9341_drawString(TXT_MACRO_VALOR_X, MACRO_FILA_GRAS_Y, buffer, ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE);

    snprintf(buffer, sizeof(buffer), "%.1f g", macros.carbohidratos);
    ILI9341_drawString(TXT_MACRO_VALOR_X, MACRO_FILA_CARB_Y, buffer, ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE);
}

void ui_pantalla_calibracion(void) {
    ILI9341_fillScreen(ILI9341_COLOR_BLACK);
    ui_dibujar_encabezado("MODO CALIBRACION");
    ILI9341_drawString(20, 50, "Apoye el peso patron y seleccione:", ILI9341_COLOR_WHITE, ILI9341_COLOR_BLACK);

    ILI9341_fillRect(BTN_PATRON_100G_X, BTN_PATRON_Y, BTN_PATRON_W, BTN_PATRON_H, ILI9341_COLOR_WHITE);
    ILI9341_drawString(TXT_PATRON_100G_X, TXT_PATRON_Y, "100 g", ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE);

    ILI9341_fillRect(BTN_PATRON_500G_X, BTN_PATRON_Y, BTN_PATRON_W, BTN_PATRON_H, ILI9341_COLOR_WHITE);
    ILI9341_drawString(TXT_PATRON_500G_X, TXT_PATRON_Y, "500 g", ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE);

    ILI9341_fillRect(BTN_PATRON_1000G_X, BTN_PATRON_Y, BTN_PATRON_W, BTN_PATRON_H, ILI9341_COLOR_WHITE);
    ILI9341_drawString(TXT_PATRON_1000G_X, TXT_PATRON_Y, "1000 g", ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE);

    ILI9341_fillRect(BTN_CALIB_CANCELAR_X, BTN_CALIB_CANCELAR_Y, BTN_CALIB_CANCELAR_W, BTN_CALIB_CANCELAR_H, ILI9341_COLOR_WHITE);
    ILI9341_drawString(TXT_CALIB_CANCELAR_X, TXT_CALIB_CANCELAR_Y, "CANCELAR", ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE);
}

void ui_mostrar_error(const char *mensaje) {
    ILI9341_fillRect(TARA_BOX_X, TARA_BOX_Y, TARA_BOX_W, TARA_BOX_H, ILI9341_COLOR_RED);
    ILI9341_drawString(TXT_TARA_AVISO_X, TXT_TARA_AVISO_Y, mensaje, ILI9341_COLOR_WHITE, ILI9341_COLOR_RED);
}

void ui_mostrar_tara(void) {
    ILI9341_fillRect(TARA_BOX_X, TARA_BOX_Y, TARA_BOX_W, TARA_BOX_H, ILI9341_COLOR_BLUE);
    ILI9341_drawString(TXT_TARA_AVISO_X, TXT_TARA_AVISO_Y, "TARANDO...", ILI9341_COLOR_WHITE, ILI9341_COLOR_BLUE);
    ILI9341_drawString(TXT_TARA_ESPERE_X, TXT_TARA_ESPERE_Y, "Espere por favor", ILI9341_COLOR_WHITE, ILI9341_COLOR_BLUE);
}

void task_pantalla(void *pvParameters) {
    EstadoUI estado_ui = UI_ESTADO_REPOSO;
    extern QueueHandle_t cola_pantalla;
    extern QueueHandle_t cola_eventos;
    
    msj_pantalla msj_in;
    uint8_t offset_alimentos = 0;
    NodoMenu_t *nodo_actual = &arbol_raiz;
    bool tocando = false; // Bandera para monodisparo (antirrebote de toques)

    arbol_alimentos_init();
    ILI9341_init();
    ui_pantalla_reposo();

    while (1) {
        // 1. Recepción de comandos de visualización desde la FSM
        if (xQueueReceive(cola_pantalla, &msj_in, 30) == pdTRUE) {
            switch (msj_in.evento) {
                case CMD_MOSTRAR_SOLO_PESO:
                    if (estado_ui != UI_ESTADO_PESAJE_NORMAL) {
                        ui_pantalla_pesaje_base(msj_in.peso);
                        ui_dibujar_botonera(UI_ESTADO_PESAJE_NORMAL);
                        estado_ui = UI_ESTADO_PESAJE_NORMAL;
                    } else {
                        ui_peso(msj_in.peso);
                    }
                    break;

                case CMD_PEDIR_PESO:
                    ui_pantalla_calibracion();
                    estado_ui = UI_ESTADO_CALIBRACION;
                    break;

                case CMD_MOSTRAR_MACROSyPESO:
                    if (estado_ui != UI_ESTADO_MODO_NUTRICIONAL) {
                        ui_dibujar_encabezado("INFO NUTRICIONAL");
                        ui_marco_izq();
                        ui_marco_der();
                        ui_dibujar_botonera(UI_ESTADO_MODO_NUTRICIONAL);
                        estado_ui = UI_ESTADO_MODO_NUTRICIONAL;
                    }
                    ui_peso(msj_in.peso);
                    ui_menu_derecha_macros(msj_in.kcal_alimento);
                    break;

                case CMD_DIBUJAR_TARA:
                    ui_mostrar_tara();
                    estado_ui = UI_ESTADO_TARA;
                    break;

                case CMD_AHORRO:
                    ui_pantalla_reposo();
                    estado_ui = UI_ESTADO_REPOSO;
                    break;

                case CMD_ERROR:
                    ui_mostrar_error("SOBRECARGA");
                    estado_ui = UI_ESTADO_ERROR;
                    break;

                case CMD_DIBUJAR_EXITO_AHORRO:
                    ui_dibujar_encabezado("CALIBRADO OK");
                    break;

                default:
                    break;
            }
        }

        // 2. Comprobación y gestión táctil con filtro monodisparo
        if (XPT2046_isPress()) {
            if (!tocando) {
                tocando = true; // Registramos que la pantalla empezó a presionarse
                uint16_t x, y;
                XPT2046_getTouch(&x, &y);

                switch (estado_ui) {
                    case UI_ESTADO_REPOSO:
                        xQueueSend(cola_eventos, &(MensajeFSM){.evento = EV_TOQUE_PANTALLA}, 0);
                        break;

                    case UI_ESTADO_PESAJE_NORMAL:
                        if (y >= BTN_NORMAL_Y && y <= (BTN_NORMAL_Y + BTN_NORMAL_H)) {
                            if (x >= BTN_NORMAL_ALIM_X && x <= (BTN_NORMAL_ALIM_X + BTN_NORMAL_W)) {
                                estado_ui = UI_ESTADO_SELECCION_ALIMENTO;
                                offset_alimentos = 0;
                                nodo_actual = &arbol_raiz;
                                ui_dibujar_encabezado(nodo_actual->nombre);
                                ui_menu_derecha_seleccion_alimentos(nodo_actual, offset_alimentos);
                                ui_dibujar_botonera(UI_ESTADO_SELECCION_ALIMENTO);
                            } else if (x >= BTN_NORMAL_TARA_X && x <= (BTN_NORMAL_TARA_X + BTN_NORMAL_W)) {
                                xQueueSend(cola_eventos, &(MensajeFSM){.evento = EV_TARA}, 0);
                            } else if (x >= BTN_NORMAL_CALIB_X && x <= (BTN_NORMAL_CALIB_X + BTN_NORMAL_W)) {
                                xQueueSend(cola_eventos, &(MensajeFSM){.evento = EV_INICIAR_CALIBRACION}, 0);
                            }
                        }
                        break;

                    case UI_ESTADO_SELECCION_ALIMENTO:
                        // Botón inferior "VOLVER"
                        if (x >= BTN_SEL_VOLVER_X && x <= (BTN_SEL_VOLVER_X + BTN_SEL_VOLVER_W) &&
                            y >= BTN_SEL_VOLVER_Y && y <= (BTN_SEL_VOLVER_Y + BTN_SEL_VOLVER_H)) {

                            offset_alimentos = 0;
                            if (nodo_actual->padre != NULL) {
                                nodo_actual = nodo_actual->padre;
                                ui_dibujar_encabezado(nodo_actual->nombre);
                                ui_menu_derecha_seleccion_alimentos(nodo_actual, offset_alimentos);
                            } else {
                                estado_ui = UI_ESTADO_PESAJE_NORMAL;
                                ui_pantalla_pesaje_base(msj_in.peso);
                                ui_dibujar_botonera(UI_ESTADO_PESAJE_NORMAL);
                                xQueueSend(cola_eventos, &(MensajeFSM){.evento = EV_VOLVER}, 0);
                            }
                        }
                        // Botón 1
                        else if (x >= BTN_ALIM1_X && x <= (BTN_ALIM1_X + BTN_ALIM_W) &&
                                 y >= BTN_ALIM1_Y && y <= (BTN_ALIM1_Y + BTN_ALIM_H)) {
                            uint8_t idx = (nodo_actual->cant_hijos > 4) ? (offset_alimentos + 0) : 0; /*Verifico si esta desplazado o no*/
                            if (idx < nodo_actual->cant_hijos) {
                                NodoMenu_t *elegido = nodo_actual->hijos[idx];
                                if (elegido->es_hoja) {
                                    xQueueSend(cola_eventos, &(MensajeFSM){.evento = EV_ALIMENTO, .id_alimento = elegido->id_alimento}, 0);
                                    nodo_actual = &arbol_raiz;
                                    offset_alimentos = 0;
                                } else {
                                    nodo_actual = elegido;
                                    offset_alimentos = 0;
                                    ui_dibujar_encabezado(nodo_actual->nombre);
                                    ui_menu_derecha_seleccion_alimentos(nodo_actual, offset_alimentos);
                                }
                            }
                        }
                        // Botón 2
                        else if (x >= BTN_ALIM2_X && x <= (BTN_ALIM2_X + BTN_ALIM_W) &&
                                 y >= BTN_ALIM2_Y && y <= (BTN_ALIM2_Y + BTN_ALIM_H)) {
                            uint8_t idx = (nodo_actual->cant_hijos > 4) ? (offset_alimentos + 1) : 1;
                            if (idx < nodo_actual->cant_hijos) {
                                NodoMenu_t *elegido = nodo_actual->hijos[idx];
                                if (elegido->es_hoja) {
                                    xQueueSend(cola_eventos, &(MensajeFSM){.evento = EV_ALIMENTO, .id_alimento = elegido->id_alimento}, 0);
                                    nodo_actual = &arbol_raiz;
                                    offset_alimentos = 0;
                                } else {
                                    nodo_actual = elegido;
                                    offset_alimentos = 0;
                                    ui_dibujar_encabezado(nodo_actual->nombre);
                                    ui_menu_derecha_seleccion_alimentos(nodo_actual, offset_alimentos);
                                }
                            }
                        }
                        // Botón 3
                        else if (x >= BTN_ALIM3_X && x <= (BTN_ALIM3_X + BTN_ALIM_W) &&
                                 y >= BTN_ALIM3_Y && y <= (BTN_ALIM3_Y + BTN_ALIM_H)) {
                            uint8_t idx = (nodo_actual->cant_hijos > 4) ? (offset_alimentos + 2) : 2;
                            if (idx < nodo_actual->cant_hijos) {
                                NodoMenu_t *elegido = nodo_actual->hijos[idx];
                                if (elegido->es_hoja) {
                                    xQueueSend(cola_eventos, &(MensajeFSM){.evento = EV_ALIMENTO, .id_alimento = elegido->id_alimento}, 0);
                                    nodo_actual = &arbol_raiz;
                                    offset_alimentos = 0;
                                } else {
                                    nodo_actual = elegido;
                                    offset_alimentos = 0;
                                    ui_dibujar_encabezado(nodo_actual->nombre);
                                    ui_menu_derecha_seleccion_alimentos(nodo_actual, offset_alimentos);
                                }
                            }
                        }
                        // Botón 4 (Alimento 4 o "MÁS >>")
                        else if (x >= BTN_ALIM4_X && x <= (BTN_ALIM4_X + BTN_ALIM_W) &&
                                 y >= BTN_ALIM4_Y && y <= (BTN_ALIM4_Y + BTN_ALIM_H)) {
                            if (nodo_actual->cant_hijos > 4) {
                                offset_alimentos += 3;
                                if (offset_alimentos >= nodo_actual->cant_hijos) {
                                    offset_alimentos = 0;
                                }
                                ui_menu_derecha_seleccion_alimentos(nodo_actual, offset_alimentos);
                            } else if (nodo_actual->cant_hijos == 4) {
                                NodoMenu_t *elegido = nodo_actual->hijos[3];
                                if (elegido->es_hoja) {
                                    xQueueSend(cola_eventos, &(MensajeFSM){.evento = EV_ALIMENTO, .id_alimento = elegido->id_alimento}, 0);
                                    nodo_actual = &arbol_raiz;
                                    offset_alimentos = 0;
                                } else {
                                    nodo_actual = elegido;
                                    offset_alimentos = 0;
                                    ui_dibujar_encabezado(nodo_actual->nombre);
                                    ui_menu_derecha_seleccion_alimentos(nodo_actual, offset_alimentos);
                                }
                            }
                        }
                        break;

                    case UI_ESTADO_MODO_NUTRICIONAL:
                        // Botonera inferior en Modo Nutricional (Y: 198 a 238)
                        if (y >= BTN_NUTRI_Y && y <= (BTN_NUTRI_Y + BTN_NUTRI_H)) {
                            if (x >= BTN_NUTRI_VOLVER_X && x <= (BTN_NUTRI_VOLVER_X + BTN_NUTRI_W)) {
                                xQueueSend(cola_eventos, &(MensajeFSM){.evento = EV_VOLVER}, 0);
                            } else if (x >= BTN_NUTRI_TARA_X && x <= (BTN_NUTRI_TARA_X + BTN_NUTRI_W)) {
                                xQueueSend(cola_eventos, &(MensajeFSM){.evento = EV_TARA}, 0);
                            } else if (x >= BTN_NUTRI_ENVIAR_X && x <= (BTN_NUTRI_ENVIAR_X + BTN_NUTRI_W)) {
                                // Evento para despachar por WiFi / MQTT al ESP32
                                xQueueSend(cola_eventos, &(MensajeFSM){.evento = EV_TIMEOUT}, 0);
                            }
                        }
                        break;

                    case UI_ESTADO_CALIBRACION:
                        if (y >= BTN_PATRON_Y && y <= (BTN_PATRON_Y + BTN_PATRON_H)) {
                            if (x >= BTN_PATRON_100G_X && x <= (BTN_PATRON_100G_X + BTN_PATRON_W)) {
                                xQueueSend(cola_eventos, &(MensajeFSM){.evento = EV_FIN_CALIBRACION, .valor_peso = 100.0f}, 0);
                            } else if (x >= BTN_PATRON_500G_X && x <= (BTN_PATRON_500G_X + BTN_PATRON_W)) {
                                xQueueSend(cola_eventos, &(MensajeFSM){.evento = EV_FIN_CALIBRACION, .valor_peso = 500.0f}, 0);
                            } else if (x >= BTN_PATRON_1000G_X && x <= (BTN_PATRON_1000G_X + BTN_PATRON_W)) {
                                xQueueSend(cola_eventos, &(MensajeFSM){.evento = EV_FIN_CALIBRACION, .valor_peso = 1000.0f}, 0);
                            }
                        } else if (x >= BTN_CALIB_CANCELAR_X && x <= (BTN_CALIB_CANCELAR_X + BTN_CALIB_CANCELAR_W) &&
                                   y >= BTN_CALIB_CANCELAR_Y && y <= (BTN_CALIB_CANCELAR_Y + BTN_CALIB_CANCELAR_H)) {
                            xQueueSend(cola_eventos, &(MensajeFSM){.evento = EV_VOLVER}, 0);
                        }
                        break;

                    case UI_ESTADO_ERROR:
                        xQueueSend(cola_eventos, &(MensajeFSM){.evento = EV_RETIRA_PESO}, 0);
                        break;

                    default:
                        break;
                }
            }
        } else {
            // El usuario levantó el dedo: habilitamos la captura del próximo toque
            tocando = false;
        }
    }
}