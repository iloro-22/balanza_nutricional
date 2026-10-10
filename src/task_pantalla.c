#include "task_pantalla.h"

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
    ui_peso(0);
}

void ui_dibujar_encabezado(const char *titulo) {
    ILI9341_fillRect(HEADER_X, HEADER_Y, HEADER_W, HEADER_H, ILI9341_COLOR_WHITE);
    ILI9341_drawString(HEADER_TEXT_X, HEADER_TEXT_Y, titulo, ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE);
}

void ui_dibujar_botonera(EstadoUI estado) {
    switch (estado) {
        case UI_ESTADO_PESAJE_NORMAL:                    
            // 1. Botón ALIMENTOS (Izquierda)
            ILI9341_fillRect(BTN_NORMAL_ALIM_X, BTN_NORMAL_Y, BTN_NORMAL_W, BTN_NORMAL_H, ILI9341_COLOR_WHITE);
            ILI9341_drawString(TXT_NORMAL_ALIM_X, TXT_NORMAL_Y, "ALIMENTOS", ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE);

            // 2. Botón TARA (Centro)
            ILI9341_fillRect(BTN_NORMAL_TARA_X, BTN_NORMAL_Y, BTN_NORMAL_W, BTN_NORMAL_H, ILI9341_COLOR_WHITE);
            ILI9341_drawString(TXT_NORMAL_TARA_X, TXT_NORMAL_Y, "TARA", ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE);

            // 3. Botón CALIBRAR (Derecha)
            ILI9341_fillRect(BTN_NORMAL_CALIB_X, BTN_NORMAL_Y, BTN_NORMAL_W, BTN_NORMAL_H, ILI9341_COLOR_WHITE);
            ILI9341_drawString(TXT_NORMAL_CALIB_X, TXT_NORMAL_Y, "CALIBRAR", ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE);
            break;

        case UI_ESTADO_SELECCION_ALIMENTO:
            ILI9341_fillRect(BTN_SEL_VOLVER_X, BTN_SEL_VOLVER_Y, BTN_SEL_VOLVER_W, BTN_SEL_VOLVER_H, ILI9341_COLOR_WHITE);
            ILI9341_drawString(BTN_SEL_VOLVER_X + 10, BTN_SEL_VOLVER_Y + 10, "VOLVER", ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE);
            break;
        case UI_ESTADO_MODO_NUTRICIONAL:
            ILI9341_fillRect(BTN_NUTRI_VOLVER_X, BTN_NUTRI_VOLVER_Y, BTN_NUTRI_VOLVER_W, BTN_NUTRI_VOLVER_H, ILI9341_COLOR_WHITE);
            ILI9341_drawString(BTN_NUTRI_VOLVER_X + 10, BTN_NUTRI_VOLVER_Y + 10, "VOLVER", ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE);
            ILI9341_fillRect(BTN_NUTRI_TARA_X, BTN_NUTRI_TARA_Y, BTN_NUTRI_TARA_W, BTN_NUTRI_TARA_H, ILI9341_COLOR_WHITE);
            ILI9341_drawString(BTN_NUTRI_TARA_X + 10, BTN_NUTRI_TARA_Y + 10, "TARA", ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE);
            ILI9341_fillRect(BTN_NUTRI_ENVIAR_X, BTN_NUTRI_ENVIAR_Y, BTN_NUTRI_ENVIAR_W, BTN_NUTRI_ENVIAR_H, ILI9341_COLOR_WHITE);
            ILI9341_drawString(BTN_NUTRI_ENVIAR_X + 10, BTN_NUTRI_ENVIAR_Y + 10, "ENVIAR", ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE);
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

void ui_menu_derecha_seleccion_alimentos(EstadoUI estado) {
    if (nodo_actual.cant_hijos > 0) {
        // Si hay hijos, dibujamos los botones correspondientes
        for (uint8_t i = 0; i < nodo_actual.cant_hijos; i++) {
            uint16_t btn_x = BTN_ALIM1_X + (i % 2) * (BTN_ALIM_W + 10); // Espaciado horizontal de 10 px
            uint16_t btn_y = BTN_ALIM1_Y + (i / 2) * (BTN_ALIM_H + 10); // Espaciado vertical de 10 px
            ILI9341_fillRect(btn_x, btn_y, BTN_ALIM_W, BTN_ALIM_H, ILI9341_COLOR_WHITE);
            ILI9341_drawString(btn_x + 10, btn_y + 20, nodo_actual.hijos[i]->nombre, ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE);
        }
    }
    else {
        // Si no hay hijos, mostramos un mensaje de error o vacío
        ILI9341_fillRect(BTN_ALIM1_X, BTN_ALIM1_Y, BTN_ALIM_W, BTN_ALIM_H, ILI9341_COLOR_WHITE);
        ILI9341_drawString(TXT_ALIM1_X, TXT_ALIM1_Y, "SIN OPCIONES", ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE);
    }
}

void ui_menu_derecha_pesaje_normal(void) {
    ILI9341_fillRect(BTN_MENU_ALIM_X, BTN_MENU_ALIM_Y, BTN_MENU_ALIM_W, BTN_MENU_ALIM_H, ILI9341_COLOR_WHITE);
    
}

void ui_menu_derecha_macros(informacion_alimento macros) {
    char buffer[16];
    ILI9341_fillRect(TXT_MACRO_VALOR_X, 40, 100, 140, ILI9341_COLOR_WHITE);

    // 2. Calorías (Kcal)
    snprintf(buffer, sizeof(buffer), "%.1f", macros.calorias);
    ILI9341_drawString(TXT_MACRO_VALOR_X, MACRO_FILA_KCAL_Y, buffer, ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE);

    // 3. Proteínas (g)
    snprintf(buffer, sizeof(buffer), "%.1f g", macros.proteinas);
    ILI9341_drawString(TXT_MACRO_VALOR_X, MACRO_FILA_PROT_Y, buffer, ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE);

    // 4. Grasas (g)
    snprintf(buffer, sizeof(buffer), "%.1f g", macros.grasas);
    ILI9341_drawString(TXT_MACRO_VALOR_X, MACRO_FILA_GRAS_Y, buffer, ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE);

    // 5. Carbohidratos (g)
    snprintf(buffer, sizeof(buffer), "%.1f g", macros.carbohidratos);
    ILI9341_drawString(TXT_MACRO_VALOR_X, MACRO_FILA_CARB_Y, buffer, ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE);
}

void ui_pantalla_calibracion(){
     // 1. Limpieza total de pantalla y encabezado
    ILI9341_fillScreen(ILI9341_COLOR_BLACK);
    ui_dibujar_encabezado("MODO CALIBRACION");
    // 2. Texto de instrucción
    ILI9341_drawString(20, 50, "Apoye el peso patron y seleccione:", ILI9341_COLOR_WHITE, ILI9341_COLOR_BLACK);
    // 3. Botón Patrón 100 g
    ILI9341_fillRect(BTN_PATRON_100G_X, BTN_PATRON_Y, BTN_PATRON_W, BTN_PATRON_H, ILI9341_COLOR_WHITE);
    ILI9341_drawString(TXT_PATRON_100G_X, TXT_PATRON_Y, "100 g", ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE);
    // 4. Botón Patrón 500 g
    ILI9341_fillRect(BTN_PATRON_500G_X, BTN_PATRON_Y, BTN_PATRON_W, BTN_PATRON_H, ILI9341_COLOR_WHITE);
    ILI9341_drawString(TXT_PATRON_500G_X, TXT_PATRON_Y, "500 g", ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE);
    // 5. Botón Patrón 1000 g
    ILI9341_fillRect(BTN_PATRON_1000G_X, BTN_PATRON_Y, BTN_PATRON_W, BTN_PATRON_H, ILI9341_COLOR_WHITE);
    ILI9341_drawString(TXT_PATRON_1000G_X, TXT_PATRON_Y, "1000 g", ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE);
    // 6. Botón Inferior CANCELAR
    ILI9341_fillRect(BTN_CALIB_CANCELAR_X, BTN_CALIB_CANCELAR_Y, BTN_CALIB_CANCELAR_W, BTN_CALIB_CANCELAR_H, ILI9341_COLOR_WHITE);
    ILI9341_drawString(TXT_CALIB_CANCELAR_X, TXT_CALIB_CANCELAR_Y, "CANCELAR", ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE);
}

void task_pantalla(void *pvParameters) {
    EstadoUI estado_ui = UI_ESTADO_REPOSO;
    extern QueueHandle_t cola_pantalla;
    extern QueueHandle_t cola_eventos;
    MensajeFSM msj;
    msj_pantalla msj_in;
    uint8_t offset_alimentos;
    arbol_alimentos_init();
    ILI9341_init();
    ui_pantalla_reposo();
    while (1) {
        if (xQueueReceive(cola_pantalla, &msj_in, 30) == pdTRUE) {
            switch (msj_in.evento) {
                case CMD_MOSTRAR_SOLO_PESO:
                    // Si venimos de otra pantalla (ej. Reposo, Menú o Calibración), armamos la escena
                    if (estado_ui != UI_ESTADO_PESAJE_NORMAL) {
                        ui_pantalla_pesaje_base(msj_in.peso);
                        ui_dibujar_botonera(UI_ESTADO_PESAJE_NORMAL);
                        estado_ui = UI_ESTADO_PESAJE_NORMAL;
                    } else {
                        // Si ya estábamos en pesaje normal, SOLO refrescamos los dígitos
                        ui_peso(msj_in.peso);
                    }
                    break;

                case CMD_PEDIR_PESO:
                    // Entramos al modo de calibración 
                    ui_pantalla_calibracion();
                    estado_ui = UI_ESTADO_CALIBRACION;
                break;

                case CMD_MOSTRAR_MACROSyPESO:
                    // Si recién entramos al modo nutricional, armamos el marco y la botonera
                    if (estado_ui != UI_ESTADO_MODO_NUTRICIONAL) {
                        ui_dibujar_encabezado("INFO NUTRICIONAL");
                        ui_marco_izq();
                        ui_marco_der();
                        ui_dibujar_botonera(UI_ESTADO_MODO_NUTRICIONAL);
                        estado_ui = UI_ESTADO_MODO_NUTRICIONAL;
                    }
                    // Actualizamos el visor de peso y la tabla de macronutrientes calculados
                    ui_peso(msj_in.peso);
                    ui_menu_derecha_macros(msj_in.kcal_alimento);
                    break;

                case CMD_DIBUJAR_TARA:
                    if (estado_ui == UI_ESTADO_TARA) {
                        estado_ui = UI_ESTADO_PESAJE_NORMAL;
                    }
                    ui_dibujar_encabezado("TARANDO...");
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

                case CMD_PEDIR_PESO:
                    // Pantalla de calibración: dibuja los botones de 100g, 500g, 1000g
                    ui_dibujar_encabezado("CALIBRACION");
                    // Acá podés llamar a una función que dibuje los 3 botones de calibración
                    // ui_dibujar_botones_calibracion();
                    break;

                case CMD_DIBUJAR_EXITO_AHORRO:
                    // Muestra mensaje de calibración exitosa antes de apagarse
                    ui_dibujar_encabezado("CALIBRADO OK");
                    break;

                default:
                    break;
            }
        }
        if (XPT2046_isPress()) {
            uint16_t x, y;
            switch (estado_ui)
            {
            case UI_ESTADO_REPOSO:
                xQueueSend(cola_eventos, &(MensajeFSM){.evento = EV_TOQUE_PANTALLA}, 0);
                break;
            case UI_ESTADO_PESAJE_NORMAL:
                XPT2046_getTouch(&x, &y);
                if (x >= BTN_NORMAL_ALIM_X && x <= (BTN_NORMAL_ALIM_X + BTN_NORMAL_W) &&
                    y >= BTN_NORMAL_Y && y <= (BTN_NORMAL_Y + BTN_NORMAL_H)) {
                    estado_ui = UI_ESTADO_SELECCION_ALIMENTO;
                    offset_alimentos=0;
                } else if (x >= BTN_NORMAL_TARA_X && x <= (BTN_NORMAL_TARA_X + BTN_NORMAL_W) &&
                           y >= BTN_NORMAL_Y && y <= (BTN_NORMAL_Y + BTN_NORMAL_H)) {
                    xQueueSend(cola_eventos, &(MensajeFSM){.evento = EV_TARA}, 0);
                } else if (x >= BTN_NORMAL_CALIB_X && x <= (BTN_NORMAL_CALIB_X + BTN_NORMAL_W) &&
                           y >= BTN_NORMAL_Y && y <= (BTN_NORMAL_Y + BTN_NORMAL_H)) {
                    xQueueSend(cola_eventos, &(MensajeFSM){.evento = EV_INICIAR_CALIBRACION}, 0);
                }
                break;
            case UI_ESTADO_SELECCION_ALIMENTO:
                
                XPT2046_getTouch(&x, &y);
                if (x >= BTN_SEL_VOLVER_X && x <= (BTN_SEL_VOLVER_X + BTN_SEL_VOLVER_W) &&
                    y >= BTN_SEL_VOLVER_Y && y <= (BTN_SEL_VOLVER_Y + BTN_SEL_VOLVER_H)) {
                    if (nodo_actual.padre != NULL) {
                        nodo_actual = nodo_actual.padre;
                        offset_alimentos=0;
                        ui_menu_derecha_seleccion_alimentos(estado_ui);
                    } else {
                        estado_ui = UI_ESTADO_PESAJE_NORMAL;
                        ILI9341_fillRect(0, 0, ILI9341_WIDTH, ILI9341_HEIGHT, ILI9341_COLOR_BLACK);
                        ui_pantalla_pesaje_base(msj_in.peso);
                        ui_dibujar_botonera(UI_ESTADO_PESAJE_NORMAL);
                        xQueueSend(cola_eventos, &(MensajeFSM){.evento = EV_VOLVER}, 0);
                    }
                }
                if (x >= BTN_ALIM1_X && x <= (BTN_ALIM1_X + BTN_ALIM_W) &&
                    y >= BTN_ALIM1_Y && y <= (BTN_ALIM1_Y + BTN_ALIM_H)) {
                    if (nodo_actual.hijos[0] != NULL) {
                        nodo_actual = *(nodo_actual.hijos[0]);
                        ui_menu_derecha_seleccion_alimentos(estado_ui);
                        offset_alimentos=0;
                    }
                    else {
                        // Si es un nodo hoja, enviamos el evento con el ID del alimento seleccionado
                        xQueueSend(cola_eventos, &(MensajeFSM){.evento = EV_ALIMENTO, .id_alimento = nodo_actual.id_alimento}, 0);
                        estado_ui = UI_ESTADO_MODO_NUTRICIONAL;
                    }
                }
                }
                else if (x >= BTN_ALIM2_X && x <= (BTN_ALIM2_X + BTN_ALIM_W) &&
                           y >= BTN_ALIM2_Y && y <= (BTN_ALIM2_Y + BTN_ALIM_H)) {
                    if(nodo_actual.hijos[1] != NULL) {
                        nodo_actual = *(nodo_actual.hijos[1]);
                        ui_menu_derecha_seleccion_alimentos(estado_ui);
                    }
                    else {
                        // Si es un nodo hoja, enviamos el evento con el ID del alimento seleccionado
                        xQueueSend(cola_eventos, &(MensajeFSM){.evento = EV_ALIMENTO, .id_alimento = nodo_actual.id_alimento}, 0);
                        estado_ui = UI_ESTADO_MODO_NUTRICIONAL;
                    }
                } else if (x >= BTN_ALIM3_X && x <= (BTN_ALIM3_X + BTN_ALIM_W) &&
                           y >= BTN_ALIM3_Y && y <= (BTN_ALIM3_Y + BTN_ALIM_H)) {
                    if(nodo_actual.hijos[2] != NULL) {
                        nodo_actual = *(nodo_actual.hijos[2]);
                        ui_menu_derecha_seleccion_alimentos(estado_ui);
                    }
                    else {
                        // Si es un nodo hoja, enviamos el evento con el ID del alimento seleccionado
                        xQueueSend(cola_eventos, &(MensajeFSM){.evento = EV_ALIMENTO, .id_alimento = nodo_actual.id_alimento}, 0);
                        estado_ui = UI_ESTADO_MODO_NUTRICIONAL;
                    }
                } else if (x >= BTN_ALIM4_X && x <= (BTN_ALIM4_X + BTN_ALIM_W) &&
                           y >= BTN_ALIM4_Y && y <= (BTN_ALIM4_Y + BTN_ALIM_H)) {
                    if (nodo_actual.hijos[3] != NULL) {
                        nodo_actual = *(nodo_actual.hijos[3]);
                        ui_menu_derecha_seleccion_alimentos(estado_ui);
                    }
                    else if {
                        // Si es un nodo hoja, enviamos el evento con el ID del alimento seleccionado
                        xQueueSend(cola_eventos, &(MensajeFSM){.evento = EV_ALIMENTO, .id_alimento = nodo_actual.id_alimento}, 0);
                        estado_ui = UI_ESTADO_MODO_NUTRICIONAL;
                    }
                    else if(nodo_actual.cant_hijos > 4) {
                        offset_alimentos+=3;


                    }
                }

                break;
            case UI_ESTADO_CALIBRACION:
                XPT2046_getTouch(&x, &y);
                if (x >= BTN_PATRON_100G_X && x <= (BTN_PATRON_100G_X + BTN_PATRON_W) &&
                    y >= BTN_PATRON_Y && y <= (BTN_PATRON_Y + BTN_PATRON_H)) {
                    xQueueSend(cola_eventos, &(MensajeFSM){.evento = EV_BOTON_100G}, 0);
                } else if (x >= BTN_PATRON_500G_X && x <= (BTN_PATRON_500G_X + BTN_PATRON_W) &&
                           y >= BTN_PATRON_Y && y <= (BTN_PATRON_Y + BTN_PATRON_H)) {
                    xQueueSend(cola_eventos, &(MensajeFSM){.evento = EV_BOTON_500G}, 0);
                } else if (x >= BTN_PATRON_1000G_X && x <= (BTN_PATRON_1000G_X + BTN_PATRON_W) &&
                           y >= BTN_PATRON_Y && y <= (BTN_PATRON_Y + BTN_PATRON_H)) {
                    xQueueSend(cola_eventos, &(MensajeFSM){.evento = EV_BOTON_1000G}, 0);
                } else if (x >= BTN_CALIB_CANCELAR_X && x <= (BTN_CALIB_CANCELAR_X + BTN_CALIB_CANCELAR_W) &&
                           y >= BTN_CALIB_CANCELAR_Y && y <= (BTN_CALIB_CANCELAR_Y + BTN_CALIB_CANCELAR_H)) {
                    xQueueSend(cola_eventos, &(MensajeFSM){.evento = EV_VOLVER}, 0);
                }
                break;
            case UI_ESTADO_ERROR:
                XPT2046_getTouch(&x, &y);
                // Si el usuario toca la pantalla en modo error, enviamos un evento para volver al estado de reposo
                xQueueSend(cola_eventos, &(MensajeFSM){.evento = EV_RETIRA_PESO}, 0);
                break;
            
            
            default:
                break;
            }
        }
    }
}