#ifndef NODO_H
#define NODO_H

#include<vector>
#include"Entrada.h"

using namespace std;

struct Entrada;

// Nodo 
struct Nodo{
    // Es hoja
    bool hoja;
    // Su padre
    Nodo* padre;
    // Almacenador de entradas
    vector<Entrada*> entradas;
};

#endif