# Balanza Nutricional Inteligente ⚖️

Sistema embebido de pesaje y cálculo nutricional desarrollado sobre FreeRTOS para la placa EDU-CIAA NXP. Diseñado para medir porciones exactas y calcular macronutrientes en tiempo real, ideal para registrar métricas precisas para planes de hipertrofia o seguimiento dietético.

**Arquitectura de Software**
El firmware está estructurado mediante una Máquina de Estados Finitos (FSM) modular y tareas concurrentes, comunicadas de forma asíncrona a través de colas de mensajes del RTOS para garantizar un bajo consumo de CPU.

**Hardware Utilizado**
* **Cerebro:** EDU-CIAA NXP (LPC4337).
* **Adquisición de Datos:** Celda de carga con conversor HX711, implementando un filtro de umbral (*deadband*) analógico por software.
* **Interfaz Visual:** Pantalla manejada por bus SPI.
* **Conectividad:** Módulo ESP32 vía UART para telemetría y registro de macros.

**Estructura de Directorios**
* `inc/`: Cabeceras (`.h`), contratos de los módulos y configuración de los estados.
* `src/`: Implementación (`.c`), lógica de tareas del RTOS, drivers de comunicación y lecturas físicas.
