#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "nvs_flash.h"
#include "lwip/sockets.h"
#include "esp_log.h"

static const char *TAG = "WIFI_MANAGER";

// Variables globales para credenciales
char ssid_guardado[32] = {0};
char pass_guardado[64] = {0};

const char *html_page = 
    "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n"
    "<!DOCTYPE html><html><body>"
    "<h2>Configurar Wi-Fi ESP32</h2>"
    "<form action=\"/guardar\" method=\"GET\">"
    "SSID: <input type=\"text\" name=\"ssid\"><br><br>"
    "Clave: <input type=\"text\" name=\"password\"><br><br>"
    "<input type=\"submit\" value=\"Guardar y Reiniciar\">"
    "</form></body></html>";

const char *html_ok = 
    "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\n\r\n"
    "Configuracion guardada. Reiniciando el micro...";

void reemplazar_mas_por_espacios(char *str) {
    for (int i = 0; str[i]; i++) {
        if (str[i] == '+') {
            str[i] = ' ';
        }
    }
}

// Reemplaza tu módulo read_and_save_credentials usando NVS (Memoria Flash del ESP32)
void leer_credenciales() {
    nvs_handle_t my_handle;
    if (nvs_open("storage", NVS_READONLY, &my_handle) == ESP_OK) {
        size_t required_size = sizeof(ssid_guardado);
        nvs_get_str(my_handle, "ssid", ssid_guardado, &required_size);
        required_size = sizeof(pass_guardado);
        nvs_get_str(my_handle, "password", pass_guardado, &required_size);
        nvs_close(my_handle);
    }
}

void guardar_credenciales(const char* nuevo_ssid, const char* nueva_pass) {
    nvs_handle_t my_handle;
    if (nvs_open("storage", NVS_READWRITE, &my_handle) == ESP_OK) {
        nvs_set_str(my_handle, "ssid", nuevo_ssid);
        nvs_set_str(my_handle, "password", nueva_pass);
        nvs_commit(my_handle);
        nvs_close(my_handle);
    }
}

// Tarea del servidor web TCP (Equivalente a tu wifi_manager.py)
void tarea_servidor_web(void *pvParameters) {
    int listen_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
    
    // SO_REUSEADDR para evitar bloqueos si el puerto 80 quedó colgado
    int opt = 1;
    setsockopt(listen_sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    
    struct sockaddr_in dest_addr;
    dest_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    dest_addr.sin_family = AF_INET;
    dest_addr.sin_port = htons(80);
    
    bind(listen_sock, (struct sockaddr *)&dest_addr, sizeof(dest_addr));
    listen(listen_sock, 1); // Cola de 1 cliente

    ESP_LOGI(TAG, "Servidor web iniciado en el puerto 80");

    while (1) {
        struct sockaddr_in source_addr;
        uint addr_len = sizeof(source_addr);
        int sock = accept(listen_sock, (struct sockaddr *)&source_addr, &addr_len);
        
        if (sock < 0) continue;

        char rx_buffer[1024] = {0};
        recv(sock, rx_buffer, sizeof(rx_buffer) - 1, 0);
        
        // Verifica si es el endpoint /guardar?
        if (strstr(rx_buffer, "GET /guardar?")) {
            char nuevo_ssid[32] = {0};
            char nueva_pass[64] = {0};
            
            // Extracción cruda del SSID y la Contraseña desde la URL
            char *ssid_ptr = strstr(rx_buffer, "ssid=");
            char *pass_ptr = strstr(rx_buffer, "&password=");
            
            if (ssid_ptr && pass_ptr) {
                ssid_ptr += 5; // Salta "ssid="
                int len_ssid = pass_ptr - ssid_ptr;
                strncpy(nuevo_ssid, ssid_ptr, len_ssid);
                
                pass_ptr += 10; // Salta "&password="
                char *http_ptr = strstr(pass_ptr, " HTTP");
                if (http_ptr) {
                    strncpy(nueva_pass, pass_ptr, http_ptr - pass_ptr);
                }
                
                // Reemplaza los '+' por espacios
                reemplazar_mas_por_espacios(nuevo_ssid);
                reemplazar_mas_por_espacios(nueva_pass);
                
                guardar_credenciales(nuevo_ssid, nueva_pass);
                
                send(sock, html_ok, strlen(html_ok), 0);
                close(sock);
                
                ESP_LOGI(TAG, "Credenciales guardadas. Reiniciando en 2 segundos...");
                vTaskDelay(pdMS_TO_TICKS(2000));
                esp_restart(); // Reemplazo directo de machine.reset()
            }
        } else {
            // Envía la página principal
            send(sock, html_page, strlen(html_page), 0);
        }
        close(sock);
    }
}

// Configura y levanta el modo Access Point (Equivalente a ap_mode.py)
void iniciar_ap_y_servidor() {
    esp_wifi_stop();
    wifi_config_t ap_config = {
        .ap = {
            .ssid = "ESP32_CONFIG",
            .ssid_len = strlen("ESP32_CONFIG"),
            .channel = 1,
            .authmode = WIFI_AUTH_OPEN, // Red abierta para evitar problemas
            .max_connection = 4,
        },
    };
    esp_wifi_set_mode(WIFI_MODE_AP);
    esp_wifi_set_config(WIFI_IF_AP, &ap_config);
    esp_wifi_start();
    
    ESP_LOGI(TAG, "MODO AP CREADO. Conectate a la red ESP32_CONFIG.");
    
    // Lanza el servidor web en una tarea de FreeRTOS
    xTaskCreate(tarea_servidor_web, "servidor_web", 4096, NULL, 5, NULL);
}

// Equivalente a wifi.py
void init_wifi() {
    esp_netif_init();
    esp_event_loop_create_default();
    esp_netif_create_default_wifi_sta();
    esp_netif_create_default_wifi_ap();

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    esp_wifi_init(&cfg);

    leer_credenciales();

    bool conectado = false;

    // Si existen credenciales, intenta modo STA
    if (strlen(ssid_guardado) > 0) {
        wifi_config_t sta_config = {0};
        strcpy((char *)sta_config.sta.ssid, ssid_guardado);
        strcpy((char *)sta_config.sta.password, pass_guardado);
        
        esp_wifi_set_mode(WIFI_MODE_STA);
        esp_wifi_set_config(WIFI_IF_STA, &sta_config);
        esp_wifi_start();
        esp_wifi_connect();

        ESP_LOGI(TAG, "Intentando conectar a %s...", ssid_guardado);

        // Bucle de intentos: 10 segundos
        for (int intentos = 0; intentos < 10; intentos++) {
            ESP_LOGI(TAG, "Conectando... intento %d/10", intentos + 1);
            vTaskDelay(pdMS_TO_TICKS(1000));
            
            wifi_ap_record_t ap_info;
            if (esp_wifi_sta_get_ap_info(&ap_info) == ESP_OK) {
                ESP_LOGI(TAG, "¡Conectado al Wi-Fi con éxito!");
                conectado = true;
                break;
            }
        }
    }

    if (!conectado) {
        ESP_LOGW(TAG, "ERROR - No se pudo conectar. Iniciando el AP.");
        iniciar_ap_y_servidor();
    }
}