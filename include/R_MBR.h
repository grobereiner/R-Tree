#ifndef R_MBR_H
#define R_MBR_H

#include <vector>
#include <utility>
#include <cstdlib>
#include "R_Nodo.h"

using namespace std;

struct R_MBR;
struct R_Nodo;

struct R_MBR
{
    R_MBR() = default;
    R_MBR(vector<pair<int, int>> tuplas);
    R_MBR(vector<pair<R_MBR, R_Nodo *>> mbrs);
    /*
    extremo[0]  ->  *-----
                    |    |
                    |    |
                    |    |
                    |    |
                    -----*  <- extremo[1]
    */
    pair<int, int> extremos[2];
    int agrandamiento(pair<int, int> tupla);
    void redimensionar_tuplas(vector<pair<int, int>> tuplas);

    void redimensionar_mbrs(vector<pair<R_MBR, R_Nodo *>> mbrs);
    bool dentro(pair<int, int> llave_tupla);
};

#endif