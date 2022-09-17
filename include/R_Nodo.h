#ifndef R_NODO_H
#define R_NODO_H

#include <vector>
#include <utility>
#include <cstdlib>
#include "Constante.h"
#include "R_MBR.h"

using namespace std;

struct R_MBR;
struct R_Nodo;

struct R_Nodo
{

    R_Nodo();
    R_Nodo(bool _hoja);

    R_Nodo *padre;
    // NODO HOJA
    vector<pair<int, int>> llaves_tupla;
    // NODO INTERNO
    vector<pair<R_MBR, R_Nodo *>> llaves_MBR_hijo;
    bool hoja;
};

#endif