#ifndef ENTRADA_INTERNA_H
#define ENTRADA_INTERNA_H

#include "Nodo.h"
#include "Entrada.h"

struct Nodo;

struct Entrada_Interna: public Entrada{
    Entrada_Interna();
    ~Entrada_Interna() override;
    Nodo* puntero_hijo;
};

#endif 