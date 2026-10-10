#include "arbol_menu.h"
#include <stddef.h>

// 1. Nivel 3: Hojas (Alimentos finales con ID real para cálculo de macros)
static NodoMenu_t alim_lomo     = { "Lomo",     1, true, NULL, {NULL}, 0 };
static NodoMenu_t alim_pechuga  = { "Pechuga",  2, true, NULL, {NULL}, 0 };
static NodoMenu_t alim_bondiola = { "Bondiola", 3, true, NULL, {NULL}, 0 };

static NodoMenu_t alim_espinaca = { "Espinaca", 4, true, NULL, {NULL}, 0 };
static NodoMenu_t alim_lentejas = { "Lentejas", 5, true, NULL, {NULL}, 0 };
static NodoMenu_t alim_papa     = { "Papa",     6, true, NULL, {NULL}, 0 };

// 2. Nivel 2: Subcategorías
static NodoMenu_t sub_vaca       = { "Vaca",       0, false, NULL, { &alim_lomo },     1 };
static NodoMenu_t sub_pollo      = { "Pollo",      0, false, NULL, { &alim_pechuga },  1 };
static NodoMenu_t sub_cerdo      = { "Cerdo",      0, false, NULL, { &alim_bondiola }, 1 };

static NodoMenu_t sub_hojas      = { "Hojas",      0, false, NULL, { &alim_espinaca }, 1 };
static NodoMenu_t sub_legumbres  = { "Legumbres",  0, false, NULL, { &alim_lentejas }, 1 };
static NodoMenu_t sub_tuberculos = { "Tuberculos", 0, false, NULL, { &alim_papa },     1 };

// 3. Nivel 1: Categorías Principales
static NodoMenu_t cat_carne   = { "Carne",   0, false, NULL, { &sub_vaca, &sub_pollo, &sub_cerdo }, 3 };
static NodoMenu_t cat_verdura = { "Verdura", 0, false, NULL, { &sub_hojas, &sub_legumbres, &sub_tuberculos }, 3 };

// 4. Nivel 0: Raíz del Sistema
NodoMenu_t arbol_raiz = { "ALIMENTOS", 0, false, NULL, { &cat_carne, &cat_verdura }, 2 };

// Función para enlazar los punteros padre hacia arriba
void arbol_alimentos_init(void) {
    cat_carne.padre   = &arbol_raiz;
    cat_verdura.padre = &arbol_raiz;

    sub_vaca.padre       = &cat_carne;
    sub_pollo.padre      = &cat_carne;
    sub_cerdo.padre      = &cat_carne;

    sub_hojas.padre      = &cat_verdura;
    sub_legumbres.padre  = &cat_verdura;
    sub_tuberculos.padre = &cat_verdura;

    alim_lomo.padre     = &sub_vaca;
    alim_pechuga.padre  = &sub_pollo;
    alim_bondiola.padre = &sub_cerdo;

    alim_espinaca.padre = &sub_hojas;
    alim_lentejas.padre = &sub_legumbres;
    alim_papa.padre     = &sub_tuberculos;
}