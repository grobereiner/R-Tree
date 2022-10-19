#ifndef ENTRADA_H
#define ENTRADA_H

#include<vector>
#include"Intervalo.h"

using namespace std;

struct Intervalo;

struct Entrada{
    Entrada();
    vector<Intervalo> intervalos;
    virtual ~Entrada();
};

#endif