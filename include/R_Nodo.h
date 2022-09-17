#ifndef R_NODO_H
#define R_NODO_H

#include<vector>
#include<utility>
#include "Constante.h"
#include "R_MBR.h"

struct R_Nodo
{

    R_Nodo() = default;
    R_Nodo(bool _hoja);

    R_Nodo *padre = nullptr;
    // NODO HOJA
    vector<pair<int, int>> llaves_tupla;
    // NODO INTERNO
    vector<pair<R_MBR, R_Nodo *>> llaves_MBR_hijo;
    bool hoja;
};

#endif