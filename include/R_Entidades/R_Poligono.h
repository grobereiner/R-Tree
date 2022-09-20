#ifndef R_POLIGONO_H
#define R_POLIGONO_H

#include<limits>
#include<vector>
#include<utility>

using namespace std;

struct R_Poligono
{
    R_Poligono() = default;
    R_Poligono(vector<pair<int, int>> p);

    vector<pair<int,int>> tuplas;
    /*
    extremo[0]  ->  *-----
                    |    |
                    |    |
                    |    |
                    |    |
                    -----*  <- extremo[1]
    */
    pair<int, int> extremos[2];
};

#endif