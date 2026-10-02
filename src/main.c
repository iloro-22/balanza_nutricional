// Lo declarás globalmente
TaskHandle_t handle_sensor_peso = NULL;

// Y cuando creás la tarea, le pasás la variable al final:
xTaskCreate(task_sensor_peso, "SensorPeso", 256, NULL, 1, &handle_sensor_peso);