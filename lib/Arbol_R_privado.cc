#include "../include/Arbol_R.h"


void Arbol_R::destruir_recursivo(Nodo* N){
    // Si es nodo interno
    if(!N->hoja){
        // Iterar por todas las entradas del nodo
        for(Entrada* e: N->entradas){
            // llamar a la función con su hijo
            destruir_recursivo(dynamic_cast<Entrada_Interna*>(e)->puntero_hijo);
            delete e;
        }
    }
    else{
        // Iterar por todas las entradas del nodo
        for(Entrada* e: N->entradas){
            // eliminar entrada
            delete e;
        }
    }
    // liberar nodo
    delete N;
}

bool Arbol_R::comparar_x(Entrada *a, Entrada *b) {
    // Comparar si un intervalo es menor que otro en el eje x
    if(a->intervalos[0].i1 < b->intervalos[0].i1){
        if(a->intervalos[0].i2 <= b->intervalos[0].i2)
            return true;
        return false;
    }
    if(a->intervalos[0].i1 > b->intervalos[0].i1){
        if(a->intervalos[0].i2 < b->intervalos[0].i2)
            return false;
        return true;
    }
    return a->intervalos[0].i2 < b->intervalos[0].i2;
}

bool Arbol_R::comparar_y(Entrada *a, Entrada *b) {
    // Comparar si un intervalo es menor que otro en el eje y
    if(a->intervalos[1].i1 < b->intervalos[1].i1){
        if(a->intervalos[1].i2 <= b->intervalos[1].i2)
            return true;
        return false;
    }
    if(a->intervalos[1].i1 > b->intervalos[1].i1){
        if(a->intervalos[1].i2 < b->intervalos[1].i2)
            return false;
        return true;
    }
    return a->intervalos[1].i2 < b->intervalos[1].i2;
}