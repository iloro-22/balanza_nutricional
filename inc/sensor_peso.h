#ifndef SENSOR_PESO_H
#define SENSOR_PESO_H

#define PIN_SCK   GPIO0   // Pin del reloj
#define PIN_DOUT  GPIO1 //jay que reveer esto mas adelante

void task_sensor_peso(void *taskParmPtr);

#endif