#ifndef ENTRADA_HOJA_H
#define ENTRADA_HOJA_H

#include<vector>
#include<limits>

#include"Entrada.h"
#include"Punto.h"

using namespace std;

struct Punto;

struct Entrada_Hoja: public Entrada{

    enum orientacion_t{
        colineal = 0,
        horario,
        antihorario
    };

    Entrada_Hoja();
    Entrada_Hoja(const vector<Punto> &t);
    ~Entrada_Hoja() override;
    vector<Punto> tuplas;
    bool dentro(Punto P);
    
    bool enSegmento(Punto a, Punto x, Punto b);
    orientacion_t orientacion(Punto a, Punto x, Punto b);
    bool intersecta(Punto a1, Punto a2, Punto b1, Punto b2);
};

#endif 