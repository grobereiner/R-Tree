#ifndef R_POLIGONO_H
#define R_POLIGONO_H

#include<limits>
#include<vector>
#include<utility>
#include<algorithm>

using namespace std;

struct R_Poligono
{
    enum orientacion_t{
        colineal = 0,
        horario,
        antihorario
    };

    R_Poligono() = default;
    R_Poligono(vector<pair<int, int>> p);

    bool enSegmento(pair<int, int> a, pair<int, int> x, pair<int, int> b);
    orientacion_t orientacion(pair<int, int> a, pair<int, int> x, pair<int, int> b);
    bool intersecta(pair<int, int> a1, pair<int, int> a2, pair<int, int> b1, pair<int, int> b2);
    bool dentro(pair<int, int> tupla);


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