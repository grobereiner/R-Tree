#ifndef ENTRADA_INTERNA_H
#define ENTRADA_INTERNA_H

#include "Nodo.h"
#include "Entrada.h"

struct Nodo;

// Herencia para nodos internos
struct Entrada_Interna: public Entrada{
    Entrada_Interna();
    ~Entrada_Interna() override;
    // Puntro a nodo hijo
    Nodo* puntero_hijo;
};

#endif 