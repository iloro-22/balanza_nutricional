buimport csv

codigo_c = """// Archivo autogenerado - NO EDITAR A MANO
#include "alimentos.h"

// Los datos corresponden a los valores cada 100 gramos
const informacion_alimento tabla_base[] = {
"""

with open('macros.csv', 'r', encoding='utf-8') as archivo:
    lector = csv.DictReader(archivo)
    for fila in lector:
        # Formatea cada linea del CSV como un elemento del struct en C
        codigo_c += f"    {{{fila['proteinas']}, {fila['carbohidratos']}, {fila['grasas']}, {fila['calorias']}}}, // ID {fila['id']}: {fila['nombre']}\n"

codigo_c += "};\n"

with open('alimentos.c', 'w', encoding='utf-8') as salida:
    salida.write(codigo_c)
    
print("Archivo alimentos.c generado con exito.")