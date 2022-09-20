#include "../../include/R_Entidades/R_Info.h"

R_Info::R_Info(pair<int, int> c)
    : info_tupla(c), poligono(false) {}

R_Info::R_Info(vector<pair<int, int>> p)
    : poligono(true), info_poligono(p) {}

bool R_Info::operator==(const R_Info &other)
{
    if (poligono != other.poligono)
        return false;
    if (!poligono)
    {
        return info_tupla == other.info_tupla;
    }
    for (int i = 0; i < other.info_poligono.tuplas.size(); i++)
    {
        if (info_poligono.tuplas[i] != other.info_poligono.tuplas[i])
            return false;
    }
    return true;
}

bool R_Info::operator!=(const R_Info &other)
{
    return !(*this == other);
}

int R_Info::get_left()
{
    if (!poligono)
        return info_tupla.first;
    return info_poligono.extremos[0].first;
}

int R_Info::get_right()
{
    if (!poligono)
        return info_tupla.first;
    return info_poligono.extremos[1].first;
}

int R_Info::get_up()
{
    if (!poligono)
        return info_tupla.second;
    return info_poligono.extremos[0].second;
}

int R_Info::get_down()
{
    if (!poligono)
        return info_tupla.second;
    return info_poligono.extremos[1].second;
}