#ifndef ENTRADA_H
#define ENTRADA_H

#include<vector>
#include"Intervalo.h"

using namespace std;

struct Intervalo;

struct Entrada{
    Entrada();
    vector<Intervalo> intervalos;
    bool dentro(int x, int y);
    virtual ~Entrada();
};

#endif