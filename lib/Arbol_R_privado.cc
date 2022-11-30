#include "../include/Arbol_R.h"

void Arbol_R::destruir_recursivo(Nodo* N){
    if(!N->hoja){
        for(Entrada* e: N->entradas){
            destruir_recursivo(dynamic_cast<Entrada_Interna*>(e)->puntero_hijo);
            delete e;
        }
    }
    else{
        for(Entrada* e: N->entradas){
            delete e;
        }
    }
    delete N;
}

bool Arbol_R::comparar_x(Entrada *a, Entrada *b) {
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