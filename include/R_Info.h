#ifndef R_INFO_H
#define R_INFO_H

#include "R_Poligono.h"
#include<vector>
#include<utility>

using namespace std;

struct R_Info
{
    R_Info(pair<int, int> c);
    R_Info(vector<pair<int, int>> p);

    bool operator==(const R_Info &other);
    bool operator!=(const R_Info &other);

    pair<int, int> info_tupla;
    R_Poligono info_poligono;
    bool poligono;
};

#endif