#ifndef NODO_H
#define NODO_H

#include<vector>
#include"Entrada.h"

using namespace std;

struct Entrada;

struct Nodo{
    bool hoja;
    Nodo* padre;
    vector<Entrada*> entradas;
};

#endif