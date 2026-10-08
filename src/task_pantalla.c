#include "task_pantalla.h"

void ui_peso(float peso) {
    char buffer[20];
    ILI9341_drawRect(PESO_DIGITOS_X, PESO_DIGITOS_Y, PESO_DIGITOS_W, PESO_DIGITOS_H, ILI9341_COLOR_BLACK);
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
            ILI9341_fillRect(BTN_MENU_ALIM_X, BTN_MENU_ALIM_Y, BTN_MENU_ALIM_W, BTN_MENU_ALIM_H, ILI9341_COLOR_WHITE);
            ILI9341_drawString(BTN_MENU_ALIM_X + 10, BTN_MENU_ALIM_Y + 10, "ALIMENTOS", ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE);
            ILI9341_fillRect(BTN_NORMAL_TARA_X, BTN_NORMAL_TARA_Y, BTN_NORMAL_TARA_W, BTN_NORMAL_TARA_H, ILI9341_COLOR_WHITE);
            ILI9341_drawString(BTN_NORMAL_TARA_X + 10, BTN_NORMAL_TARA_Y + 10, "TARA", ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE);
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
    ILI9341_drawRect(PESO_BOX_X, PESO_BOX_Y, PESO_BOX_W, PESO_BOX_H, ILI9341_COLOR_WHITE);
}

void ui_marco_der(void) {
    ILI9341_drawRect(MENU_DER_X, MENU_DER_Y, MENU_DER_W, MENU_DER_H, ILI9341_COLOR_WHITE);
}

void ui_menu_derecha_seleccion_alimentos(void) {
    ILI9341_drawRect(BTN_ALIM1_X, BTN_ALIM1_Y, BTN_ALIM_W, BTN_ALIM_H, ILI9341_COLOR_WHITE);
    ILI9341_drawRect(BTN_ALIM2_X, BTN_ALIM2_Y, BTN_ALIM_W, BTN_ALIM_H, ILI9341_COLOR_WHITE);
    ILI9341_drawRect(BTN_ALIM3_X, BTN_ALIM3_Y, BTN_ALIM_W, BTN_ALIM_H, ILI9341_COLOR_WHITE);
    ILI9341_drawRect(BTN_ALIM4_X, BTN_ALIM4_Y, BTN_ALIM_W, BTN_ALIM_H, ILI9341_COLOR_WHITE);
    /*Falta implementar el arbol del menú y ahi se cambia el texto por el string que almecene el nodo*/
    ILI9341_drawString(TXT_ALIM1_X, TXT_ALIM1_Y, "ALIM 1", ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE); 
    ILI9341_drawString(TXT_ALIM2_X, TXT_ALIM2_Y, "ALIM 2", ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE);
    ILI9341_drawString(TXT_ALIM3_X, TXT_ALIM3_Y, "ALIM 3", ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE);
    ILI9341_drawString(TXT_ALIM4_X, TXT_ALIM4_Y, "ALIM 4", ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE);
}

void ui_menu_derecha_pesaje_normal(void) {
    ILI9341_fillRect(BTN_MENU_ALIM_X, BTN_MENU_ALIM_Y, BTN_MENU_ALIM_W, BTN_MENU_ALIM_H, ILI9341_COLOR_WHITE);
    ILI9341_drawString(TXT_NORMAL_ALIM_X, TXT_BOTONERA_Y, "ALIMENTOS", ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE);
    
}

void ui_menu_derecha_macros(const uint16_t macros) {
    ILI9341_drawRect(BTN_NUTRI_VOLVER_X, BTN_NUTRI_VOLVER_Y, BTN_NUTRI_VOLVER_W, BTN_NUTRI_VOLVER_H, ILI9341_COLOR_WHITE);
    ILI9341_drawString(TXT_NUTRI_VOLVER_X, TXT_BOTONERA_Y, "VOLVER", ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE);
    ILI9341_drawRect(BTN_NUTRI_TARA_X, BTN_NUTRI_TARA_Y, BTN_NUTRI_TARA_W, BTN_NUTRI_TARA_H, ILI9341_COLOR_WHITE);
    ILI9341_drawString(TXT_NUTRI_TARA_X, TXT_BOTONERA_Y, "TARA", ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE);
    ILI9341_drawRect(BTN_NUTRI_ENVIAR_X, BTN_NUTRI_ENVIAR_Y, BTN_NUTRI_ENVIAR_W, BTN_NUTRI_ENVIAR_H, ILI9341_COLOR_WHITE);
    ILI9341_drawString(TXT_NUTRI_ENVIAR_X, TXT_BOTONERA_Y, "ENVIAR", ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE);
    /*Falta implementar el arbol del menú y ahi se cambia el texto por el string que almecene el nodo*/
    ILI9341_drawString(TXT_MACRO_ETIQUETA_X, MACRO_FILA_KCAL_Y, "Kcal", ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE);
    ILI9341_drawString(TXT_MACRO_ETIQUETA_X, MACRO_FILA_PROT_Y, "Prot:", ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE);
    ILI9341_drawString(TXT_MACRO_ETIQUETA_X, MACRO_FILA_GRAS_Y, "Grasas:", ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE);
    ILI9341_drawString(TXT_MACRO_ETIQUETA_X, MACRO_FILA_CARB_Y, "Carbs:", ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE);
    /*Esto depende de como sea el struct de las macros*/
    char buffer[10];
    snprintf(buffer, sizeof(buffer), "%d", macros);
    ILI9341_drawString(TXT_MACRO_VALOR_X, MACRO_FILA_KCAL_Y, buffer, ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE);
    ILI9341_drawString(TXT_MACRO_VALOR_X, MACRO_FILA_PROT_Y, buffer, ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE);
    ILI9341_drawString(TXT_MACRO_VALOR_X, MACRO_FILA_GRAS_Y, buffer, ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE);
    ILI9341_drawString(TXT_MACRO_VALOR_X, MACRO_FILA_CARB_Y, buffer, ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE);
}


void task_pantalla(void *pvParameters) {
    EstadoUI estado_ui = UI_ESTADO_REPOSO;
    extern QueueHandle_t cola_pantalla;
    extern MensajeFSM msj;
    ILI9341_init();
    ui_pantalla_reposo();
    
    while (1) {
        if (xQueueReceive(cola_pantalla, &msj, 30) == pdTRUE) {
            switch (msj.evento) {
               case REPOSO:
                  ui_marco_izq();
                  ui_marco_der();
                  ui_dibujar_botonera(estado_ui);
                  ui_dibujar_encabezado("REPOSO");
                  ui_pantalla_pesaje_base();
                  estado_ui = UI_ESTADO_REPOSO;
                  break;
               case PESAJE_NORMAL:
                  ui_peso(msj.valor_peso);
                  estado_ui = UI_ESTADO_PESAJE_NORMAL;
                  break;
               case TARA:
                  ui_mostrar_tara();
                  estado_ui = UI_ESTADO_TARA;
                  break;
               case MODO_NUTRICIONAL:
                  ui_menu_derecha_macros(msj.valor_peso);
                  estado_ui = UI_ESTADO_MODO_NUTRICIONAL;
                  break;
               case ESTADO_ERROR:
                  ui_mostrar_error("SOBRECARGA");
                  estado_ui = UI_ESTADO_ERROR;
                  break;    
            }
        } // Espera a que se reciba un nuevo estado de la cola

    }
}