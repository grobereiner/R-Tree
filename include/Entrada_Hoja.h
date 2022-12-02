#ifndef ENTRADA_HOJA_H
#define ENTRADA_HOJA_H

#include<vector>
#include<limits>

#include"Entrada.h"
#include"Punto.h"

using namespace std;

struct Punto;

// Herencia para nodos hojas
struct Entrada_Hoja: public Entrada{

    // ENUM DEPRECADO
    enum orientacion_t{
        colineal = 0,
        horario,
        antihorario
    };

    Entrada_Hoja();
    // CONSTRUCTOR
    Entrada_Hoja(const vector<Punto> &t);
    ~Entrada_Hoja() override;
    // Puntos que definen un punto o poligono
    vector<Punto> tuplas;
    // Determinar si un punto esta en el MBR
    bool dentro(Punto P);
    
    // DEPRECADO
    bool enSegmento(Punto a, Punto x, Punto b);
    // DEPRECADO
    orientacion_t orientacion(Punto a, Punto x, Punto b);
    // DEPRECADO
    bool intersecta(Punto a1, Punto a2, Punto b1, Punto b2);
};

#endif 