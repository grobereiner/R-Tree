#ifndef R_MBR_H
#define R_MBR_H

#include<algorithm>
#include <vector>
#include <utility>
#include <cstdlib>
#include "R_Nodo.h"
#include "R_Info.h"

using namespace std;

struct R_MBR;
struct R_Nodo;

struct R_MBR
{
    R_MBR() = default;
    R_MBR(R_Info tupla, int radio);
    R_MBR(vector<R_Info> tuplas);
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
    int agrandamiento(R_Info tupla);
    void redimensionar_tuplas(vector<R_Info> tuplas);
    bool intercepta(R_MBR otro);
    pair<int, int> medio();

    void redimensionar_mbrs(vector<pair<R_MBR, R_Nodo *>> mbrs);
    bool dentro(R_Info llave_tupla);
};

#endif