#include "../include/R_Poligono.h"

R_Poligono::R_Poligono(vector<pair<int, int>> p)
    : tuplas(p)
{
    extremos[0] = p[0];
    extremos[1] = p[0];
    for (auto i : tuplas)
    {
        if (i.first < extremos[0].first)
            extremos[0].first = i.first;
        if (i.first > extremos[1].first)
            extremos[1].first = i.first;
        if (i.second < extremos[1].second)
            extremos[1].second = i.second;
        if (i.second > extremos[0].second)
            extremos[0].second = i.second;
    }
}