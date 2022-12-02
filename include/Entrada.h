#ifndef ENTRADA_H
#define ENTRADA_H

#include<vector>
#include"Intervalo.h"

using namespace std;

struct Intervalo;

// Etnrada abstracta de un nodo
struct Entrada{
    Entrada();
    // MBR
    vector<Intervalo> intervalos;
    // Determinar si un punto está dentro
    bool dentro(int x, int y);
    // Destructor virtual para realizar dynamicc_Cast
    virtual ~Entrada();
};

#endif